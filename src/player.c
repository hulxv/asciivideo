
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <math.h>

#include "player.h"
#include "helpers.h"

bool is_audio_stopped = false;

static void audio_stop(pid_t audio_pid)
{
    if (audio_pid > 0)
    {
        // Send termination signal to audio process
        int result = kill(audio_pid, SIGSTOP);
        if (result == -1)
        {
            ERROR("Failed to stop audio playback: %s", strerror(errno));
        }
        else
        {
            is_audio_stopped = true;
        }
    }
}

static void audio_resume(pid_t audio_pid)
{
    if (audio_pid > 0)
    {
        // Send termination signal to audio process
        int result = kill(audio_pid, SIGCONT);
        if (result == -1)
        {
            ERROR("Failed to resume audio playback: %s", strerror(errno));
        }
        else
        {
            is_audio_stopped = false;
        }
    }
}

static int print_frame(char *filename, WINDOW *frame_pad, int width, int height)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        INFO("Error: Could not open file %s\n", filename);
        return -1;
    }

    werase(frame_pad);
    char line[width + 1];
    for (int y = 0; y < height; y++)
    {
        if (fgets(line, sizeof(line), file) == NULL)
            break;

        mvwprintw(frame_pad, y, 0, "%.*s", width, line);
    }

    fclose(file);
    return 0;
}

player *player_new(const char *src, specs *specs)
{
    player *player = malloc(sizeof(player));
    player->src = src;
    player->specs = specs;
    return player;
}

int player_video_run(player *player)
{
    double fps = player->specs->fps;
    uint32_t frames_count = player->specs->frames_count;
    double duration = player->specs->duration;

    int frame_delay_us = 1000000 / fps;

    int width = player->specs->width * 4, height = player->specs->height;

    INFO("Video dimensions: width = %i, height = %i\n", width, height);

    int offset_y, offset_x, term_rows, term_cols, prev_term_rows, prev_term_cols;

    getmaxyx(stdscr, prev_term_rows, prev_term_cols);
    WINDOW *frame_pad = newpad(height, width);

    if (!frame_pad)
    {
        ERROR("Could not create frame pad\n");
        return -1;
    }

    uint64_t video_start_time = get_current_time_us();

    for (int i = 1; i <= frames_count; i++)
    {
        uint64_t frame_target_time = video_start_time + i * frame_delay_us * 1;

        do
        {
            getmaxyx(stdscr, term_rows, term_cols);
            offset_y = (term_rows - height) / 2;
            offset_x = (term_cols - width) / 2;

            if (offset_y >= 0 && offset_x >= 0)
            {
                if (player->specs->audio)
                    audio_resume(player->audio_pid);
                break;
            }
            else
            {
                clear();
                mvprintw(term_rows / 2, (term_cols - 30) / 2, "Resize terminal to %dx%d", width, height);
                mvprintw(term_rows / 2 + 1, (term_cols - 30) / 2, "Current: %dx%d", term_cols, term_rows);
                refresh();

                if (player->specs->audio)
                    audio_stop(player->audio_pid);
                usleep(100000);
            }
        } while (1);

        char filename[128];
        snprintf(filename, sizeof(filename), "%s/%i.ascii", player->src, i);
        print_frame(filename, frame_pad, width, height);

        if (term_rows != prev_term_rows || term_cols != prev_term_cols)
        {
            clear();
            refresh();
        }

        prefresh(frame_pad, 0, 0, offset_y, offset_x, offset_y + height - 1, offset_x + width - 1);
        uint64_t current_time = get_current_time_us();
        if (frame_target_time > current_time)
        {
            usleep(frame_target_time - current_time);
        }

        prev_term_rows = term_rows;
        prev_term_cols = term_cols;
    }

    delwin(frame_pad);
    return 0;
}

/**
 * Play a WAV audio file using aplay
 * @param audio_file Path to the WAV audio file to play
 * @return 0 on success, -1 on failure
 */
int player_audio_run(player *player)
{

    char audio_file[1024];
    snprintf(audio_file, sizeof(audio_file), "%s/audio.wav", player->src);
    
    // Validate input parameters
    if (!audio_file || strlen(audio_file) == 0)
    {
        ERROR("Invalid audio file path");
        return -1;
    }

    // Check if file exists
    if (access(audio_file, F_OK) == -1)
    {
        ERROR("Audio file does not exist: %s", audio_file);
        return -1;
    }

    // Fork a child process
    player->audio_pid = fork();

    if (player->audio_pid == -1)
    {
        // Fork failed
        ERROR("Failed to fork process");
        return -1;
    }
    else if (player->audio_pid == 0)
    {
        // Child process
        // Execute aplay to play the audio file
        // Use execl to replace the current process with aplay
        execl("/usr/bin/aplay",
              "aplay",    // argv[0]
              "-q",       // quiet mode (suppress output)
              audio_file, // audio file to play
              NULL);

        // If execl fails, exit the child process
        ERROR("Failed to execute aplay");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        // Wait for the child process to complete
        waitpid(player->audio_pid, &status, 0);

        // Check if the process exited normally
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status == 0)
            {
                INFO("Audio playback completed");
                return 0;
            }
            else
            {
                ERROR("Audio playback failed with exit code %d", exit_status);
                return -1;
            }
        }
        else
        {
            ERROR("Audio playback process did not exit normally");
            return -1;
        }
    }
}
