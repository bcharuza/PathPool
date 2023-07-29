################
# Google Tests #
# pathpool     #
################
add_google_test(
  FNAME "pathpool"
  LABEL "interface-short"
  DEPENDS
  pathpool::pool
  pathpool::utest::includes

  FILES
  "empty_short.cpp"
  "iterator_short.cpp"
  "get_taglist_short.cpp"
  "get_common_path_short.cpp"  

  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO

  FLAGS
  "-Wall"
  )

add_google_test(
  FNAME "pathpool"
  LABEL "interface-long"
  DEPENDS
  pathpool::pool
  pathpool::utest::includes
  
  FILES
  "vergecase_long.cpp"
  "crosscomp_long.cpp"

  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO

  FLAGS
  "-Wall"
  )
