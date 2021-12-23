cmake_minimum_required(VERSION 3.16)

project("Tiny-C"
  DESCRIPTION "LLVM Frontend for Tiny-C"
  LANGUAGES CXX
)

# enable clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)
message("Found LLV ${LLVM_PACKAGE_VERSION}, build type ${LLVM_BUILD_TYPE}")
list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})

add_definitions(${LLVM_DEFINITIONS})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs Core)

if (LLVM_COMPILER_IS_GCC_COMPATIBLE)
  if (NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  endif()
  if(NOT LLVM_ENABLE_EH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
  endif()
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory("lib")
add_subdirectory("tools")
add_subdirectory("unittest")

enable_testing()