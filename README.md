# IBKR_CWMR_STRATEGY

This project implements a Confidence Weighted Mean Reversion (CWMR) trading strategy 
using the Interactive Brokers (IBKR) API. The strategy is designed to analyze market data 
and execute trades based on predefined criteria. 

## Compiling and Execution

### Prequisites
- C++23 compatible compiler (e.g., GCC, Clang, MSVC)
- CMake 3.25 or later
- vcpkg package manager (for dependency management)
- Protobuf (5.29.5) compiler (protoc) for generating C++ classes from .proto files
- Intelrdfpmathlib library for advanced mathematical computations
- Threads support for concurrent execution
- ws2_32 library for Windows socket programming

### Using vcpkg for Dependency Management

This project utilizes Microsoft's `vcpkg` package manager in **Manifest Mode** for C++ 
libraries. 

#### Automated Dependency Management with Visual Studio 2022

- Building the code in Visual Studio 2022 and later versions is straightforward. The IDE 
automatically detects the `vcpkg.json` file and installs the required dependencies.

#### Manual Dependency Management with vcpkg

If you prefer to manage dependencies manually, you can use `vcpkg` commands to install 
the required libraries.

#### Generate the build configuration files using CMake
```bash
cmake -S . -B build \
	-DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config x64-release
```