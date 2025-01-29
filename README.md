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

## Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/hulxv/asciivideo.git && cd asciivideo
   ```

2. **Install required libraries**
   ```
   pkg-config libavcodec-dev libavdevice-devlibavfilter-dev libavformat-dev libavutil-dev libswresample-dev libswscale-dev libncurses5-dev
   ```

3. **Build the Project**:

   ```bash
   cmake .
   cmake --build .
   ```

4. **Install the Executable**:

   ```bash
   sudo make install
   ```

## Usage

To play a video in ASCII format, run the following command:

```bash
asciivideo -g <video-src> <ascii-dir>  # Generate ASCII art from video and save to <out>.
asciivideo -p <ascii-dir>              # Play ASCII art from <src-dir>.
```

Replace `/path/to/your/video.mp4` with the actual path to the video file you wish to play.

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a new branch:

   ```bash
   git checkout -b feature-name
   ```

3. Make your changes and commit them:

   ```bash
   git commit -m 'Add feature'
   ```

4. Push to the branch:

   ```bash
   git push origin feature-name
   ```

5. Open a pull request detailing your changes.

## License

This project is licensed under the GNU General Public License v2.0. See the [LICENSE](https://github.com/hulxv/asciivideo/blob/master/LICENSE) file for more details.
