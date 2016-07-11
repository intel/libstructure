add_library(convert INTERFACE)
target_include_directories(convert
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    INTERFACE $<INSTALL_INTERFACE:include/structure/detail>)

install(FILES convert.hpp
        DESTINATION include/structure/detail
        COMPONENT dev)
