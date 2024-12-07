#include "./player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

 void print_frame(char *frame_path, int width, int height)
{
    FILE *file = fopen(frame_path, "r");
    if(file == NULL)
    {
        printf("Could not open file %s \n", frame_path);
        return;
    }

    char buffer[256];
    while(fgets(buffer, sizeof(buffer), file) != NULL)
        printf("%s", buffer);

    fclose(file);
}


video_player *viden_player_new(char *src, specs *specs)
{
    video_player *player = malloc(sizeof(video_player));
    player->src = src;
    player->specs = specs_deserialize(src);
    return player;
}


int video_player_run(video_player *player)
{
    int fps = player->specs->fps;
    int width = player->specs->width;
    int height = player->specs->width;
    char frame_path[256];

    for(int i = 0; i < player->specs->frames_count; i++)
    {
        snprintf(frame_path, sizeof(frame_path), "%s/%d.ascii", player->src, i);

        print_frame(frame_path, width, height);

        usleep(1000000 / fps);
    }

    return 0;
}
