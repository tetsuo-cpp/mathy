add_executable(mathy_test test_main.cpp)
target_link_libraries(mathy_test mathy_lib)
target_include_directories(mathy_test PRIVATE ../ext/Catch2/single_include)
