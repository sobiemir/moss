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

#ifndef __MSH_TILEMAP__
#define __MSH_TILEMAP__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../configuration.h"
#include "../buffer.h"
#include "../array.h"
#include "./tile.h"

typedef struct MSS_TILEMAPCFG
{
    uint TileMask;
    uint VariantMask;
    uint TileShift;
    char Separator;
    int  ReturnChar;
}
MS_TILEMAPCFG;

typedef struct MSS_TILELAYER
{
    /** Przesunięcie warstwy względem osi X. */
    int ShiftX;
    /** Przesunięcie warstwy względem osi Y. */
    int ShiftY;
    /** Ilość kolumn wyświetlanych kafelków na mapie. */
    int Columns;
    /** Ilość wierszy wyświetlanych kafelków na mapie. */
    int Rows;
    /** Szerokość pojedynczego kafelka - warstwa musi mieć takie same wymiary kafelków. */
    int TileWidth;
    /** Wysokość pojedynczego kafelka - warstwa musi mieć takie same wymiary kafelków. */
    int TileHeight;
    /** Identyfikatory kafelków wczytane z pliku warstwy. */
    MS_ARRAY *TileID;
    /** Jeden wielki kafelek zawierający wszystkie używane przez warstwę kafelki. */
    MS_TILE Tile;
}
MS_TILELAYER;

typedef struct MSS_TILEMAP
{
    int Columns;
    int Rows;
    float Zoom;
    int TileWidth;
    int TileHeight;

    size_t Length;
    size_t GroundLayers;
    uint **Ground;
    size_t CollisionID;
    const char *MapName;
    size_t NameLength;
    char FileName[64];
    int MapRegion[2];
    MS_TILEMAPCFG Config;
    bool Destroy;
    bool Optimized;
    MS_ARRAY Tiles;
    MS_TILE SingleTile;
}
MS_TILEMAP;

MS_TILEMAP *ms_tilemap_create( const char *name, const MS_TILEMAPCFG *conf );
int ms_tilemap_init( const char *name, MS_TILEMAP *const map, const MS_TILEMAPCFG *conf );
int ms_tilemap_load( MS_TILEMAP *const map );
int ms_tilemap_optimize( MS_TILEMAP *map );
int ms_tilemap_draw( const MS_TILEMAP *map, int sx, int sy );
void ms_tilemap_free( MS_TILEMAP *map );

#endif
