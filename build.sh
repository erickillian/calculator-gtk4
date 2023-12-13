#!/bin/bash

# Specify GCC version
export CC=gcc-11
export CXX=g++-11

CPPFLAGS="-I/opt/gtk/include"
LDFLAGS="-L/opt/gtk/lib"
PKG_CONFIG_PATH="/opt/gtk/lib/pkgconfig"
export CPPFLAGS LDFLAGS PKG_CONFIG_PATH

LD_LIBRARY_PATH="/opt/gtk/lib"
PATH="/opt/gtk/bin:$PATH"
export LD_LIBRARY_PATH PATH

# Create a build directory and enter it
rm -rf build
mkdir -p build
cd build

# Configure the project with CMake
cmake ..

# Build the project
make

rm calculator -f
cd ..
mv ./build/src/calculator .
