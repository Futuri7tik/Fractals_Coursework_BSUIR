cmake_minimum_required(VERSION 3.20)
project(KURSACH C)

set(CMAKE_C_STANDARD 11)
set(RAYLIB_DIR "D:/LIBS/raylib" CACHE PATH "Путь к raylib")
set(RAYGUI_DIR "D:/LIBS/raygui" CACHE PATH "Путь к raygui")

find_package(OpenMP REQUIRED)

# === ИСХОДНЫЕ ФАЙЛЫ ===
add_executable(KURSACH
        functions.h
        tree.c
        sierpinski_carpet.c
        sierpinski_triangle.c
        main.c
        mandel.c
        ui.c
        julia.c
        add.c
)

if(OpenMP_C_FOUND)
    target_link_libraries(KURSACH PRIVATE OpenMP::OpenMP_C)
    set_target_properties(KURSACH PROPERTIES COMPILE_FLAGS "${OpenMP_C_FLAGS}")
    set_target_properties(KURSACH PROPERTIES LINK_FLAGS "${OpenMP_C_FLAGS}")
endif()

# === ПУТИ INCLUDE ===
target_include_directories(KURSACH PRIVATE
        ${CSFML_DIR}/include
        ${RAYLIB_DIR}/include
        ${RAYGUI_DIR}/src
)

# === ПУТИ К БИБЛИОТЕКАМ ===
target_link_directories(KURSACH PRIVATE
        ${RAYLIB_DIR}/lib
)

# === ЛИНКОВКА ===
target_link_libraries(KURSACH PRIVATE
        raylib
        opengl32
        gdi32
        winmm
        m
)
