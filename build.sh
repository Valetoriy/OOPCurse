cmake -G Ninja -S . -B "./build" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build "./build"
mv build/CURSE .
