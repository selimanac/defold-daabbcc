# Run from this folder
# Builds a C++ library and moves it to the extension library folder
rm AABB.o libaabb.a AABB.d

g++ -O3 -std=c++11 -DNDEBUG -Isrc -c -o AABB.o AABB.cc
g++ -MM -std=c++11 -DNDEBUG -Isrc AABB.cc > AABB.d

ar rcs libaabb.a AABB.o
ranlib libaabb.a

mkdir -p ./bin/lib/osx

cp -v  ./libaabb.a ./bin/lib/osx
#cp -v  ./libaabb.a ./bin/lib/linux

