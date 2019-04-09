set(CO_CMAKE_MIN_VERSION 3.5)
cmake_minimum_required(VERSION ${CO_CMAKE_MIN_VERSION})

project(ScatterMemoryIODevice)

set(SOURCE_EXE main.cpp ScatterMemoryIODevice.cpp)

set (CMAKE_CXX_STANDARD 11)

include_directories( ~/COExternals/3rdParty/Install/boost-1.65.1-7@4b4e81d1-debug-x86_64-static-Clang-10.0.1-Makefiles-bb44c072/include)

add_executable(main ${SOURCE_EXE})

target_link_libraries(main ~/COExternals/3rdParty/Install/boost-1.65.1-7@4b4e81d1-debug-x86_64-static-Clang-10.0.1-Makefiles-bb44c072/lib/libboost_iostreams.a)
