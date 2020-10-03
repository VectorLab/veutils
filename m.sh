#!/bin/env bash

set -e

if [ "" == "${CC}" ]; then
CC=gcc
fi

if [ "" == "${CXX}" ]; then
CXX=g++
fi

rm -rf ./build

mkdir -p ./build/usr/lib
mkdir -p ./build/usr/bin

echo "build vert..."
cp -r ./package/vert/include ./build/usr/include
${CXX} ./package/vert/src/vert/FolderRecurser.cpp ./package/vert/src/vert/TargetBuilder.cpp -o ./build/usr/lib/libvert.so -I./build/usr/include -fvisibility=default -fPIC -s -O3 -std=c++17 -shared -Wl,-rpath='$ORIGIN'

echo "build mb..."
cp -r ./package/mb/include ./build/usr/include
${CXX} ./package/mb/src/main.cpp -o ./build/usr/bin/mb -L./build/usr/lib -lvert -I./build/usr/include -fPIC -s -O3 -std=c++17 -Wl,-rpath='$ORIGIN/../lib'

echo "done."












