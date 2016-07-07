add_library(binary-export INTERFACE)
target_include_directories(binary-export INTERFACE include)
target_link_libraries(binary-export INTERFACE structure)
