```
            _____  _____ _____ _____  
     /\    / ____|/ ____|_   _|_   _| 
    /  \  | (___ | |      | |   | |   
   / /\ \  \___ \| |      | |   | |   
  / ____ \ ____) | |____ _| |_ _| |_  
 /_/    \_\_____/_\_____|_____|_____| 
 \ \    / (_)   | |                   
  \ \  / / _  __| | ___  ___          
   \ \/ / | |/ _` |/ _ \/ _ \         
    \  /  | | (_| |  __/ (_) |        
     \/   |_|\__,_|\___|\___/         
                                      
```

ASCIIVideo is a simple tool that allows you to play videos in ASCII format directly within your terminal, providing a minimalist and efficient way to enjoy video content without relying on bulky software.



## Dependencies

Before , ensure that you have the following installed on your system:

- **FFmpeg:** A command-line tool, designed for processing video and audio files. 
- **aplay:** A command-line audio player.
- **CMake:** Used for building the project.

```
pkg-config libavcodec-dev libavdevice-devlibavfilter-dev libavformat-dev libavutil-dev libswresample-dev libswscale-dev libncurses5-dev
```


## Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/hulxv/asciivideo.git && cd asciivideo
   ```

2. **Build the Project**:

   ```bash
   cmake .
   cmake --build .
  
   sudo make install # for installation
   ```

## Usage

To play a video in ASCII format, run the following command:

```bash
asciivideo -g <video-src> <ascii-dir>  # Generate ASCII art from video and save to <out>.
asciivideo -p <ascii-dir>              # Play ASCII art from <src-dir>.
```


## License

This project is licensed under the GNU General Public License v2.0. See the [LICENSE](https://github.com/hulxv/asciivideo/blob/master/LICENSE) file for more details.
