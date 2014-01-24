# Installation

## Linux

**TODO**

## MacOS X

1. Open `MarkEmptyDirs.dmg` and drag `MarkEmptyDirs` application icon to desired location.
2. Add the directory `<path-to-app>/MarkEmptyDirs.app/Contents/MacOS/` to your shell's search path.

## Windows

1. Unpack the downloaded archive at desired location.
2. Add the directory containing executable `MarkEmptyDirs.exe` to your search path.

# Building MarkEmptyDirs

The following sections describe requirements and steps necessary to build everything on a supported platform.

Note that older compiler versions might also work.

## Linux

### Prerequisites

1. Qt 5.2 (or newer)
2. GCC 4.8 or clang 3.3 compiler (or newer).
3. `QTDIR` environment variable must be set to Qt's root directory path.

### Compiling / Building

1. Open command line shell.
2. Change to project's root directory.
3. Execute shell script: `build_linux.sh`.

## MacOS X

### Prerequisites

1. Qt 5.2 (or newer)
2. GCC 4.8 or clang 3.3 compiler (or newer).
3. `QTDIR` environment variable must be set to Qt's root directory path.

### Compiling / Building

1. Open command line shell.
2. Change to project's root directory.
3. Execute shell script: `build_osx.sh`.

## Windows

### Prerequisites

1. Qt 5.2 (or newer)
2. Visual Studio 2012 compiler (or newer). Expression Edition sufficient.
3. 7-Zip Installation. Download from here: http://www.7-zip.org
4. `QTDIR` environment variable must be set to Qt's root directory path.

### Compiling / Building

1. Open command line shell.
2. Change to project's root directory.
3. Execute shell script: `build_win32.bat`.
