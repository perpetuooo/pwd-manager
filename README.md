# Password Manager

## Compiling
Windows:
```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```
Linux \ macOS:
```
sudo apt install cmake pkg-config libsodium-dev -y
cmake -B build -S .
cmake --build build
```