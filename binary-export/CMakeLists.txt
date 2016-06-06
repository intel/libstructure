add_library(binary-export INTERFACE)
target_include_directories(binary-export INTERFACE include)
target_link_libraries(binary-export INTERFACE structure)

add_executable(binary-exporter-poc main.cpp)
target_link_libraries(binary-exporter-poc binary-export)
