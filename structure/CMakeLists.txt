add_library(structure SHARED
    functions.cpp
    Structure.cpp
    Block.cpp
    StructureValue.cpp
    Field.cpp
    Visitor.cpp)
target_include_directories(structure PUBLIC include PRIVATE .)
