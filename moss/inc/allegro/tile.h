#ifndef __MSH_TILE__
#define __MSH_TILE__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../array.h"

typedef struct MSS_RECTI
{
    int x1;
    int y1;
    int x2;
    int y2;
}
MS_RECTI;

typedef struct MSS_TILE
{
    bool SingleSize;
    bool Destroy;
    int Width;
    int Height;
    MS_ARRAY Bounds;
    char *FileName;
    ALLEGRO_BITMAP *Bitmap;
}
MS_TILE;

int ms_tile_load( const char *iname, MS_TILE *const tile );
void ms_tile_single_size( MS_TILE *tile, int width, int height );
void ms_tile_sizes( MS_ARRAY *sizes );
void ms_tile_free( MS_TILE *tile );
void ms_tile_draw( const MS_TILE *tile );
void ms_tile_draw_frame( const MS_TILE *tile, size_t frame );
void ms_tile_draw_rotated( const MS_TILE *tile, float angle );

#endif
