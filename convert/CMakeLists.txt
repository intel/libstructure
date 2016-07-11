configure_file(convert.hpp ${CMAKE_BINARY_DIR}/structure/detail/convert.hpp COPYONLY)

install(FILES convert.hpp
        DESTINATION include/structure/detail
        COMPONENT dev)
