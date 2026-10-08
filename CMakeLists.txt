cmake_minimum_required(VERSION 3.10)
project(PA4_SceneEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios de inclusión
include_directories(include)
include_directories(include/core)
include_directories(include/graphics)
include_directories(include/scene)
include_directories(include/animation)

# Paquetes gráficos requeridos
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Recopilación de archivos fuente
set(SOURCES
    src/main.cpp
    src/core/TimeManager.cpp
    src/graphics/Camera.cpp
    src/graphics/Light.cpp
    src/graphics/TextureLoader.cpp
    src/graphics/HUD.cpp
    src/scene/Geometry.cpp
    src/scene/ModelOBJ.cpp
    src/scene/SceneNode.cpp
    src/scene/Scene.cpp
    src/animation/AnimationController.cpp
)

add_executable(PA4_SceneEditor ${SOURCES})

target_link_libraries(PA4_SceneEditor
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)
