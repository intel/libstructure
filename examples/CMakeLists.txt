add_executable(default-value-example default-value.cpp)
target_link_libraries(default-value-example structure)

add_executable(binary-exporter-example binary-export.cpp)
target_link_libraries(binary-exporter-example binary-export)

add_executable(pfw-export-example
    parameter-framework/export.cpp
    parameter-framework/exportValue.cpp
    parameter-framework/main.cpp)
target_link_libraries(pfw-export-example structure)
