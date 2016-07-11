add_library(safe_cast INTERFACE)
target_link_libraries(safe_cast INTERFACE convert)
target_include_directories(safe_cast
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    INTERFACE $<INSTALL_INTERFACE:include/structure/detail>)

add_executable(safe_cast_unitTests test.cpp)
target_link_libraries(safe_cast_unitTests
                      PRIVATE safe_cast catch)
add_test(NAME safe_cast_unitTests
         COMMAND safe_cast_unitTests)

install(FILES safe_cast.hpp
        DESTINATION include/structure/detail
        COMPONENT dev)
