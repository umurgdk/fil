cmake_minimum_required(VERSION 3.7)
project(Fil)

set(CMAKE_C_STANDARD 99)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0 json-glib)
pkg_check_modules(JSON REQUIRED json-glib-1.0)

include_directories(${GTK3_INCLUDE_DIRS}
                    ${JSON_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

set(SOURCE_FILES
        src/main.c
        src/adapters/fil-timeline-model.c src/models.h src/adapters/fil-toot-cell-renderer.h src/adapters/fil-toot-cell-renderer.c)

add_executable(Fil ${SOURCE_FILES})

target_link_libraries(Fil ${GTK3_LIBRARIES} ${JSON_LIBRARIES})