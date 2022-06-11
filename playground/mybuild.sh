emsdk-env emcc -std=c++17 -O3 --bind -o native.js native.cpp -s ALLOW_MEMORY_GROWTH=1 -Wno-writable-strings
#g++ -std=c++17 -O3 -o native-cmd native-cmd.cpp  -Wno-write-strings
