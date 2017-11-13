# Graph Data Project

## Purpose

This project is for the purpose of supporting Graph Cut Navigation on small computing platforms, as outlined in the project at [Researchgate](https://www.researchgate.net/project/Implementation-of-Graph-Cut-Navigation-software-in-C).

## Goal

The goal of this software is to provide a shared library (libgraphdata.so for *nix systems, graphdata.dll for MS Windows, etc.) that will perform efficiently on small computing platforms, such as the Raspberry Pi.  This is the first building block for a full autonomous navigation system on small platforms.

## Code and Build

1. The language for this library is C, using the C99 standard.
2. The build is managed by CMake (currently using minimum of CMake v3.8).
3. The output is a shared library, to be used by future software components related to project.
4. The initial output is an RPM, containing the shared library and associated headers.

## IDE Requirements

As best as can be managed, any IDE-specific files, configurations, and settings will be kept out of the source code here.  The IDE that I am using is Jetbrains' CLion, which I enjoy very much, but I would rather go through the extra step of having to set up my IDE on every initial clone rather than forcing any and all developers to follow in lock-step with my choices.  (Having the freedom to change my mind later is nice, too.)

Ideally, with the correct compiler and enough knowedge, you should be able to modify and build this project with any editor tools on any compatible platform, all the way down to pure command line.  If there is any problems in this regard, please feel free to get in touch with me or submit a pull request to correct it.

## Contact Info

Name:  David C. Days
Email: david.c.days (at) gmail.com

