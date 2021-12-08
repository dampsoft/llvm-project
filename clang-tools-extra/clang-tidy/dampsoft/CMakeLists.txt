set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_library(clangTidyDampsoftModule
  AssertCheck.cpp
  ChangeValueSameParametersCheck.cpp
  HeaderStdafxCheck.cpp
  HeaderUsingCheck.cpp
  InlineStaticVariableCheck.cpp
  LambdaCaptureLeakCheck.cpp
  LoggersCheck.cpp
  SociTransactionCheck.cpp
  StdLockGuardCheck.cpp
  ThisCaptureThreadFunctionCheck.cpp

  DampsoftTidyModule.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyDampsoftModule
  PRIVATE
  clangAnalysis
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangSerialization
  clangTooling
  )
