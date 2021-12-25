add_tiny_c_library(codegen CodeGen.cpp)
target_link_libraries(codegen PRIVATE ${llvm_libs})