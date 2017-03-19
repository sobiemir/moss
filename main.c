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

    MS_TILEMAP tilemap;
    MS_TILE *tarr;
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

    ms_array_init( &array, sizeof(int), 20 );
    while( al < 20 )
    {
        ms_array_mpush( &array, &a[al++] );
        printf( "Capacity: %zu, Length: %zu\n", array.Capacity, array.Length );
    }

    while( (tmpitm = ms_array_mpop(&array)) )
        printf( "%d\n", *(int*)tmpitm ),
        free( tmpitm );

    // inicjalizacja zależności biblioteki allegro
    check_failure( al_init()                 , -1 );
    check_failure( al_install_keyboard()     , -2 );
    check_failure( al_init_primitives_addon(), -3 );
    check_failure( al_init_image_addon()     , -4 );

    // okno programu
    GameMain.Display = al_create_display( 640, 640 );
    check_failure( GameMain.Display != NULL, -10 );
    
    // kolejka zdarzeń
    GameMain.EventQueue = al_create_event_queue();
    check_failure( GameMain.EventQueue != NULL, -11 );

    // czasomierz
    GameMain.Timer = al_create_timer( 1.0 / FPS );
    check_failure( GameMain.Timer != NULL, -12 );

    // rejestracja zdarzeń
    al_register_event_source( GameMain.EventQueue, al_get_keyboard_event_source() );
    al_register_event_source( GameMain.EventQueue, al_get_display_event_source(GameMain.Display) );
    al_register_event_source( GameMain.EventQueue, al_get_timer_event_source(GameMain.Timer) );

    al_clear_to_color( al_map_rgb(0, 0, 0) );
    al_flip_display();

    // at_mapstd_parse( "./map/test/ground1.map", &map );

    ms_tilemap_init( "test", &tilemap, NULL );
    ms_tilemap_load( &tilemap );

    tarr = (MS_TILE*)tilemap.Tiles.Items;

    // rozpocznij odmierzanie czasu
    al_start_timer( GameMain.Timer );

    while( GameMain.Running )
    {
        // czekaj na zdarzenie
    	ALLEGRO_EVENT ev;
        al_wait_for_event( GameMain.EventQueue, &ev );

        switch( ev.type )
        {
            case ALLEGRO_EVENT_KEY_DOWN:
                switch( ev.keyboard.keycode )
                {
                    case ALLEGRO_KEY_UP:
                        GameMain.Keys[KEY_UP] = 1;
                    break;
                    case ALLEGRO_KEY_DOWN:
                        GameMain.Keys[KEY_DOWN] = 1;
                    break;
                    case ALLEGRO_KEY_LEFT:
                        GameMain.Keys[KEY_LEFT] = 1;
                    break;
                    case ALLEGRO_KEY_RIGHT:
                        GameMain.Keys[KEY_RIGHT] = 1;
                    break;
                    case ALLEGRO_KEY_ESCAPE:
                        GameMain.Running = 0;
                    break;
                }
            break;
            case ALLEGRO_EVENT_KEY_UP:
                switch( ev.keyboard.keycode )
                {
                    case ALLEGRO_KEY_UP:
                        GameMain.Keys[KEY_UP] = 0;
                    break;
                    case ALLEGRO_KEY_DOWN:
                        GameMain.Keys[KEY_DOWN] = 0;
                    break;
                    case ALLEGRO_KEY_LEFT:
                        GameMain.Keys[KEY_LEFT] = 0;
                    break;
                    case ALLEGRO_KEY_RIGHT:
                        GameMain.Keys[KEY_RIGHT] = 0;
                    break;
                }
            break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                GameMain.Running = 0;
            break;
            case ALLEGRO_EVENT_TIMER:
            {
                float shiftx = (float)(-1 * GameMain.Keys[KEY_LEFT] + 1 * GameMain.Keys[KEY_RIGHT]),
                      shifty = (float)(-1 * GameMain.Keys[KEY_UP]   + 1 * GameMain.Keys[KEY_DOWN] );
                
                if( shiftx != 0.f && shifty != 0.f )
                    shiftx *= 0.7f,
                    shifty *= 0.7f;

                GameMain.Position[0] += shiftx;
                GameMain.Position[1] += shifty;
                GameMain.Position[2] += shiftx;
                GameMain.Position[3] += shifty;

                GameMain.Redraw = 1;
            }
            break;
        }

        // ponowne rysowanie całej sceny
        if( GameMain.Redraw )
        {
            int   lidx = 0,
                  pidx = 0,
                  sidx = 0,
                  fidx = 0;
            float posx = 0,
                  posy = 0;

            GameMain.Redraw = 0;

            al_hold_bitmap_drawing( true );

            while( lidx < tilemap.GroundLayers )
            {
                uint *tiledata = tilemap.Ground[lidx];

                posy = 0.f;
                posx = 0.f;

                pidx = 0,
                sidx = tilemap.Length;
                fidx = 0;

                while( pidx < tilemap.Length )
                {
                    if( fidx == tilemap.Columns )
                        posy += 32,
                        posx  = 0.f,
                        fidx  = 0;

                    if( tiledata[pidx] != 0 )
                    {
/*                        if( tiledata[sidx] != 0 )
                            al_draw_rotated_bitmap( tarr[tiledata[pidx]].Frame[0],
                                16.f, 16.f, posx + 16.f, posy + 16.f, tarr[tiledata[pidx]].Angles[tiledata[sidx]], 0 );
                        else
                            al_draw_bitmap( tarr[tiledata[pidx]].Frame[0], posx, posy, 0 ); */
                    }

                    posx += 32;
                    ++pidx;
                    ++sidx;
                    ++fidx;
                }
                ++lidx;
            }

            al_hold_bitmap_drawing( false );

            al_draw_filled_rectangle( GameMain.Position[0], GameMain.Position[1], GameMain.Position[2], GameMain.Position[3],
                al_map_rgb(255, 0, 0) );
    
            al_flip_display();
            al_clear_to_color( al_map_rgb(255, 255, 255) );
        }
    }

    ms_tilemap_free( &tilemap );

    // zwolnij miejsce w pamięci
    check_failure( 0, 0 );
    return 0;
}
