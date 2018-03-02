# Run from this folder
# Builds a C++ library and moves it to the extension library folder
rm AABB.o libaabb.a AABB.d

EMCC=/Users/selimanac/SDK/emsdk-portable/emscripten/1.35.0/em++
EMAR=/Users/selimanac/SDK/emsdk-portable/emscripten/1.35.0/emar
EMRA=/Users/selimanac/SDK/emsdk-portable/emscripten/1.35.0/emranlib

$EMCC -O3 -std=c++11 -DNDEBUG -Isrc AABB.cc -c -o AABB.o
$EMAR rcs libaabb.a AABB.o
$EMRA libaabb.a

mkdir -p ./bin/lib/js-web

cp -v libaabb.a ./bin/lib/web

