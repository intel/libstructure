add_library(structure SHARED
    functions.cpp
    Structure.cpp
    Block.cpp
    StructureValue.cpp
    Field.cpp
    Visitor.cpp)

target_include_directories(structure PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})

include(GenerateExportHeader)
generate_export_header(structure)

