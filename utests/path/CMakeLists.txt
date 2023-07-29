################
# Google Tests #
# path	       #
################
add_google_test(
  FNAME "path"
  LABEL "interface-short"
  DEPENDS
  pathpool::path
  pathpool::utest::includes

  FILES
  "empty_short.cpp"
  "iterator_short.cpp"
  "populated_short.cpp"
  "common_path_short.cpp"

  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO

  FLAGS
  "-Wall"
  )
