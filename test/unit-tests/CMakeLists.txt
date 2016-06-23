add_executable(libstructureUnitTest
               basic.cpp
               export.cpp
               import.cpp)

target_link_libraries(libstructureUnitTest
                      PRIVATE structure catch)

add_test(NAME libstructureUnitTest
         COMMAND libstructureUnitTest)

add_executable(binaryUnitTest binary.cpp)
target_link_libraries(binaryUnitTest PRIVATE binary-export catch)
add_test(NAME binary COMMAND binaryUnitTest)
