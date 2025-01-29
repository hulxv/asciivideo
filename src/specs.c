#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "specs.h"
#include "helpers.h"

const char *SPECS_FILE_NAME = "specsfile";

specs *specs_new(uint64_t frames_count, double fps, double duration, uint32_t width, uint32_t height, bool audio)
{
    specs *new_specs = (specs *)malloc(sizeof(specs));

    if (new_specs == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    new_specs->frames_count = frames_count;
    new_specs->fps = fps;
    new_specs->duration = duration;
    new_specs->width = width;
    new_specs->height = height;
    new_specs->audio = audio;
    return new_specs;
}

char *specs_serialize(specs *s)
{
    if (s == NULL)
        return NULL;

    int required_size = snprintf(NULL, 0, "%lu;%lf;%lf;%lu;%lu;%i",
                                 s->frames_count,
                                 s->fps,
                                 s->duration,
                                 s->width,
                                 s->height,
                                 s->audio);

    if (required_size < 0)
    {
        fprintf(stderr, "Error calculating required size\n");
        return NULL;
    }

    char *buffer = (char *)malloc(required_size + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    snprintf(buffer, required_size + 1, "%lu;%lf;%lf;%lu;%lu;%i",
             s->frames_count,
             s->fps,
             s->duration,
             s->width,
             s->height,
             s->audio);

    return buffer;
}

specs *specs_deserialize(char *str)
{
    if (str == NULL)
    {
        return NULL;
    }

    specs *new_specs = (specs *)malloc(sizeof(specs));
    if (new_specs == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    int parsed = sscanf(str, "%u;%lf;%lf;%lu;%lu;%i",
                        &new_specs->frames_count,
                        &new_specs->fps,
                        &new_specs->duration,
                        &new_specs->width,
                        &new_specs->height,
                        &new_specs->audio);
    if (parsed < 6)
    {
        return NULL;
    }

    return new_specs;
}

int specs_write_to(specs *s, char *dest_dir)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "%s/%s", dest_dir, SPECS_FILE_NAME);

    FILE *f = fopen(buf, "w");

    if (f == NULL)
    {
        return -1;
    }
    char *content = specs_serialize(s);
    fputs(content, f);
    fclose(f);

    return 0;
}

/**
 * Reading specifcations from a source directory.
 *
 * It will return NULL if it couldn't parse the file according to invalid schema.
 */
specs *specs_read_from(char *src_dir)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "%s/%s", src_dir, SPECS_FILE_NAME);

    FILE *f = fopen(buf, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open file for reading\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return NULL;
    }

    fread(file_content, 1, file_size, f);
    file_content[file_size] = '\0';

    fclose(f);

    specs *s = specs_deserialize(file_content);
    free(file_content);

    return s;
}
