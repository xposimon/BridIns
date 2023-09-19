set(LLVM_LINK_COMPONENTS support)

add_clang_executable(BridIns
  BridIns.cpp
  )
target_link_libraries(BridIns
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )
