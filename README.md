# cync
Syncing files accross directories and devices.

> **NOTE:** WIP &rarr; Only `--schema=0` option works for now!

<img src="imgs/screenshot.jpg" width="720">

## Usage
```
cync v1.0.1 -- syncing files accross directories and devices.
-t1 --target1 path to target folder 1
-t2 --target2 path to target folder 2
 -v --verbose verbose output
 -m --move_df move dot files
 -s  --schema syncronization schema
 -h    --help This help information.
OPTIONS:
        --schema=0  master-slave syncronization (default)
        --schema=1  sync both targets, but don't delete items
        --schema=2  sync both targets completely
EXAMPLE:
        cync --target1 ~/disk1 --target2 ~/disk2 --schema 0 --verbose
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

