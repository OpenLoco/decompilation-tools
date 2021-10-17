cmake_minimum_required(VERSION 3.15)

project(LocoParser)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(Filesystem REQUIRED)

find_package(Clang REQUIRED)
add_library(Clang INTERFACE)
target_link_libraries(Clang INTERFACE libclang)
target_include_directories(Clang INTERFACE "${CLANG_INCLUDE_DIRS}")
target_compile_definitions(Clang INTERFACE ${LLVM_DEFINITIONS})

add_executable(locoparser main.cpp)
target_compile_features(locoparser PRIVATE cxx_std_20)
target_link_libraries(locoparser PRIVATE Clang)
target_link_libraries(locoparser PRIVATE std::filesystem)