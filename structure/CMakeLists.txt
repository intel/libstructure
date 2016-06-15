add_library(structure SHARED
    functions.cpp
    Structure.cpp
    Block.cpp
    StructureValue.cpp
    Visitor.cpp
    VarArray.cpp
    LiteralTreeImporter.cpp)

target_include_directories(structure
    PUBLIC include ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/convert)

include(GenerateExportHeader)
generate_export_header(structure)

