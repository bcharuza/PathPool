##################
# utest/includes #
##################
set(name utest-includes)
message("Target: ${name}")
add_library( ${name} INTERFACE )

target_sources( ${name}
  INTERFACE
  "./utest_tuple.h"
  "./utest_utils.h"
  "./utest_pretty.h"
  "./utest_inclusive_range.h"
  "./utest_traits.h"
  "./utest.h"
  )

target_compile_features( ${name}
  INTERFACE
  cxx_std_14
  )

target_compile_options( ${name}
  INTERFACE
  "-Wall"
  )

target_include_directories( ${name}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_library( pathpool::utest::includes ALIAS ${name} )
message("Target: ${name} [done]")
