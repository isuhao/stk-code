## Building from source

In order to build SuperTuxKart from source, you'll need both the code and the assets (See <https://supertuxkart.net/Source_control> for more information):

```bash
git clone https://github.com/supertuxkart/stk-code stk-code
svn co https://svn.code.sf.net/p/supertuxkart/code/stk-assets stk-assets
```

## Building SuperTuxKart on Linux

### Dependencies

To build SuperTuxKart from source, you'll need to install the following packages:

  * OpenGL (mesa)
  * OpenAL (recommended: openal-soft-devel)
  * Ogg (libogg-dev)
  * Vorbis (libvorbis-dev)
  * Freetype (libfreetype6-dev)
  * libcurl (libcurl-devel)
  * libbluetooth (bluez-devel)
  * libpng (libpng-devel)
  * zlib (zlib-devel)
  * jpeg (libjpeg-turbo-devel)

Ubuntu command:

```bash
sudo apt-get install build-essential cmake libbluetooth-dev \
libcurl4-openssl-dev libenet-dev libfreetype6-dev libfribidi-dev \
libgl1-mesa-dev libglew-dev libjpeg-dev libogg-dev libopenal-dev libpng-dev \
libssl-dev libvorbis-dev libxrandr-dev libx11-dev pkg-config zlib1g-dev
```
Fedora command:

```bash
sudo dnf install @development-tools cmake bluez-libs-devel \
openssl-devel libcurl-devel freetype-devel fribidi-devel mesa-libGL-devel \
libjpeg-turbo-devel libogg-devel openal-soft-devel libpng-devel \
libvorbis-devel libXrandr-devel libGLEW pkgconf zlib-devel
```
### In-game recorder

To build the in-game recorder for STK, you have to install
libopenglrecorder from your distribution, or compile it yourself from [here](https://github.com/Benau/libopenglrecorder).
Compilation instruction is explained there. If you don't need this feature, pass `-DBUILD_RECORDER=off` to cmake.

### Compiling

To compile SuperTuxKart, run the following commands inside `stk-code` directory:

```bash
mkdir cmake_build
cd cmake_build
cmake ..
make -j4
```
STK can then be run from the build directory with `bin/supertuxkart`

#### Keeping your build up to date

To recompile the latest code without redownloading the entire source, first run the ```svn up``` command inside the 'stk-assets' directory, then run the following commands inside the 'stk-code' directory:

```bash
git pull
cd cmake_build
cmake ..
make -j4
```

##### Build Speed Optimization

"-j4" is an example, for a faster build, use "-jx" instead, where "x" is the amount of CPU threads you have, minus one. "-j$(nproc)" usually works.

### Further options

To create a debug version of STK, run:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

You can install your build system-wide:

```bash
sudo make install
```

The default install location is `/usr/local`, i.e. the data files will
be written to `/usr/local/share/games/supertuxkart`, the executable
will be copied to `/usr/local/bin`. To change the default installation
location, specify `CMAKE_INSTALL_PREFIX` when running cmake, e.g.:
`cmake .. -DCMAKE_INSTALL_PREFIX=/opt/stk`



## Building SuperTuxKart on Windows
To Build SuperTuxKart on Windows, follow these instructions:

1. Download and install Visual Studio from here: [Visual Studio - Download](https://www.visualstudio.com/downloads/). The free Visual Studio Community edition works fine.
2. Download the SuperTuxKart source package from either [SuperTuxKart download area - SourceForge.net](https://sourceforge.net/projects/supertuxkart/files/SuperTuxKart/) or [SuperTuxKart.net - Source Control](https://supertuxkart.net/Source_control), and unpack it. 
*Note: If you downloaded the source package from here: [SuperTuxKart.net - Source Control](https://supertuxkart.net/Source_control), then both `stk-code` and `stk-assets` **must** be in the same directory, otherwise the build can result in failure*
3. Download the Windows dependencies package from either [SuperTuxKart download area: Dependencies - SourceForge.net](https://sourceforge.net/projects/supertuxkart/files/SuperTuxKart%20Dependencies/Windows/)
or [SuperTuxKart on GitHub - Dependencies](https://github.com/supertuxkart/dependencies), and unpack it; then, copy the `dependencies` directory from either the `windows` or the `windows_64bit` directories into the `stk-code` directory, rename it to `dependencies-64bit` if you want to compile a 64bit build.
4. Download CMake from here: [CMake - download page](https://cmake.org/download/), install it; once CMake is installed, double click on the CMake icon on your desktop, and point it towards your `stk-code` directory in the 'Where is the source code' field, and point it to a directory called `build` or `bld` inside the stk-code directory.
5. Press 'Configure'; CMake will ask you if it is OK to create the aforementioned directory, press `Yes`. CMake will then ask you about your version of Visual Studio. 
Confirm your selection; *Please look at the table below to avoid confusion between version numbers and releases of Visual Studio*;
CMake will begin creating the required files for the build in the directory.
6. Navigate to your build directory and open the `SuperTuxKart.sln` file; Visual Studio will now load the solution. 
7. In the 'Solution Explorer', right click on the `supertuxkart` project and select "Set as StartUp project"
8. Open the 'Build' menu and select 'Build Solution'; or, press the default keyboard shortcut: `CTRL + SHIFT + B` to build the solution.

*Note: To avoid confusion between releases and versions, refer to this table:*

Visual Studio Release | Version
----------------------|------------
Visual Studio 2017| 15
Visual Studio 2015| 14
Visual Studio 2013| 13

## Building SuperTuxKart on Windows (from PowerShell/Command line)
1. Download and install Visual Studio from here: [Visual Studio - Download](https://www.visualstudio.com/downloads/), the free Visual Studio Community edition works fine.

2. Download a source package from either [SuperTuxKart 0.9.2 download area - SourceForge.net](https://sourceforge.net/projects/supertuxkart/files/SuperTuxKart/0.9.2) or [SuperTuxKart.net - Source Control](https://supertuxkart.net/Source_control)
NOTE: the `stk-code` and `stk-assets` directories **must** be in the same directory 
3. Download the Windows dependencies package from either [SuperTuxKart download area - SourceForge.net](https://sourceforge.net/projects/supertuxkart/files/SuperTuxKart%20Dependencies/Windows/)
or [SuperTuxKart on GitHub - Dependencies](https://github.com/supertuxkart/dependencies)
and unpack the archive; once unpacked, copy the `dependencies` directory from either the `windows` or the `windows_64bit` directories into the `stk-code` directory
4. Download CMake from here: [CMake - download page](https://cmake.org/download/); and install it. Navigate to the `stk-code` directory; and create an directory called "build":
```cmd
mkdir build
cd build
```
5. Once inside the build directory; run CMake to start the compilation process:
```cmd
cmake ..
```
6. Now that CMake finished configuring and creating the necessary files for the build, run the build command in the same directory:
```cmd
msbuild.exe SuperTuxKart.sln 
``` 
SuperTuxKart can now be run as `bin\Debug\supertuxkart.exe` or `bin\Release\supertuxkart.exe` 

## Building SuperTuxKart on macOS

### Getting Started

Install the developer tools, either from the OS X Install DVD or from Apple's website.

If you have never built anything before, you have create `/usr/local/include/` first:

```bash
sudo mkdir -p /usr/local/include/
```

Symlink the `include`-folder of OpenGL framework to `/usr/local/include/GL` (Unix programs have an easier time finding it this way):

```bash
sudo ln -s /System/Library/Frameworks/OpenGL.framework/Versions/A/Headers/ /usr/local/include/GL
```

On OS X 10.9.5, you might need the following workaround:

```bash
sudo ln -s `xcrun --show-sdk-path`/usr/include/ /usr/include
sudo ln -s `xcrun --show-sdk-path`/System/Library/Frameworks/OpenGL.framework/Headers/ /usr/local/include/OpenGL
```
The first link is required in order to find libcurl, the second to find opengl.

### CMake

CMake is used to build STK. At this time CMake will not make a binary that is ready for distribution.

You'll have to run these commands inside your stk-code directory.


### STK 0.9.4 or later (or latest git)

Install homebrew ( https://brew.sh/)
Install all of the dependencies using homebrew :

```bash
brew install libogg
brew install libvorbis
brew install openal-soft
brew install freetype
brew install curl
brew install openssl@1.1
brew install fribidi
brew install glew
```

Build STK
```bash
mkdir cmake_build
cd cmake_build
CMAKE_PREFIX_PATH=/usr/local/opt/freetype/:/usr/local/opt/curl/:/usr/local/opt/libogg/:/usr/local/opt/libogg/:/usr/local/opt/libvorbis/:/usr/local/opt/openssl\@1.1/:/usr/local/opt/glew/:/usr/local/opt/fribidi/ /usr/local/opt/cmake/bin/cmake .. -DFREETYPE_INCLUDE_DIRS=/usr/local/opt/freetype/include/freetype2/ -DUSE_SYSTEM_GLEW=1 -DOPENAL_INCLUDE_DIR=/usr/local/opt/openal-soft/include/ -DOPENAL_LIBRARY=/usr/local/opt/openal-soft/lib/libopenal.dylib -DFREETYPE_LIBRARY=/usr/local/opt/freetype/lib/libfreetype.dylib
make
```

#### (Optional) packaging for distribution

By default, the executable that is produced is not ready for distribution. Install https://github.com/auriamg/macdylibbundler and run:

```bash
dylibbundler -od -b -x ./bin/SuperTuxKart.app/Contents/MacOS/supertuxkart -d ./bin/SuperTuxKart.app/Contents/libs/ -p @executable_path/../libs/
```

Afterwards, copy the contents of `stk-assets` into `/SuperTuxKart.app/Contents/Resources/data`.

### STK 0.9.3 or earlier

Download pre-built dependencies from [here](https://sourceforge.net/projects/supertuxkart/files/SuperTuxKart%20Dependencies/OSX/) and put the frameworks in [hard disk root]/Library/Frameworks

Building with clang:

```bash
mkdir cmake_build
cd cmake_build
cmake ..
make
```

Building with GCC:
```bash
mkdir cmake_build
cd cmake_build
cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_C_COMPILER=/usr/bin/gcc
make
```

Building on 10.10 with 10.9 compatibility:
```bash
cmake .. -DCMAKE_OSX_SYSROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9
```

#### Xcode

Place an additional copy of the dependencies into `Users/<YOUR_USERNAME>/Library/Frameworks`.
Then cd to your cloned stk-code directory and execute the following commands:

```bash
mkdir xcode_build && cd xcode_build
cmake .. -GXcode
```

Use Finder to navigate to your stk-code/xcode_build folder and open the newly generated Xcode project (`SuperTuxKart.xcodeproj`).

You can then build the project in Xcode using Product -> Build

Note: Xcode is much less well tested than makefiles, so there may be issues when trying to use Xcode.
