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
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    INTERFACE $<INSTALL_INTERFACE:include>)
if (WIN32)
    # Force include iso646.h to support alternative operator form (and, or, not...).
    # Put it to *interface* compile options only because it already has been
    # added globally for the whole project.
    target_compile_options(structure INTERFACE /FIiso646.h)
endif ()

set(libstructure_export_header ${libstructure_generated_includes_dir}/structure_export.h)
include(GenerateExportHeader)
generate_export_header(structure EXPORT_FILE_NAME ${libstructure_export_header})

install(TARGETS structure EXPORT LibStructureTargets
    LIBRARY DESTINATION lib COMPONENT runtime
    RUNTIME DESTINATION lib COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT dev)

install(FILES
    ${libstructure_export_header}
    DESTINATION "include/structure"
    COMPONENT dev)

install(DIRECTORY
    include
    DESTINATION "."
    COMPONENT dev
    FILES_MATCHING PATTERN "*.hpp")

unset(libstructure_export_header)
