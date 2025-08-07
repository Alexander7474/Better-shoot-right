cmake_minimum_required(VERSION 3.16)
project(FinalGame LANGUAGES CXX)

# TODO -- Ajouter du debuggage dans imgui pour tous mettre dans un seule mode debug

# =========================
# Paramètres de compilation
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # Code indépendant de la position (utile pour libs dynamiques)

# Type de build par défaut : Debug si non défini
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# =========================
# Organisation des sources
# =========================
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# On collecte automatiquement les fichiers du moteur et du jeu
file(GLOB_RECURSE ENGINE_SRCS ${SRC_DIR}/engine/*.cpp)
file(GLOB_RECURSE GAME_SRCS   ${SRC_DIR}/game/*.cpp)

# Fichiers spécifiques à ImGui
set(IMGUI_SRCS
    ${SRC_DIR}/imgui/imgui.cpp
    ${SRC_DIR}/imgui/imgui_demo.cpp
    ${SRC_DIR}/imgui/imgui_draw.cpp
    ${SRC_DIR}/imgui/imgui_tables.cpp
    ${SRC_DIR}/imgui/imgui_widgets.cpp
    ${SRC_DIR}/backends/imgui_impl_glfw.cpp
    ${SRC_DIR}/backends/imgui_impl_opengl3.cpp
)

# Sources finales à compiler
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${ENGINE_SRCS}
    ${GAME_SRCS}
    ${IMGUI_SRCS}
)

# =========================
# Création de la lib ImGui
# =========================
add_library(imgui STATIC ${IMGUI_SRCS})
target_include_directories(imgui PUBLIC ${SRC_DIR}/imgui ${SRC_DIR}/backends)
target_compile_definitions(imgui PUBLIC IMGUI_DEBUG)

# Warnings utiles (GCC / Clang uniquement)
target_compile_options(imgui PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
    -Wall -Wno-delete-non-virtual-dtor -Wformat -fdiagnostics-color=always>
)

# =========================
# Exécutable principal
# =========================
add_executable(final ${SOURCES})
target_include_directories(final PRIVATE ${SRC_DIR} /usr/include/freetype2)
target_link_libraries(final PRIVATE imgui)

# =========================
# Détection des libs système
# =========================
find_package(PkgConfig QUIET)

# GLFW
if(PkgConfig_FOUND)
    pkg_check_modules(GLFW QUIET glfw3)
    if(GLFW_FOUND)
        target_include_directories(final PRIVATE ${GLFW_INCLUDE_DIRS})
        target_link_libraries(final PRIVATE ${GLFW_LIBRARIES})
    endif()
endif()

# OpenGL (obligatoire)
find_package(OpenGL REQUIRED)
target_link_libraries(final PRIVATE OpenGL::GL)

# GLEW
find_package(GLEW QUIET)
if(GLEW_FOUND)
    target_include_directories(final PRIVATE ${GLEW_INCLUDE_DIRS})
    target_link_libraries(final PRIVATE ${GLEW_LIBRARIES})
elseif(PkgConfig_FOUND)
    pkg_check_modules(GLEW QUIET glew)
    if(GLEW_FOUND)
        target_include_directories(final PRIVATE ${GLEW_INCLUDE_DIRS})
        target_link_libraries(final PRIVATE ${GLEW_LIBRARIES})
    endif()
endif()

# Freetype
if(PkgConfig_FOUND)
    pkg_check_modules(FREETYPE QUIET freetype2)
    if(FREETYPE_FOUND)
        target_include_directories(final PRIVATE ${FREETYPE_INCLUDE_DIRS})
        target_link_libraries(final PRIVATE ${FREETYPE_LIBRARIES})
    endif()
endif()

# SDL2
find_package(SDL2 QUIET)
if(TARGET SDL2::SDL2)
    target_link_libraries(final PRIVATE SDL2::SDL2 SDL2::SDL2main)
elseif(PkgConfig_FOUND)
    pkg_check_modules(SDL2 QUIET sdl2)
    if(SDL2_FOUND)
        target_include_directories(final PRIVATE ${SDL2_INCLUDE_DIRS})
        target_link_libraries(final PRIVATE ${SDL2_LIBRARIES})
    endif()
endif()

# SDL2_mixer (ajout de vraie détection)
find_package(SDL2_mixer QUIET)
if(TARGET SDL2_mixer::SDL2_mixer)
    target_link_libraries(final PRIVATE SDL2_mixer::SDL2_mixer)
elseif(PkgConfig_FOUND)
    pkg_check_modules(SDL2_MIXER QUIET SDL2_mixer)
    if(SDL2_MIXER_FOUND)
        target_include_directories(final PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
        target_link_libraries(final PRIVATE ${SDL2_MIXER_LIBRARIES})
    else()
        message(FATAL_ERROR "SDL2_mixer introuvable")
    endif()
else()
    message(FATAL_ERROR "SDL2_mixer introuvable")
endif()

# Bbop (lib locale)
find_library(BBOP_LIB NAMES bbop PATHS ${CMAKE_SOURCE_DIR}/Bbop-2D ${CMAKE_SOURCE_DIR}/Bbop-2D/lib NO_DEFAULT_PATH)
if(BBOP_LIB)
    target_link_libraries(final PRIVATE ${BBOP_LIB})
else()
    message(WARNING "Lib bbop introuvable, elle devra être dans le chemin de linkage.")
endif()

# Box2D
find_library(BOX2D_LIB NAMES box2d)
if(BOX2D_LIB)
    target_link_libraries(final PRIVATE ${BOX2D_LIB})
else()
    message(FATAL_ERROR "Box2D introuvable")
endif()

# LDtkLoader
find_library(LDTK_LIB NAMES LDtkLoader ldtkloader)
if(LDTK_LIB)
    target_link_libraries(final PRIVATE ${LDTK_LIB})
else()
    message(FATAL_ERROR "LDtkLoader introuvable")
endif()


# =========================
# Flags de debug
# =========================
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(final PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-g>)
endif()
