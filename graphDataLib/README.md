# graphData
This is the C project that provides basic graph manipulation for other processes.

# Project Structure
This is a CMake project that is intended to be able to either cross-compile or build native libraries on various platforms.  As
such, a lot of effort has been put into separating platform-specific requirements from basic C++ build and test requirements.

The C project's overall structure is as follows:
```
📦 graphDataLib
│
├──🔧 cmake
│   └──🔧 linux-x64.cmake Linux toolchain configuration
│   └──🔧 win-x64.cmake Windows toolchain configuration
│   └──🔧 packaging.cmake Platform-based installer package configurations
│
├──🔧 conf.in
│   └──🔧 linux-x64-runtime.nuspec.in Template for Linux NuGet runtime package
│   └──🔧 linux_runtime.csproj.in Template for Linux NuGet package build project
│   └──🔧 win-x64-runtime.nuspec.in Template for Windows NuGet runtime package
│   └──🔧 win_runtime.csproj.in Template for Windows NuGet package build project
│
├──⚒️  include
|  └── Build and install headers 
│
├──📂 runtimes
|  └── Windows and Linux x64 libraries for supporting runtime nuget packages
│
├──⚒️ src
|  └── Central wrapper code
│
├──📋 tests
|  └── CMake testing subproject with data, code, and configuration to run build-time tests
│
└──🏗️ buildLinux.sh
└──🏗️ BuildgraphData.ps1
└──📜 CMakeLists.txt
└──🗒️ README.md

```
* 📦 Project directory
* ⚒️  Source code and headers
* 🔧 settings and configuration
* 📋 Testing
* 🏗️ Build script
* 📜 CMake file
* 🗒️ Documentation (e.g. README)

# Build
Platform-specific options, settings, and code is typically named or located to indicate the target platform.  Wherever these
settings are mixed in, comments and buildtime/runtime checks are included.

## Build/Package/Install
The general setup and build files are described here.

* `🏗️ buildLinux.sh` This is the build script for Linux.  Simply run it with `./buildLinux.sh -h` to see all options.
* `🏗️ BuildgraphData.ps1` This is the cross-platform build script.  Simply run it with `./BuildgraphData.ps1 -help` to see all options.
* `🔧 linux-x64.cmake`  CMake build and install settings for Linux.  Make os-specific changes here, if possible
* `🔧 osx.cmake`  CMake build and install settings for OSX.  Make os-specific changes here, if possible
* `🔧 win-x64.cmake`  CMake build and install settings for Windows.  Make os-specific changes here, if possible
* `📂 build-linux-x64` Created by the build script.  When the built on a Linux machine, the intermediate products and settings (including all the relevant Makefile output)
  will be placed under this folder.
* `📂 build-osx` Created by the build script.  When the built on an OSX machine, the intermediate products and settings (including all the relevant Makefile output)
  will be placed under this folder.
* `📂 build-win-x64` Created by the build script.  When the built on a Windows machine, the intermediate products and settings (including all the relevant Makefile output)
  will be placed under this folder.
* `📂 output` Created by build script.  Final output products (NuGet package definitions, binaries, etc.) are placed here. 

# Testing
See the file [Test.md](tests/Testing.md) for specifics.

# Contribute
* Build this project on various platforms and make adjustments to the targeted build process
* Add build setup for Mac/OSX
* Add platform-specific installation and packaging (.deb, .rpm, .msi, etc.)
* Convert to a C++ testing platform