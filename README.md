# ProceduralMapGenGame
## Final Project for GAP 391 Procedural Content Generation.

I really enjoyed this project, the goal was to create a game using 3 different types of procedural content. I had wanted to attempt to make a cool map for a few semesters now, so I really dove into the tuning of how the maps looked. I found a [blog](http://androidarts.com/skeletonwars/) online that had a hand-drawn pixel art map that I wanted to try to replicate. [You can download the release build here.](https://github.com/GroverErin/ProceduralMapGenGame/releases/download/v1.0.0/NoiseMap_x64_Release.zip)

### This project uses previous iteration of the Exelius Engine and is not similar to, or a representation of, the in-progress version of [Exelius](https://github.com/GroverErin/ExeliusEngine).

Hand-Drawn Image (Arne Niklas Jansson, 2006-2010):
<p align="center">
  <img src="https://github.com/GroverErin/ProceduralMapGenGame/blob/main/Screenshots/SkeletonLords.gif?raw=true" alt="Hand-Drawn Map"/>
</p>

Here is the final result of this generator:
<p align="center">
  <img src="https://github.com/GroverErin/ProceduralMapGenGame/blob/main/Screenshots/NoiseMap.png?raw=true" alt="Generated Map"/>
</p>

## PCG
The three procedurally generated peices of content are:
   - Perlin & Squirrel Noise generated map using 3 layers.
      - The first layer of noise determines the map terrain pattern.
      - The second layer uses noise to determine the biome.
      - The final layer of noise is used to determine the tempurature.
   - Perlin & Squirrel Noise generated clouds in 2 layers.
      - The first layer of noise determines the cloud shape.
      - The second layer of noise determines the alpha fall-off.
   - Cellular Automata generates and speads fire.
      - Fire spreads at a different rate to each color of tile. (Faster in dryer biomes, slower in humid biomes)

## Gameplay
The gameplay consists of a player controlled plane that can collect water from the ocean and dump it over fire. The GIF below was set to a low framerate for size, but the game runs at a reasonable pace (It should probably run at 60+ FPS on most machines *fingers crossed*).

<p align="center">
  <img src="https://github.com/GroverErin/ProceduralMapGenGame/blob/main/Screenshots/FinalProjectShorter.gif?raw=true" alt="Gameplay"/>
</p>

Some Instructions are shown in the console, as well as the win/lose state and other information.

   - `Arrow Keys` to move.
   - `Space` over water to fill up the planes tank.
   - `Space` over land to attempt to extinguish the fire.
   - `Q` will quit the game.
   - `Esc` will regenerate the map.
   - You win the game by extinguishing all the fire before losing %80 of the land.

## Installation
### Windows
#### Requirements:
  - [Visual Studio 2019](https://visualstudio.microsoft.com/vs/)
  - [Git for Windows (Optional)](https://gitforwindows.org)
#### Installation and Building:
  - Clone this repository into an empty directory using this command:
    - `git clone https://github.com/GroverErin/ProceduralMapGenGame`
    - Alternatively, you can just download the source by selecting the Code dropdown and the Download ZIP option.
<p align="center">
  <img src="https://github.com/GroverErin/ProceduralMapGenGame/blob/main/Screenshots/DownloadRepo.png?raw=true" alt="Alternate Download Method"/>
</p>

  - Open the `ExeliusEngine.sln` in the root directory with Visual Studio 2019.
  - Build desired configuration
  - Run the build in the Visual Studio IDE.
    - Is it recommended to run the engine from the Visual Studio IDE, as running the executable requires copying some assets. To Be Patched.

## Known Bugs
   - Sometimes the player ship will dissappear, causing a crash when space is pressed. This can be avoided by pressing `Esc` to regenerate the map when the ship disappears. 
