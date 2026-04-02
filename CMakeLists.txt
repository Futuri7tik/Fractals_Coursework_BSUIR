cmake_minimum_required(VERSION 4.1)
project(KURSACH C)

set(CMAKE_C_STANDARD 11)

# === ПУТИ К БИБЛИОТЕКАМ ===
set(CSFML_DIR "D:/LIBS/CSFML" CACHE PATH "Путь к CSFML")
set(RAYLIB_DIR "D:/LIBS/raylib" CACHE PATH "Путь к raylib")
set(RAYGUI_DIR "D:/LIBS/raygui" CACHE PATH "Путь к raygui")

# === ИСХОДНЫЕ ФАЙЛЫ ===
add_executable(KURSACH
        functions.h
        tree.c
        sierpinski_carpet.c
        sierpinski_triangle.c
        # globals.c
        main.c
)

# === ПУТИ INCLUDE ===
target_include_directories(KURSACH PRIVATE
        ${CSFML_DIR}/include
        ${RAYLIB_DIR}/include
        ${RAYGUI_DIR}/src        # ← Изменено: добавлен /src
)

# === ПУТИ К БИБЛИОТЕКАМ ===
target_link_directories(KURSACH PRIVATE
        ${CSFML_DIR}/lib/gcc
        ${RAYLIB_DIR}/lib
)

# === ЛИНКОВКА ===
target_link_libraries(KURSACH
        csfml-graphics
        csfml-window
        csfml-system
        raylib
        opengl32
        gdi32
        winmm
        m
)

# === КОПИРОВАНИЕ DLL ===
add_custom_command(TARGET KURSACH POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CSFML_DIR}/bin/csfml-graphics-2.dll"
        "${CSFML_DIR}/bin/csfml-window-2.dll"
        "${CSFML_DIR}/bin/csfml-system-2.dll"
        "${RAYLIB_DIR}/lib/raylib.dll"
        "$<TARGET_FILE_DIR:KURSACH>"
)