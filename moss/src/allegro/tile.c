/*
 *  Moss Library >>> http://moss.aculo.pl
 *
 *     /'\_/`\                           
 *    /\      \    ___     ____    ____  
 *    \ \ \__\ \  / __`\  /',__\  /',__\ 
 *     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 *      \ \_\\ \_\ \____/\/\____/\/\____/
 *       \/_/ \/_/\/___/  \/___/  \/___/ 
 *
 *  See LICENSE file for copyright information.
 */

#include "../../inc/allegro/tile.h"

int ms_tile_load( const char *iname, MS_TILE *tile )
{
    int ecode = MSEC_OK;

    assert( tile );
    assert( iname );

    if( !(tile->Bitmap = al_load_bitmap(iname)) )
        return MSEC_NULL_POINTER;
    if( (ecode = ms_array_init(&tile->Bounds, sizeof(MS_RECTI), 1)) )
        return ecode;

    tile->Width  = al_get_bitmap_width( tile->Bitmap );
    tile->Height = al_get_bitmap_height( tile->Bitmap );

    ((MS_RECTI*)tile->Bounds.Items)[0].x1 = 0;
    ((MS_RECTI*)tile->Bounds.Items)[0].y1 = 0;
    ((MS_RECTI*)tile->Bounds.Items)[0].x2 = tile->Width;
    ((MS_RECTI*)tile->Bounds.Items)[0].y2 = tile->Height;

    tile->SingleSize = true;
    tile->Destroy    = false;
    tile->FileName   = (char*)iname;

    return MSEC_OK;
}

void ms_tile_single_size( MS_TILE *tile, int width, int height )
{
    tile->SingleSize = true;
}

void ms_tile_free( MS_TILE *tile )
{
    size_t tiles = 0;
/*
    if( tile->Frame )
        while( tiles < tile->Length )
            al_destroy_bitmap( tile->Frame[tiles++] );
    free( tile->Frame );
*/
    if( tile->Destroy )
        free( tile );
}

void ms_tile_draw( const MS_TILE *tile )
{
    assert( tile );
}

void ms_tile_draw_frame( const MS_TILE *tile, size_t frame )
{
    assert( tile );
}
