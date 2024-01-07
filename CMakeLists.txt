#	  ☀️
cmake_minimum_required (VERSION 3.10)

project (2d3d LANGUAGES CXX)

set(IMGUI_SRC src/imgui)

set (SOURCE_FILES
    src/diplom.cpp
    src/geom.cpp
    src/dialogs.cpp
    src/project.cpp
    src/renderTarget.cpp
    ${IMGUI_SRC}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_SRC}/backends/imgui_impl_opengl2.cpp
    ${IMGUI_SRC}/imgui.cpp
    ${IMGUI_SRC}/imgui_demo.cpp
    ${IMGUI_SRC}/imgui_draw.cpp
    ${IMGUI_SRC}/imgui_tables.cpp
    ${IMGUI_SRC}/imgui_widgets.cpp
)
set (HEADER_FILES
    src/geom.h
    src/dialogs.h
    src/project.h
    src/renderTarget.h
)

if (UNIX AND NOT APPLE)
    set (LINUX TRUE)
    cmake_policy (SET CMP0072 NEW)
endif()

#if (WIN32)
#    set(ICON ${CMAKE_PROJECT_NAME}.rc)
#endif()


set (LIBSUFFIX "a")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    set (LIBSUFFIX "lib")
else ()
	set (CMAKE_CXX_STANDARD 17)
endif ()

set (CMAKE_PREFIX_PATH "./libs")

find_package (OpenGL REQUIRED)
find_package (SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package (SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

if (MINGW OR MSVC)
    add_compile_definitions(WINDOWS_PLATFORM)
endif ()


set(LIBS "./libs/lib")
set (LIBRARIES 
    -lm
    ${OPENGL_LIBRARIES}
)

set (LIBS_HEADERS
    "./libs/include"
    "${IMGUI_SRC}"
    "${IMGUI_SRC}/backends"
)

if (LINUX)
    set (LIBRARIES  ${LIBRARIES} -lrt -ldl -lX11)
endif (LINUX)


set (LIBRARIES  ${LIBRARIES} -lpthread)

include_directories (${LIBS_HEADERS})
link_directories (${LIBS})

add_executable (${CMAKE_PROJECT_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})

if(TARGET SDL2::SDL2main)
    set (LIBRARIES  ${LIBRARIES} SDL2::SDL2main)
endif()
set (LIBRARIES  ${LIBRARIES} SDL2::SDL2)

message (STATUS "LIBRARIES ${LIBRARIES}")

target_link_libraries (${CMAKE_PROJECT_NAME} ${LIBRARIES})
