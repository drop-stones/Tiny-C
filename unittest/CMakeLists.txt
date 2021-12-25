
add_executable(LexerTest LexerTest.cpp)
target_link_libraries(LexerTest PRIVATE lexer gtest gtest_main)

add_executable(ParserTest ParserTest.cpp)
target_link_libraries(ParserTest PRIVATE parser gtest gtest_main)