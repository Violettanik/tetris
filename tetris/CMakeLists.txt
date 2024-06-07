cmake_minimum_required(VERSION 3.10.0)
add_executable(tetris_test tetris.cpp test.cpp)
target_link_libraries(tetris_test PUBLIC doctest default)
add_test(NAME tetris_test COMMAND tetris_test --force-colors -d)
add_custom_target(cloud-test COMMAND tetris_test --force-colors -d)

add_executable(tetris tetris.cpp main.cpp)
target_link_libraries(tetris PUBLIC default)
