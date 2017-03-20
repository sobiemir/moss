#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "moss/moss.h"

#define FPS 60

#define COLOR_GREEN(_T_)   "\033[0;32;32m" _T_ "\033[0m"
#define COLOR_CYAN(_T_)    "\033[0;36m"    _T_ "\033[0m"
#define COLOR_RED(_T_)     "\033[0;32;31m" _T_ "\033[0m"
#define COLOR_YELLOW(_T_)  "\033[0;33m"    _T_ "\033[0m"
#define COLOR_MAGNETA(_T_) "\033[0;35m"    _T_ "\033[0m"
#define COLOR_BLUE(_T_)    "\033[0;32;34m" _T_ "\033[0m"

#define KEY_UP      0x00
#define KEY_DOWN    0x01
#define KEY_LEFT    0x02
#define KEY_RIGHT   0x03

const int LIST[] =
{
    313, 392, 403, 283, 413, 431, 463, 450, 378, 506, 384, 477, 289, 376, 346, 296, 311, 465, 436, 434, 312,
    299, 371, 482, 402, 325, 263, 448, 491, 381, 308, 512, 342, 280, 290, 417, 305, 359, 336, 498, 363, 326,
    368, 508, 279, 453, 318, 406, 271, 319, 293, 354, 374, 323, 459, 454, 507, 467, 440, 292, 470, 306, 409,
    270, 488, 393, 341, 257, 358, 416, 284, 343, 473, 495, 266, 464, 383, 460, 361, 380, 432, 304, 439, 369,
    339, 273, 348, 286, 404, 499, 272, 394, 423, 476, 382, 487, 469, 309, 366, 489, 405, 501, 267, 377, 301,
    397, 462, 412, 461, 347, 320, 474, 269, 294, 276, 445, 370, 388, 492, 430, 437, 481, 315, 387, 365, 256,
    421, 446, 410, 332, 471, 395, 265, 324, 504, 355, 295, 303, 335, 468, 351, 505, 420, 386, 458, 372, 281,
    285, 373, 344, 400, 274, 379, 398, 298, 364, 277, 327, 411, 475, 331, 362, 494, 316, 428, 288, 291, 422,
    391, 457, 287, 300, 338, 390, 389, 350, 456, 407, 356, 375, 441, 401, 333, 275, 502, 509, 511, 349, 337,
    490, 424, 414, 435, 282, 451, 496, 353, 260, 317, 314, 352, 329, 418, 302, 340, 345, 442, 447, 321, 486,
    357, 478, 503, 500, 307, 310, 367, 297, 278, 264, 268, 425, 360, 429, 443, 419, 322, 426, 497, 484, 427,
    466, 433, 334, 479, 483, 261, 455, 510, 444, 262, 415, 449, 452, 472, 328, 493, 396, 399, 259, 438, 330,
    385, 480, 258, 485
};

// TWORZENIE PRZYKŁADOWEJ TABLICY
// =====================================================================================================================
MS_ARRAY *mstst_create_sample_array( size_t size )
{
    MS_ARRAY *array = ms_array_alloc( sizeof(int), size > 256 ? 256 : size );
    int ercode;

    ercode = ms_array_push_values( array, LIST, size > 256 ? 256 : size );

    return array;
}

void array_join_slice()
{
    MS_ARRAY *array1 = mstst_create_sample_array( 256 );
    MS_ARRAY array2;
    MS_ARRAY array3;
    int ercode;
    size_t x, y;
    int *items;

    ercode = ms_array_init( &array2, array1->ItemSize, 64 );

    // inicjalizacja tablicy z innym rozmiarem elementów
    ercode = ms_array_init( &array3, array1->ItemSize * 2, 64 );

    // dodaj wartości z innej tablicy z podanego zakresu
    ercode = ms_array_join_slice( &array2, array1, 64, 64 );

    // dodaj wartości z tej samej tablicy
    ercode = ms_array_join( &array2, &array2 );

    // dodaj wartości z innej tablicy z podanego zakresu
    ercode = ms_array_join_slice( &array2, array1, 192, 64 );

    // błąd - indeks poza zakresem
    ercode = ms_array_join_slice( &array2, array1, 192, 256 );

    // próbuj skopiować wartości do tablicy z innymi rozmiarami elementów
    ercode = ms_array_join( &array3, &array2 );

    // sprawdź czy w tablicy znajdują się poprawne elementy
    items = (int*)array2.Items;
    for( x = 0, y = 64; y < 128; ++x, ++y )
        ;// cr_assert_eq( items[x], LIST[y] );
    // 64 elementy powinny być kopią
    for( y = 64; y < 128; ++x, ++y )
        ;// cr_assert_eq( items[x], LIST[y] );
    for( y = 192; y < 256; ++x, ++y )
        ;// cr_assert_eq( items[x], LIST[y] );

    ms_array_free( array1 );
    ms_array_free( &array2 );
}

struct GAMEMAIN
{
    ALLEGRO_DISPLAY     *Display;
    ALLEGRO_EVENT_QUEUE *EventQueue;
    ALLEGRO_TIMER       *Timer;
    int                  Running,
                         Redraw,
                         Keys[4];
    float                Position[4];
}
GameMain =
{
    NULL,   // Display
    NULL,   // EventQueue
    NULL,   // Timer
    1,      // Running
    0,      // Redraw
    {       // Keys
        0,      // KEY_UP
        0,      // KEY_DOWN
        0,      // KEY_LEFT
        0       // KEY_RIGHT
    }, {    // Position
        -10,    // X1
        -10,    // Y1
        0,      // X2
        0       // Y2
    }
};

void check_failure( int what, int code )
{
    if( what )
        return;

    al_uninstall_keyboard();
    al_shutdown_primitives_addon();

    if( GameMain.EventQueue )
        al_destroy_event_queue( GameMain.EventQueue );
    if( GameMain.Display )
        al_destroy_display( GameMain.Display );
    if( GameMain.Timer )
        al_destroy_timer( GameMain.Timer );

    if( code != 0 )
        exit( code );
}

// podłoże
typedef struct S_MAPDATA
{
    int Columns,    // ilość kolumn mapy
        Rows,       // ilość wierszy mapy
        Total,      // ilość wszystkich kafelków
       *Tiles,      // identyfikatory kafelków
       *Variants,   // warianty kafelków
        CheckID;    // identyfikator aktualnego kafelka
}
MAPDATA;

typedef struct S_TILEDATA
{
    int Width,
        Height;

    ALLEGRO_BITMAP *Bitmap;
}
TILEDATA;

int at_mapstd_get_dims( const char *name, int *w, int *h );
int at_mapstd_parse( const char *name, MAPDATA *const map );
void at_map_free( MAPDATA *const map );

int main( int argc, char **argv )
{
    MS_ARRAY array;

    size_t aidx = 0;

    // MS_TILEMAP tilemap;
    // MS_TILE *tarr;
    void *tmpitm;

    TILEDATA tiles[5];
    int al = 0;

    int a[] = {
        20,
        40,
        11,
        14,
        1,
        6,
        8,
        22,
        27,
        33,
        2,
        5,
        6,
        8,
        9,
        37,
        31,
        34,
        28,
        21
    };

    array_join_slice();

//     ms_array_init( &array, sizeof(int), 20 );
//     while( al < 20 )
//     {
//         ms_array_mpush( &array, &a[al++] );
//         printf( "Capacity: %zu, Length: %zu\n", array.Capacity, array.Length );
//     }

//     while( (tmpitm = ms_array_mpop(&array)) )
//         printf( "%d\n", *(int*)tmpitm ),
//         free( tmpitm );

//     // inicjalizacja zależności biblioteki allegro
//     check_failure( al_init()                 , -1 );
//     check_failure( al_install_keyboard()     , -2 );
//     check_failure( al_init_primitives_addon(), -3 );
//     check_failure( al_init_image_addon()     , -4 );

//     // okno programu
//     GameMain.Display = al_create_display( 640, 640 );
//     check_failure( GameMain.Display != NULL, -10 );
    
//     // kolejka zdarzeń
//     GameMain.EventQueue = al_create_event_queue();
//     check_failure( GameMain.EventQueue != NULL, -11 );

//     // czasomierz
//     GameMain.Timer = al_create_timer( 1.0 / FPS );
//     check_failure( GameMain.Timer != NULL, -12 );

//     // rejestracja zdarzeń
//     al_register_event_source( GameMain.EventQueue, al_get_keyboard_event_source() );
//     al_register_event_source( GameMain.EventQueue, al_get_display_event_source(GameMain.Display) );
//     al_register_event_source( GameMain.EventQueue, al_get_timer_event_source(GameMain.Timer) );

//     al_clear_to_color( al_map_rgb(0, 0, 0) );
//     al_flip_display();

//     // at_mapstd_parse( "./map/test/ground1.map", &map );

//     ms_tilemap_init( "test", &tilemap, NULL );
//     ms_tilemap_load( &tilemap );

//     tarr = (MS_TILE*)tilemap.Tiles.Items;

//     // rozpocznij odmierzanie czasu
//     al_start_timer( GameMain.Timer );

//     while( GameMain.Running )
//     {
//         // czekaj na zdarzenie
//     	ALLEGRO_EVENT ev;
//         al_wait_for_event( GameMain.EventQueue, &ev );

//         switch( ev.type )
//         {
//             case ALLEGRO_EVENT_KEY_DOWN:
//                 switch( ev.keyboard.keycode )
//                 {
//                     case ALLEGRO_KEY_UP:
//                         GameMain.Keys[KEY_UP] = 1;
//                     break;
//                     case ALLEGRO_KEY_DOWN:
//                         GameMain.Keys[KEY_DOWN] = 1;
//                     break;
//                     case ALLEGRO_KEY_LEFT:
//                         GameMain.Keys[KEY_LEFT] = 1;
//                     break;
//                     case ALLEGRO_KEY_RIGHT:
//                         GameMain.Keys[KEY_RIGHT] = 1;
//                     break;
//                     case ALLEGRO_KEY_ESCAPE:
//                         GameMain.Running = 0;
//                     break;
//                 }
//             break;
//             case ALLEGRO_EVENT_KEY_UP:
//                 switch( ev.keyboard.keycode )
//                 {
//                     case ALLEGRO_KEY_UP:
//                         GameMain.Keys[KEY_UP] = 0;
//                     break;
//                     case ALLEGRO_KEY_DOWN:
//                         GameMain.Keys[KEY_DOWN] = 0;
//                     break;
//                     case ALLEGRO_KEY_LEFT:
//                         GameMain.Keys[KEY_LEFT] = 0;
//                     break;
//                     case ALLEGRO_KEY_RIGHT:
//                         GameMain.Keys[KEY_RIGHT] = 0;
//                     break;
//                 }
//             break;
//             case ALLEGRO_EVENT_DISPLAY_CLOSE:
//                 GameMain.Running = 0;
//             break;
//             case ALLEGRO_EVENT_TIMER:
//             {
//                 float shiftx = (float)(-1 * GameMain.Keys[KEY_LEFT] + 1 * GameMain.Keys[KEY_RIGHT]),
//                       shifty = (float)(-1 * GameMain.Keys[KEY_UP]   + 1 * GameMain.Keys[KEY_DOWN] );
                
//                 if( shiftx != 0.f && shifty != 0.f )
//                     shiftx *= 0.7f,
//                     shifty *= 0.7f;

//                 GameMain.Position[0] += shiftx;
//                 GameMain.Position[1] += shifty;
//                 GameMain.Position[2] += shiftx;
//                 GameMain.Position[3] += shifty;

//                 GameMain.Redraw = 1;
//             }
//             break;
//         }

//         // ponowne rysowanie całej sceny
//         if( GameMain.Redraw )
//         {
//             int   lidx = 0,
//                   pidx = 0,
//                   sidx = 0,
//                   fidx = 0;
//             float posx = 0,
//                   posy = 0;

//             GameMain.Redraw = 0;

//             al_hold_bitmap_drawing( true );

//             while( lidx < tilemap.GroundLayers )
//             {
//                 uint *tiledata = tilemap.Ground[lidx];

//                 posy = 0.f;
//                 posx = 0.f;

//                 pidx = 0,
//                 sidx = tilemap.Length;
//                 fidx = 0;

//                 while( pidx < tilemap.Length )
//                 {
//                     if( fidx == tilemap.Columns )
//                         posy += 32,
//                         posx  = 0.f,
//                         fidx  = 0;

//                     if( tiledata[pidx] != 0 )
//                     {
// /*                        if( tiledata[sidx] != 0 )
//                             al_draw_rotated_bitmap( tarr[tiledata[pidx]].Frame[0],
//                                 16.f, 16.f, posx + 16.f, posy + 16.f, tarr[tiledata[pidx]].Angles[tiledata[sidx]], 0 );
//                         else
//                             al_draw_bitmap( tarr[tiledata[pidx]].Frame[0], posx, posy, 0 ); */
//                     }

//                     posx += 32;
//                     ++pidx;
//                     ++sidx;
//                     ++fidx;
//                 }
//                 ++lidx;
//             }

//             al_hold_bitmap_drawing( false );

//             al_draw_filled_rectangle( GameMain.Position[0], GameMain.Position[1], GameMain.Position[2], GameMain.Position[3],
//                 al_map_rgb(255, 0, 0) );
    
//             al_flip_display();
//             al_clear_to_color( al_map_rgb(255, 255, 255) );
//         }
    // }

    // ms_tilemap_free( &tilemap );

    // // zwolnij miejsce w pamięci
    // check_failure( 0, 0 );
    // return 0;
    return 0;
}
