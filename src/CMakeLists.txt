############
# PathPool #
############
set( name pool )
message( "Target: pathpool::${name}")
add_library( ${name} INTERFACE )

target_sources( ${name}
  INTERFACE
  "impl/custom_iterators.h"
  "path_pool.h"
  "impl/impl_list_path_pool.h"
  "impl/impl_hash_path_pool.h"
  "impl/impl_path_pool_utils.h" 
  )

target_compile_features( ${name}
  INTERFACE
  cxx_std_14
  )

target_include_directories( ${name}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_library( pathpool::pool ALIAS ${name} )
message( "Target: pathpool::${name} [done]")

########
# Path #
########
set( name path )
message( "Target: pathpool::${name}")
add_library( ${name} INTERFACE )

target_sources( ${name}
  INTERFACE
  "path.h"
  "impl/impl_path_utils.h"
  "impl/impl_path.h"
  )

target_compile_features( ${name}
  INTERFACE
  cxx_std_14
  )

target_include_directories( ${name}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

target_link_libraries( ${name}
  INTERFACE
  pathpool::pool
  )

add_library( pathpool::path ALIAS ${name} )
message( "Target: pathpool::${name} [done]")
