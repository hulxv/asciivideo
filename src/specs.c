#include "./specs.h"
#include "./helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>                                //to use uint32_t without any problems

typedef struct                                     //Define the specs structure
{
    unsigned int frames_count;
    unsigned short fps;
    float duration;
    uint32_t width;
    uint32_t height;
}specs;


                                                    /*The specs_new function will allocate memory 
                                                    for a new specs structure and initialize it with the provided values*/
specs *specs_new(unsigned int frames_count, unsigned short fps, float duration, uint32_t width, uint32_t height)
{                                                         
                                                    //dynamic memory allocation for new_specs structre
    specs *new_specs = (specs *)malloc(sizeof(specs));            //pointer to structure

                                                    //check if memory allocation was successful
    if (new_specs==NULL) {
        fprintf(stderr,"Memory allocation failed\n");              //fprintf function writes the string to the file.txt
        return NULL;
    }
                                                    // Initialization the fields of the  (specs_new) structure
    new_specs->frames_count = frames_count;
    new_specs->fps = fps;
    new_specs->duration = duration;
    new_specs->width = width;
    new_specs->height = height;
    return new_specs;
    free(new_specs);
}

                                                    //Serialize and convert the struct into string
char *specs_serialize(specs *specifications)
{
    UNIMPLEMENTED();
}





/*
 * Convert the string into struct depend on previous schema:
 *  "frames_count,fps,duration,width,height"
 */
specs *specs_deserialize(char *str)
{
    UNIMPLEMENTED();
}