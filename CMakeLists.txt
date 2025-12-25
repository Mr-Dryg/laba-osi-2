cmake_minimum_required(VERSION 3.11)
project(laba)

add_library(lib src/filter.c src/matrix.c)

# add_executable(child src/remove_vowels.c)
# target_link_libraries(child PRIVATE lib)

add_executable(main main.c)
target_link_libraries(main PRIVATE lib pthread)

add_executable(test test.c)
target_link_libraries(test PRIVATE pthread)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources"
)

# file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/stats.py"
#     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
# )