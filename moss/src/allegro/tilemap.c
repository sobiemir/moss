#include "../../inc/allegro/tilemap.h"

/**
 * Pobiera ilość warstw mapy o danej nazwie terenu.
 * W przypadku gdy mapa posiada kilka warstw, to warto je wszystkie wczytać i wyświetlić.
 * Tekstury mogą być nakładane na siebie, dzięki czemu jest ich mniej.
 * Taka operacja dodatkowo sprawia wrażenie większej różnorodności wyświetlanego świata.
 *
 * @param {MS_TILEMAP*} map  - Informacje o mapie dla której sprawdzane będą warstwy.
 * @param {const char*} elem - Nazwa terenu dla którego mają być liczone warstwy.
 */
static uint msf_tilemap_layers( MS_TILEMAP *map, const char *elem );

/**
 * Wczytuje warstwę o podanym terenie i numerze.
 * Funkcja pozwala na wczytanie warstwy terenu do pamięci komputera, dzięki czemu może być później wyświetlona.
 * Jest to część funkcji pozwalającej na wczytanie całej mapy.
 * Poszczególne warstwy można wykluczyć z rysowania przy wywoływaniu funkcji rysującej świat.
 *
 * @param {MS_TILEMAP*} map   - Informacje o mapie dla której wczytywany będzie teren.
 * @param {const char*} name  - Nazwa terenu do wczytania.
 * @param {uint}        layer - Numer warstwy terenu do wczytania.
 */
static int msf_tilemap_load_layer( MS_TILEMAP *map, const char *name, uint layer );

static int msf_tilemap_load_tiles( MS_TILEMAP *map );
static uint msf_tilemap_count_tiles( const MS_TILEMAP *map );
static int msf_tilemap_load_tiles_files( MS_TILEMAP *map );

/* =====================================================================================================================
    TWORZENIE NOWEGO OBIEKTU MAPY
===================================================================================================================== */

MS_TILEMAP *ms_tilemap_create( const char *name, const MS_TILEMAPCFG *conf )
{
    MS_TILEMAP *map    = NULL;
    int         retval = 0;

    if( !(map = malloc(sizeof *map)) )
        SETERRNOANDRETURNC( MSEC_MEMORY_ALLOCATION, NULL );
    
    if( (retval = ms_tilemap_init(name, map, conf)) )
        SETERRNOANDRETURNC( retval, NULL );

    map->Destroy = true;
    return map;
}

/* =====================================================================================================================
    INICJALIZACJA DANYCH
===================================================================================================================== */

int ms_tilemap_init( const char *name, MS_TILEMAP *map, const MS_TILEMAPCFG *conf )
{
    FILE *file  = NULL; // uchwyt do pliku
    int   chr   = -1,   // wczytany znak ze strumienia
          isnum = 0;    // czy aktualnie przetwarzana wartość jest numerem

    assert( map );
    assert( name );

    memset( map, 0, sizeof *map );

    // nazwa mapy jest za długa
    if( (map->NameLength = strlen(name)) >= 44 )
        SETERRNOANDRETURN( MSEC_TOO_LONG_STRING );

    memcpy( map->FileName, "./map/", 6 );
    memcpy( &map->FileName[6], name, map->NameLength );
    memcpy( &map->FileName[6 + map->NameLength], "/ground0.map", 13 );

    map->NameLength += 7;
    map->MapName     = name;
    map->Zoom        = 1.f;
    map->Rows        = 1;

    if( !(file = fopen(map->FileName, "r")) )
        return MSEC_NULL_POINTER;
    
    if( conf != NULL )
        memcpy( &map->Config, conf, sizeof *conf );
    else
        map->Config = (MS_TILEMAPCFG){
            0xFF00,
            0x00FF,
            0x8,
            ' ',
            0
        };

    // policz ilość kolumn
    while( (chr = fgetc(file)) != -1 )
        if( (chr >= '0' && chr <= '9') || (chr >= 'A' && chr <='F') )
            isnum = 1;
        else if( chr == map->Config.Separator)
            isnum = isnum
                ? (map->Columns++, 0)
                : 0;
        else if( chr == '\r' || chr == '\n' )
            break;

    // brak kolumn lub tylko jeden wiersz
    if( map->Columns == 0 )
        return
            fclose(file),
            MSEC_BAD_CONTENT;

    // CR+LF lub CR, strzelaj CR
    if( chr == '\r' )
        map->Config.ReturnChar = 2;
    
    // no to teraz na pewno CR+LF
    if( (chr = fgetc(file)) == '\n' )
        map->Config.ReturnChar = 1;
    else
        ungetc( chr, file );

    // licz ilość wierszy
    isnum = 0;
    while( (chr = fgetc(file)) != -1 )
        if( chr == '\n' && map->Config.ReturnChar == 1 )
            continue;
        else if( chr == '\r' || chr == '\n' )
            isnum = isnum
                ? 2
                : (map->Rows++, 1);
        else
            isnum = 0;

    // zwiększ ilość wierszy gdy ostatnia linia nie kończy się znakiem nowej linii
    if( !isnum )
        map->Rows++;

    fclose( file );
    return MSEC_OK;
}

/* =====================================================================================================================
    ŁADOWANIE MAPY
===================================================================================================================== */

int ms_tilemap_load( MS_TILEMAP *map )
{
    FILE *file   = NULL;
    uint  index  = 0;
    int   retval = MSEC_OK;

    assert( map );
    assert( map->Columns && map->Rows );

    map->Length       = map->Columns * map->Rows;
    map->GroundLayers = msf_tilemap_layers( map, "ground" );
    map->Ground       = calloc( map->GroundLayers, sizeof *map->Ground );

    if( !map->Ground || map->GroundLayers == 0 )
        return
            free( map->Ground ),
            (map->Ground = NULL),
            MSEC_MEMORY_ALLOCATION;

    // wczytaj warstwy terenu
    while( index < map->GroundLayers )
        if( (retval = msf_tilemap_load_layer(map, "ground", index++)) )
            break;
    
    if( retval )
        ms_tilemap_free( map );
    
    // wczytaj kafelki potrzebne do odtworzenia mapy
    if( (retval = msf_tilemap_load_tiles(map)) )
        ms_tilemap_free( map );

    if( map->Tiles.Length == 0 )
        return retval;

    map->TileHeight = ((MS_TILE*)map->Tiles.Items)[0].Height;
    map->TileWidth  = ((MS_TILE*)map->Tiles.Items)[0].Width;

    return retval;
}

int ms_tilemap_optimize( MS_TILEMAP *map )
{
    int   optsize = 0;
    float range   = 0.f;
    
    assert( map );
    
    optsize = (range = map->Tiles.Length / 2.f * map->TileWidth,
          range <= 32.f   ? 32
        : range <= 64.f   ? 64
        : range <= 128.f  ? 128
        : range <= 256.f  ? 256
        : range <= 512.f  ? 512
        : range <= 1024.f ? 1024
        : range <= 2048.f ? 2048
        : range <= 4096.f ? 4096
        : 0 );
    
    if( !optsize )
        return MSEC_OUT_OF_RANGE;
    
    // utwórz bitmapę do umieszczania wszystkich map
    map->SingleTile.Bitmap     = al_create_bitmap( optsize, optsize );
    map->SingleTile.Width      = optsize;
    map->SingleTile.Height     = optsize;
    map->SingleTile.Destroy    = false;
    map->SingleTile.FileName   = NULL;
    map->SingleTile.SingleSize = false;

    return MSEC_OK;
}

/* =====================================================================================================================
    RYSOWANIE MAPY
===================================================================================================================== */

int ms_tilemap_draw( const MS_TILEMAP *map, int sx, int sy )
{
    MS_TILE *tile = NULL;

    assert( map );

    if( map->Tiles.Length == 0 )
        SETERRNOANDRETURN( MSEC_EMPTY_ARRAY );

    return MSEC_OK;
}

/* =====================================================================================================================
    ZWALNIANIE PAMIĘCI
===================================================================================================================== */

void ms_tilemap_free( MS_TILEMAP *map )
{
    MS_TILE *tile  = NULL;
    size_t   index = 0;

    assert( map );

    // zwolnij pamięć po podłożu
    while( index < map->GroundLayers )
        free( map->Ground[index] ),
        map->Ground[index++] = NULL;
    
    free( map->Ground );
    map->Ground = NULL;

    // zwolnij pamięć po kafelkach
    tile = (MS_TILE*)map->Tiles.Items;
    for( index = 0; index < map->Tiles.Length; ++index )
        free( tile[index].FileName ),
        ms_tile_free( &tile[index] );

    // zwolnij pamięć po tablicy
    ms_array_free( &map->Tiles );

    if( map->Destroy )
        free( map );
}

/* =====================================================================================================================
    WCZYTYWANIE KAFELKÓW Z PLIKU
===================================================================================================================== */

int msf_tilemap_load_tiles( MS_TILEMAP *map )
{
    FILE    *file  = NULL;
    uint     tiles = 0,
             index = 0;
    MS_TILE *tarr  = NULL;

    assert( map );
    
    memcpy( &map->FileName[map->NameLength], "tiles.lst", 10 );
    tiles = msf_tilemap_count_tiles( map );

    if( tiles == 0 )
        return MSEC_BAD_CONTENT;

    // inicjalizacja tablicy
    if( (index = ms_array_init(&map->Tiles, sizeof(MS_TILE), tiles + 1)) )
        return index;

    memset( map->Tiles.Items, 0, sizeof(MS_TILE) * (tiles + 1) );

    // ładowanie wszystkich kafelków
    map->Tiles.Length = tiles + 1;
    if( msf_tilemap_load_tiles_files(map) )
        return MSEC_CHAIN_ALLOCATION;

    tarr  = (MS_TILE*)map->Tiles.Items;
    tiles = MSEC_OK;

    // wczytywanie obrazków dla wszystkich kafelków
    for( index = 0; index < map->Tiles.Length; ++index )
        if( tarr[index].FileName != NULL )
            tiles = ms_tile_load( tarr[index].FileName, &tarr[index] ) | tiles;

    // problem specjalny - błąd w serii
    if( tiles )
        return MSEC_CHAIN_ALLOCATION;

    return MSEC_OK;
}

/* =====================================================================================================================
    ŁADOWANIE OBRAZKÓW DLA KAFELEK
===================================================================================================================== */

int msf_tilemap_load_tiles_files( MS_TILEMAP *map )
{
    MS_TILE *tarr = NULL;

    FILE *file = NULL;
    char  chr  = -1;
    int   mode = 0,
          idx1 = 6,
          idx2 = 0,
          idx3 = 1,
          rval = MSEC_OK;

    char filepath[256] = {0};

    assert( map );
    tarr = (MS_TILE*)map->Tiles.Items;

    // otwórz plik
    if( !(file = fopen(map->FileName, "r")) )
        return MSEC_FILE_OPEN;     

    memcpy( filepath, "./img/", 6 );

    // pobieraj ścieżki
    while( (chr = fgetc(file)) != -1 )
        // tryb pobierania znaków
        if( (mode == 1 || mode == 2 || mode == 3) && (chr != '\n' && chr != '\r') )
            if( mode == 1 && chr != ' ' )
                filepath[idx1++] = chr;
            else if( mode == 2 && chr != ' ' )
                filepath[idx2++] = chr;
            else
                continue;
        // koniec trybu pobierania nazwy paczki
        else if( mode == 1 )
            filepath[idx1++] = '/',
            mode = 0,
            idx2 = idx1;
        // koniec trybu pobierania nazwy pliku
        else if( mode == 2 )
            filepath[idx2++] = mode = 0,
            rval |= !( tarr[idx3].FileName = malloc(sizeof *tarr[idx3].FileName * (idx2 + 1)) )
                ? MSEC_MEMORY_ALLOCATION
                : (memcpy(tarr[idx3].FileName, filepath, idx2 + 1), MSEC_OK),
            idx3++,
            idx2 = idx1;
        else if( chr == '\n' || chr == '\r' )
            mode = 0;
        // włącz tryb pobierania nazwy paczki
        else if( chr == '>' )
            mode = 1,
            idx1 = 6;
        // komentarz
        else if( chr == '#' )
            mode = 3;
        // włącz tryb pobierania nazwy pliku
        else
            mode = 2,
            filepath[idx2++] = chr;

    fclose( file );
    return rval
        ? MSEC_CHAIN_ALLOCATION
        : MSEC_OK;
}

/* =====================================================================================================================
    ZLICZANIE KAFELEK
===================================================================================================================== */

uint msf_tilemap_count_tiles( const MS_TILEMAP *map )
{
    FILE *file  = NULL;
    uint  tiles = 0;
    int   chr   = -1,
          mode  = 0;
        
    char package[32] = {0},
         fpath[256]  = {0};

    if( !(file = fopen(map->FileName, "r")) )
        return 0;
    
    while( (chr = fgetc(file)) != -1 )
        if( (mode == 1 || mode == 2) && (chr != '\n' && chr != '\r') )
            continue;
        else if( mode == 2 )
            tiles++,
            mode = 0;
        else if( chr == '\n' || chr == '\r' )
            mode = 0;
        else if( chr == '>' || chr == '#' )
            mode = 1;
        else
            mode = 2;
    
    fclose( file );
    return tiles;
}

/* =====================================================================================================================
    OBLICZANIE ILOŚCI WARSTW
===================================================================================================================== */

uint msf_tilemap_layers( MS_TILEMAP *map, const char *elem )
{
    FILE *file = NULL;
    uint  num  = 0,
          slen = 0;
    
    assert( map );
    assert( elem );

    if( (slen = strlen(elem)) > 6 )
        return 0;

    memcpy( &map->FileName[map->NameLength], elem, slen );
    memcpy( &map->FileName[map->NameLength + slen], "0.map", 6 );

    while( (file = fopen(map->FileName, "r")) && num < 99 )
        sprintf( &map->FileName[map->NameLength + 6], "%d.map", ++num ),
        fclose( file );

    return num;
}

/* =====================================================================================================================
    WCZYTYWANIE WARSTWY MAPY
===================================================================================================================== */

int msf_tilemap_load_layer( MS_TILEMAP *map, const char *name, uint layer )
{
    FILE  *file  = NULL;
    int    chr   = -1,
           isnum = 0;
    uint  *tiles = NULL;    // skrót do listy kafelków
    size_t idx1  = 0,       // indeks zapisu numeru kafelka
           idx2  = 0;       // indeks do tablicy strnum lub wariantów

    char strnum[16] = {0};

    assert( map );
    assert( name );

    // jeżeli jakimś cudem udało się zapisać poza przydzieloną pamięcią
    if( sprintf(&map->FileName[map->NameLength], "%s%u.map", name, layer) > 13 )
        return MSEC_TOO_LONG_STRING;
    if( !(file = fopen(map->FileName, "r")) )
        return MSEC_NULL_POINTER;

    // wygląd pamięci: (xxxxxx yyyyyy), gdzie x to numer kafelka a y to wariant dla siatki 3x3 kafelki
    free( map->Ground[layer] );
    map->Ground[layer] = calloc( map->Length * 2, sizeof **map->Ground );

    if( !map->Ground[layer] )
        return
            fclose( file ),
            MSEC_MEMORY_ALLOCATION;

    // // pobieraj dane
    tiles = map->Ground[layer];
    while( (chr = fgetc(file)) != -1 )
        if( chr == '\n' && map->Config.ReturnChar == 1 )
            continue;
        else if( chr == '\r' || chr == '\n' )
            isnum = isnum
                ? (tiles[idx1++] = ms_atoui(strnum, MSTC_HEXDEC_UPCASE), 0)
                : 0,
            idx2 = 0;
        else if( idx2 < 16 && ((chr >= '0' && chr <= '9') || (chr >= 'A' && chr <='F')) )
            isnum = 1,
            strnum[idx2++] = chr;
        else if( chr == ' ' )
            isnum = isnum
                ? (tiles[idx1++] = ms_atoui((strnum[idx2] = ' ', strnum), MSTC_HEXDEC_UPCASE), 0)
                : 0,
            idx2 = 0;

    // oddziel warianty od identyfikatorów kafelków
    for( isnum = tiles[idx1 = 0], idx2 = map->Length; idx1 < map->Length; isnum = tiles[++idx1], ++idx2 )
        tiles[idx2] = isnum > map->Config.VariantMask
            ? ((tiles[idx1] = (isnum & map->Config.TileMask) >> map->Config.TileShift), isnum & map->Config.VariantMask)
            : 0;

    fclose( file );
    return MSEC_OK;
}
