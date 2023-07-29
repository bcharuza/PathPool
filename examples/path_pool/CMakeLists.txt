set(name pathpool-ex1)

message("Target: ${name}")

add_executable( ${name}
  ex1.cpp)

target_link_libraries( ${name}
  PRIVATE
  pathpool::pool)

message("Target: ${name} [done]")
add_executable( pathpool::examples::${name} ALIAS ${name})
