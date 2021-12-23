
add_executable(LexerTest LexerTest.cpp)
target_link_libraries(LexerTest PRIVATE lexer gtest gtest_main)
add_test(NAME LexerTest COMMAND LexerTest)