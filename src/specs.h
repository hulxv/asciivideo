#ifndef _SPEC_H
#define _SPEC_H

#include <stdint.h>
#include <stdbool.h>
typedef struct
{
  uint32_t
      frames_count; // the count of frames of the video that will be rendererd
  double fps;       // Count of the frames that will be rendererd per second
  double duration;  // Duration of the video in seconds
  uint32_t width;
  uint32_t height;
  bool audio;

} specs;

/*
 * Create a new structure for specifications of video.
 */

specs *specs_new(uint64_t frames_count, double fps, double duration, uint32_t width, uint32_t height, bool audio);

/*
 * Serialize and convert the struct into string depend on this schema:
 *  "frames_count,fps,duration,"
 *
 * *Example:*
 *  specs* specs = specs_new(1000, 30, 3043.0, 320, 200);
 *  char* specs_as_str = specs_serialize(specs);
 *
 *  assert(strcmp(specs_as_str, "1000,30,3043.0,320,200"))
 */

char *specs_serialize(specs *s);

/*
 * Convert the string into struct depend on previous schema:
 *  "frames_count,fps,duration,width,height"
 * 
 *  * It will return NULL if it couldn't parse the string according to invalid schema.
 */
specs *specs_deserialize(char *str);

int specs_write_to(specs *s, char *dest_dir);

specs *specs_read_from(char *src_dir);

#endif
