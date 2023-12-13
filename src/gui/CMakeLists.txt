add_library(gui gui.cpp)

# Includes the Parent Dir for the header files
target_include_directories(gui PUBLIC ../)

target_include_directories(gui PUBLIC ${GTK4_INCLUDE_DIRS})
target_link_directories(gui PUBLIC ${GTK4_LIBRARY_DIRS})
target_link_libraries(gui ${GTK4_LIBRARIES})
