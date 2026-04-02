cmake_minimum_required(VERSION 4.1)
project(KURSACH C)

set(CMAKE_C_STANDARD 11)

# === ПУТИ К БИБЛИОТЕКАМ ===
set(CSFML_DIR "D:/LIBS/CSFML" CACHE PATH "Путь к CSFML")
set(NUKLEAR_DIR "D:/LIBS/Nuklear-master" CACHE PATH "Путь к Nuklear")  # ← Добавить!

# === ИСХОДНЫЕ ФАЙЛЫ ===
add_executable(KURSACH
        functions.h
        globals.c              # ✅ Определение vertices и vertex_count
        tree.c                 # ✅ Логика дерева (без render_tree)
        sierpinski_triangle.c  # ✅ Треугольник Серпинского
        sierpinski_carpet.c    # ✅ Ковёр Серпинского
        main.c                 # ✅ Главный цикл + Nuklear UI
)

# === ПУТИ INCLUDE ===
target_include_directories(KURSACH PRIVATE
        ${CSFML_DIR}/include
        ${NUKLEAR_DIR}                    # ← Для nuklear.h
        ${NUKLEAR_DIR}/demo/sfml_opengl2  # ← Для nuklear_sfml_gl2.h
)

# === ПУТИ К БИБЛИОТЕКАМ ===
target_link_directories(KURSACH PRIVATE
        ${CSFML_DIR}/lib/gcc
)

# === ЛИНКОВКА ===
target_link_libraries(KURSACH
        csfml-graphics
        csfml-window
        csfml-system
        opengl32          # ← 🔥 КРИТИЧНО для Nuklear SFML backend!
        m
)

# === КОПИРОВАНИЕ DLL ===
add_custom_command(TARGET KURSACH POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CSFML_DIR}/bin/csfml-graphics-2.dll"
        "${CSFML_DIR}/bin/csfml-window-2.dll"
        "${CSFML_DIR}/bin/csfml-system-2.dll"
        "$<TARGET_FILE_DIR:KURSACH>"
)