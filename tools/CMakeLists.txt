
add_tiny_c_executable(tiny-c Driver.cpp)

target_link_libraries(tiny-c
  PRIVATE
  lexer
  parser
  codegen
)