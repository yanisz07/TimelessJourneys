# TimelessJourneys
Timeless Journeys is a single-player top-down view hack-and-slash PC video game.

## External dependencies
The project is supported for Windows and macOS machines. It utilizes the SDL suite of cross-platform development libraries to manage user inputs and image rendering.
Installation guides are provided below. 

On Windows,
1) Create a new folder called SDL2_sources in the C drive directory(C:\)
2) Navigate to the SDL2 releases GitHub page (https://github.com/libsdl-org/SDL/releases) and download the latest stable release of SDL2 for the MinGW compiler system.
Example: SDL2-devel-2.28.5-mingw.zip
3) Repeat the previous step for the SDL2_image, SDL_tff and SDL_mixer libraries.
4) Extract the contents of the ZIP files to the SDL2_sources folder. It should end up having folders called SDL2_image-2.6.3 and SDL2_2.28.5, for example.

On macOS,
To install the SDL libraries we recommend using the Homebrew software package management system.
To install Homebrew,
1) Copy and paste the following command into the Terminal and press Enter:
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
2) Verify the installation using the following command: brew doctor
3) Update Homebrew using this command: brew update

Ensure that you have XCode Command Line Tools installed. You can install them using this command: xcode-select --install
In case of path-related errors, run the following command:
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> /Users/$(whoami)/.zshrc eval "$(/opt/homebrew/bin/brew shellenv)"
To install SDL2,
1) Run the following commands to install the corresponding libraries:
brew install sdl2
brew install sdl2_image
brew install sdl2_ttf
brew install sdl2_mixer
2) You can check the correct installation with the following commands:
brew info sdl2
brew info sdl2_image
brew info sdl2_ttf
brew info sdl2_mixer

## Compilation
To run the project, open the Cmake file in the Qt Creator IDE. Configure the project to work with your preferred compiler in 64-bit systems, in Release mode. 
Once configured, press the run button to build, compile and execute the application.


 
