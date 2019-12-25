set(
  MATHY_TEST_FILES
  LexerTest.cpp
  mathy_test.cpp
  )

add_executable(mathy_test ${MATHY_TEST_FILES})
target_link_libraries(mathy_test mathy_lib)
target_include_directories(mathy_test PRIVATE ../lib)
target_include_directories(mathy_test PRIVATE ../ext/Catch2/single_include)
