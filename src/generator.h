#ifndef _ASCII_GEN_H
#define _ASCII_GEN_H

#include "./specs.h"
#include "./video.h"

typedef struct
{
    const char *src;
    const char *dest;
    video *vid;
} ascii_video_gen;

/**
 * Creating a new ascii video generator struct.
 */
ascii_video_gen *ascii_viden_gen_new(const char *src, const char *dest, video *vid);

/**
 * Running the generator.
 */
int ascii_video_gen_run(ascii_video_gen *gen);

#endif