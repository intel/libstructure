add_library(structure SHARED
    functions.cpp
    Structure.cpp
    Block.cpp
    StructureValue.cpp
    Visitor.cpp
    VarArray.cpp
    ValueInitializer.cpp
    ListImporter.cpp
    DefaultImporter.cpp)

target_include_directories(structure
    PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(structure
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    INTERFACE $<INSTALL_INTERFACE:include/structure>)

target_link_libraries(structure safe_cast)

include(GenerateExportHeader)
generate_export_header(structure)

install(TARGETS structure EXPORT StructureTargets
    LIBRARY DESTINATION lib COMPONENT runtime
    RUNTIME DESTINATION lib COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT dev)

install(FILES
    include/Exception.hpp
    include/functions.hpp
    include/ValueImporter.hpp
    include/ValueInitializer.hpp
    include/Visitor.hpp
    DESTINATION "include/structure"
    COMPONENT dev)

install(DIRECTORY
    include/type
    include/value
    include/importer
    include/attributes
    include/detail
    DESTINATION "include/structure"
    COMPONENT dev
    FILES_MATCHING PATTERN "*.hpp")
