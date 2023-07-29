#####################
# Google Benchmark  #
# pathpool     	    #
#####################
add_perf_test(
  FNAME "pathpool"
  DEPENDS
  pathpool::pool
  
  FILES
  "random_insertion.cpp"

  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  )
