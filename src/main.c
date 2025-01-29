#define __USE_MISC
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define __USE_GNU
#include <getopt.h>
#include <math.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "frame.h"
#include "generator.h"
#include "helpers.h"
#include "pixel.h"
#include "player.h"
#include "specs.h"
#include "video.h"

void *audio_routine(void *p) { return (void *)player_audio_run((player *)p); }
void *video_routine(void *p) { return (void *)player_video_run((player *)p); }

void print_help()
{
  printf("Usage: asciiplayer [OPTION]... [FILE]...\n");
  printf("Convert video to ASCII art or play ASCII art from a directory.\n\n");
  printf("Options:\n");
  printf("  -g, --generate <video-src> <out>  Generate ASCII art from video "
         "and save to <out>.\n");
  printf(
      "  -p, --play <src-dir>              Play ASCII art from <src-dir>.\n");
  printf("  -h, --help                        Display this help and exit.\n");
}

void generate_ascii_video(const char *src, const char *out)
{
  struct stat st;
  bool generate = true;
  if (lstat(out, &st) == -1)
  {
    mkdir(out, 0755);
  }
  else
  {
    generate = false;

    char ans;
    printf("Directory is already exist. Do you want to overwrite it (y/N)? ");
    scanf("%c", &ans);
    if (ans == 'y' || ans == 'Y')
      generate = true;
  }

  if (!generate)
  {
    INFO("Generating ASCII Video was cancelled");
    return;
  }

  video *vid = video_new(src, out);

  ascii_video_gen *gen = ascii_viden_gen_new(src, out, vid);
  ascii_video_gen_run(gen);

  char buf[1024];
  snprintf(buf, sizeof(buf), "%s/audio.wav", out);

  int audio_ext_ret = video_extract_audio(vid, buf);
  if (audio_ext_ret != 0)
  {
    WARN("Couldn't extract the audio and play it.");
  }

  AVRational framerate =
      av_guess_frame_rate(vid->fmt_ctx, vid->video_stream, vid->frame);
  double fps = framerate.num / framerate.den;
  double duration = vid->fmt_ctx->duration / (double)AV_TIME_BASE;
  int frames_count = fps * duration;

  specs *specs = specs_new(
      frames_count, fps, duration, vid->video_stream->codecpar->width,
      vid->video_stream->codecpar->height, audio_ext_ret == 0 ? true : false);
  if (specs_write_to(specs, out) == 0)
  {
    INFO("specs file was created, content={%s}", specs_serialize(specs));
  }
  else
  {
    ERROR("Couldn't create specs file.");
  }
  video_clean_up(vid);
}

void play_ascii_video(char *src)
{
  specs *specs = specs_read_from(src);
  player *player = player_new(src, specs);

  pthread_t th_video;
  pthread_t th_audio;
  initscr();
  noecho();
  curs_set(0);

  void *ret_audio, *ret_video;

  if (specs->audio > 0) {
    pthread_create(&th_audio, NULL, audio_routine, (void *)player);
    pthread_setname_np(th_video, "audio-player-runner");
  }
  specs->audio == 0 && pthread_join(th_audio, &ret_audio);

  pthread_create(&th_video, NULL, video_routine, (void *)player);
  pthread_setname_np(th_video, "video-player-runner");


  // audio_routine((void *)player);
  pthread_join(th_video, &ret_video);


  if ((int)ret_video == -1) {
    ERROR("player failed to play the video");
  }

  endwin();
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    ERROR("Read help to know how to use this tool");
    print_help();
    return -1;
  }

  int opt;
  const char *short_options = "g:p:h";
  struct option long_options[] = {{"generate", required_argument, 0, 'g'},
                                  {"play", required_argument, 0, 'p'},
                                  {"help", no_argument, 0, 'h'},
                                  {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1)
  {
    switch (opt)
    {
    case 'g':

      const char *out = argv[optind];
      if (out == NULL)
      {
        ERROR("Unknown destination, you should pass out directory.");
        exit(EXIT_FAILURE);
      }
      generate_ascii_video(optarg, out);
      optind++;

      break;
    case 'p':
      if (optarg == NULL)
      {
        ERROR("Unknown destination, you should pass out directory.");
        exit(EXIT_FAILURE);
      }
      play_ascii_video(optarg);
      break;
    case 'h':
      print_help();
      exit(EXIT_SUCCESS);
    default:
      ERROR("Unknown option.");
      print_help();
      exit(EXIT_FAILURE);
    }
  }

  if (optind < argc)
  {
    ERROR("Unexpected argument: %s", argv[optind]);
    print_help();
    exit(EXIT_FAILURE);
  }

  return 0;
}
