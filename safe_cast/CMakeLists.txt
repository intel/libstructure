configure_file(safe_cast.hpp ${CMAKE_BINARY_DIR}/structure/detail/safe_cast.hpp COPYONLY)

add_executable(safe_cast_unitTests test.cpp)
target_include_directories(safe_cast_unitTests PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(safe_cast_unitTests
                      PRIVATE catch)
add_test(NAME safe_cast_unitTests
         COMMAND safe_cast_unitTests)

install(FILES safe_cast.hpp
        DESTINATION include/structure/detail
        COMPONENT dev)
