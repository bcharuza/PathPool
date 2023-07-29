###################
# Add Google Test #
###################
function(add_google_test)
  cmake_parse_arguments(add_google_test "" "FNAME;LABEL;TIMEOUT" "DEPENDS;FILES;FEATURES,FLAGS;PROPERTIES;DEFINITIONS" ${ARGN})
  set(fname ${add_google_test_FNAME})
  set(label ${add_google_test_LABEL})
  set(timeout ${add_google_test_TIMEOUT})
  if(NOT ${timeout})
    set(timeout 5)
  endif()
  set(files ${add_google_test_FILES})
  set(depends ${add_google_test_DEPENDS})
  set(features ${add_google_test_FEATURES})
  set(flags ${add_google_test_FLAGS})
  set(properties ${add_google_test_PROPERTIES})
  set(definitions ${add_google_test_DEFINITIONS})

  set(name utest-${label}-${fname})
  message("Target: ${name}")
  add_executable( ${name} )
  target_sources( ${name} PRIVATE ${files}  )
  target_compile_features( ${name} PRIVATE ${features} )
  target_compile_options( ${name} PRIVATE ${compile_opt} )
  set_target_properties( ${name} PROPERTIES ${properties} )
  target_compile_definitions( ${name} PRIVATE ${definitions} )
  target_include_directories( ${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries( ${name} PRIVATE ${depends} GTest::GTest GTest::Main)
  gtest_discover_tests( ${name}
    TEST_PREFIX "${name}_"
    PROPERTIES LABELS "${label}"
    DISCOVERY_TIMEOUT ${timeout} )
  message("Target: ${name} [done]")
endfunction()

add_subdirectory(includes)
add_subdirectory(pathpool)
add_subdirectory(path)

setup_target_for_coverage(NAME coverage EXECUTABLE ctest -j ${PROCESSOR_COUNT}  EXCLUDE "utests/*" "/usr/*" "external/*" "perftest/*" "examples/*")
