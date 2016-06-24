add_library(safe_cast INTERFACE)
target_include_directories(safe_cast INTERFACE ${PROJECT_SOURCE_DIR}/convert ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(safe_cast_unitTests test.cpp)
target_link_libraries(safe_cast_unitTests
                      PRIVATE safe_cast catch)
add_test(NAME safe_cast_unitTests
         COMMAND safe_cast_unitTests)
