#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "moss/moss.h"
#include "moss/trunk/xxhash.h"
#include "moss/trunk/xxhsum.c"

#define FPS 60

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

int main( int argc, char **argv )
{
    const char *str = "SDBMAlgorithm";
    
    printf( "32_DJB2   : %x\n", ms_hash_32_djb2(str, strlen(str)) );
    printf( "32_DJB2-A : %x\n", ms_hash_32_djb2a(str, strlen(str)) );
    printf( "32_FNV-1  : %x\n", ms_hash_32_fnv1(str, strlen(str)) );
    printf( "32_FNV-1A : %x\n", ms_hash_32_fnv1a(str, strlen(str)) );
    printf( "32_JOAAT  : %x\n", ms_hash_32_joaat(str, strlen(str)) );
    printf( "32_MURMUR3: %x\n", ms_hash_32_murmur3(str, strlen(str)) );
    printf( "32_MURMUR2: %x\n", ms_hash_32_murmur2(str, strlen(str)) );
    printf( "32_MURMUR : %x\n", ms_hash_32_murmur(str, strlen(str)) );
    printf( "32_SDBM   : %x\n", ms_hash_32_sdbm(str, strlen(str)) );
    printf( "64_FNV-1  : %lx\n", ms_hash_64_fnv1(str, strlen(str)) );
    printf( "64_FNV-1A : %lx\n", ms_hash_64_fnv1a(str, strlen(str)) );
    printf( "64_MURMUR2: %lx\n", ms_hash_64_murmur2(str, strlen(str)) );
    printf( "32_XXHASH : %x => %x\n", ms_hash_32_xxhash(str, strlen(str)), XXH32( str, strlen(str), 1234 ) );
    printf( "64_XXHASH : %lx => %llx\n", ms_hash_64_xxhash(str, strlen(str)), XXH64( str, strlen(str), 1234 ) );

    return 0;
}
