cmake_minimum_required(VERSION 3.27)
project(Calculator)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_subdirectory(src)
