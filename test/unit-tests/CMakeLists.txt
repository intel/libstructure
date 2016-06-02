add_executable(libstructureUnitTest
               basic.cpp)

target_link_libraries(libstructureUnitTest
                      PRIVATE structure catch)

add_test(NAME libstructureUnitTest
         COMMAND libstructureUnitTest)


