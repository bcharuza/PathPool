#####################
# Google Benchmark  #
# path     	    #
#####################
add_perf_test(FNAME "path-insertion"
  DEPENDS
  pathpool::path
  pathpool::perftest::includes
  
  FILES "insertion.cpp"

  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  )

add_perf_test(FNAME "path-comparison"
  DEPENDS
  pathpool::path
  pathpool::perftest::includes
  
  FILES
  "comparison.cpp"

  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  )
