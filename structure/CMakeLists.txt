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
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    INTERFACE $<INSTALL_INTERFACE:include>)
if (WIN32)
    # Force include iso646.h to support alternative operator form (and, or, not...).
    # Put it to *interface* compile options only because it already has been
    # added globally for the whole project.
    target_compile_options(structure INTERFACE /FIiso646.h)
endif ()

include(GenerateExportHeader)
generate_export_header(structure)

install(TARGETS structure EXPORT LibStructureTargets
    LIBRARY DESTINATION lib COMPONENT runtime
    RUNTIME DESTINATION lib COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT dev)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/structure_export.h
    DESTINATION "include/structure"
    COMPONENT dev)

install(DIRECTORY
    include
    DESTINATION "."
    COMPONENT dev
    FILES_MATCHING PATTERN "*.hpp")
