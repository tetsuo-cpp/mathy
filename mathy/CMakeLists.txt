add_executable(mathy mathy.cpp)
target_link_libraries(mathy mathy_lib)
target_include_directories(mathy PRIVATE ../lib)
