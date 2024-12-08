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
    // free(new_specs);
}



                                                    //Serialize and convert the struct into string
char *specs_serialize(specs *specifications)
{
    if (specifications==NULL)
        return NULL;


                                                    // Calculate the required buffer size dynamically
    int required_size = snprintf (NULL,0, "%u,%hu,%.1f,%u,%u",          /*The snprintf() function is used to redirect the output of printf() function onto a buffer.*/
                                    specifications->frames_count,             
                                    specifications->fps,
                                    specifications->duration,
                                    specifications->width,
                                    specifications->height);

                                                    // Check if snprintf succeeded in calculating the required size
    if (required_size < 0)  {
        fprintf(stderr, "Error calculating required size\n");
        return NULL;
    }

                                                    // Allocate memory for the string (by the required_size)
    char *buffer = (char *)malloc(required_size + 1);            //+1 for the null terminator (as now we deal with characters)
                                                   
    if (buffer==NULL) {                                          //check if memory allocation was successful
        fprintf(stderr,"Memory allocation failed\n");              
        return NULL;
        }
                                                    //write the formatted string
    snprintf (buffer,required_size + 1, "%u,%hu,%.1f,%u,%u",            //Write formatted output to sized buffer
                            specifications->frames_count,             
                            specifications->fps,
                            specifications->duration,
                            specifications->width,
                            specifications->height);

    return buffer;                                       
    // free(serialized);
}


                                                    //The specs_deserialize function will convert a string back into a specs structure
specs *specs_deserialize(char *str)
{
    if (str == NULL) {
        return NULL;
    }
                                                    // Allocate memory for the new specs object
    specs *new_specs = (specs *)malloc(sizeof(specs));
    if (new_specs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
//using sscanf to read values from the comma-separated string and store them in the corresponding fields of the specs structure
    sscanf(str, "%u,%hu,%f,%u,%u",              
                            &new_specs->frames_count, 
                            &new_specs->fps, 
                            &new_specs->duration, 
                            &new_specs->width,
                            &new_specs->height);

    return new_specs;
    // free(deserialized);
}