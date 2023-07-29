#####################
# perftest/includes #
#####################
find_package(Boost REQUIRED filesystem)

set(name perftest-includes)

message("Target: ${name}")
add_library( ${name} INTERFACE )

target_link_libraries( ${name}
  INTERFACE
  Boost::filesystem
  )

target_include_directories( ${name}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

target_sources( ${name}
  INTERFACE
  "BoostPath.h"
  )

message("Target: ${name} [done]")
add_library( pathpool::perftest::includes ALIAS ${name} )
