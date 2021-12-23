
add_library(lexer STATIC Lexer.cpp)
target_link_libraries(lexer PRIVATE ${llvm_libs})