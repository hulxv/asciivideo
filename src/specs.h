#ifndef _SPEC_H
#define _SPEC_H

#include <stdint.h>

typedef struct
{
  uint32_t
      frames_count; // the count of frames of the video that will be rendererd
  uint16_t fps;     // Count of the frames that will be rendererd per second
  float duration;   // Duration of the video in seconds
  uint32_t width;
  uint32_t height;

} specs;

/*
 * Create a new structure for specifications of video.
 */

specs *specs_new(unsigned int frames_count, unsigned short fps, float duration, uint32_t width, uint32_t height);

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

char *specs_serialize(specs *specifications);

/*
 * Convert the string into struct depend on previous schema:
 *  "frames_count,fps,duration,width,height"
 */
specs *specs_deserialize(char *str);



specs *specs_read_from(char *file_path);

//function reads data from text file and return the deserialized string


#endif
