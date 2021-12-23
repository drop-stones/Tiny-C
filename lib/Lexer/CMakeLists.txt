
# add_tiny_c_library(lexer Lexer.cpp Parser.cpp)
add_library(lexer STATIC Lexer.cpp)
target_link_libraries(lexer PRIVATE ${llvm_libs})