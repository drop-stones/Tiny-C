macro(add_tiny_c_subdirectory name)
  # add_llvm_subdirectory(project type name)
  add_llvm_subdirectory(TINY_C TOOL ${name})
endmacro()

macro(add_tiny_c_library name)
  llvm_add_library(${name} STATIC ${ARGN})
endmacro()

macro(add_tiny_c_executable name)
  add_llvm_executable(${name} ${ARGN})
endmacro()

macro(add_tiny_c_tool name)
  add_tiny_c_executable(${name} ${ARGN})
  install(TARGETS ${name}
    RUNTIME DESTINATION bin
    COMPONENT ${name}
  )
endmacro()