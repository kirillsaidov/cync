<img src="imgs/logo.png" width="64" height="64" align="left"></img>
# cync
Syncing files accross directories and devices.

<img src="imgs/screenshot.jpg" width="720">

## Usage
```
cync v1.0.3 -- syncing files accross directories and devices.
-s       --src source directory
-d       --dst destination directory
-v   --verbose verbose output
-i --ignore_df ignore dot files
-l   --low_mem use less memory
-m      --mode syncronization mode
-h      --help This help information.
OPTIONS:
        --mode=0  ensure the destination folder is a strict copy of the source specified (default)
        --mode=1  synchronize both targets, but do not remove files automatically
        --mode=2  synchronize both targets completely
        --mode=3  synchronize both targets over the network in --mode=2
EXAMPLE:
        cync --src ~/disk1 --dst ~/disk2 --mode 0 --verbose
```

## Installation
### Requirements
* C compiler
* CMake

### Install from source
#### Clone
```sh
git clone --recurse-submodules https://github.com/kirillsaidov/cync.git
cd cync/
```

#### Configure
Modify the build type in `third_party/vita/CMakeLists.txt`:
```sh
# COMMENT OUT
# set(DEFAULT_BUILD_TYPE "Debug")
# set(CMAKE_C_FLAGS "-Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")

# UNCOMMENT
set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "-DNDEBUG -Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")
```

#### Build
```sh
./build.sh  # linux, macos
./build.bat # windows
```
You will find the executable inside the `bin/` folder. 

## LICENSE
All code is licensed under the MIT license.

