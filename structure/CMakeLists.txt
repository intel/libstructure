add_library(structure SHARED
    functions.cpp
    Structure.cpp
    Block.cpp
    StructureValue.cpp
    FieldTraits.cpp)
target_include_directories(structure PUBLIC include PRIVATE .)
