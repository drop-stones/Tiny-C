# Options
option(
  CODE_COVERAGE
  "Builds targets with code coverage instrumentation. (Requires GCC or clang)"
  OFF
)

# Programs
find_program(LLVM_COV_PATH llvm-cov)
find_program(LLVM_PROFDATA_PATH llvm-profdata)

function(add_code_coverage)
  if(CODE_COVERAGE)
    add_compile_options(-g -fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-g -fprofile-instr-generate -fcoverage-mapping)
  endif()
endfunction()