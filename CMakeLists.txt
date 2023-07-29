# Requirements:
# Git package
# GTest package

cmake_minimum_required( VERSION 3.13.4 FATAL_ERROR )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/bilke/cmake-modules")
option(version "Project version")
option(perftest "build perftest")
option(use-gcovr "use gcovr instead of lcov")

include(${CMAKE_CURRENT_LIST_DIR}/cmake_scripts/Version.cmake)

if(NOT version)
  git_tag_desc(version)
else()
  message(WARNING "Project version defined manually."
  "To return to automatic version detection reset CMake cache.\n"
  "If using out-of-source build remove build directory with 'rm -r ./build/'\n"
  "If using git version control system remove all untracked files with 'git clean -d -f -x -n'")
endif()

parse_version(version ${version})
if(NOT version_FOUND)
  message(SEND_ERROR "No project version could be obtained reading git signed tags.")
  message(FATAL_ERROR "Set project version manually by: cmake -D version=v1.2.3-description")
endif()

message("Project version is: ${version_major}.${version_minor}.${version_patch}")
message("Version description: ${version_desc}")
project( PathPool VERSION ${version_major}.${version_minor}.${version_patch} LANGUAGES CXX ) # HOMEPAGE, DESCRIPTION 

include(${CMAKE_CURRENT_LIST_DIR}/cmake_scripts/Coverage.cmake)

message("----------Enter Subdirectories----------")

add_subdirectory(src)

##############
# Unit Tests #
##############
message("----------Build unit tests----------")
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(utests)

message("--------------Examples--------------")
add_subdirectory(examples)

if(perftest)
  message("----------Build performace tests----------")
  set(BENCHMARK_ENABLE_TESTING false)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/google/benchmark)
  add_subdirectory(perftest)
endif()
