add_subdirectory(gui)
add_subdirectory(core)

add_executable(calculator main.cpp)
target_link_libraries(calculator gui core)
