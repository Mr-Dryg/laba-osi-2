cmake_minimum_required(VERSION 3.11)
project(laba)

add_library(lib src/filter.c src/matrix.c)

# add_executable(child src/remove_vowels.c)
# target_link_libraries(child PRIVATE lib)

add_executable(main main.c)
target_link_libraries(main PRIVATE lib)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources"
)