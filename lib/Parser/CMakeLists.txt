add_tiny_c_library(parser Parser.cpp)
target_link_libraries(parser PRIVATE lexer)