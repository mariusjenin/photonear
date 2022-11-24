cmake_minimum_required(VERSION 3.0)
project(photonear)

find_package(OpenGL REQUIRED)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)")
endif ()
if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()
if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()

add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP


set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        )

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)


include_directories(
        external/glm/
        external/glew/include/
        external/glfw/include/
        external/stbi/
        src/
        src/common/
        src/utils/
        src/texture/
        src/scene/
        src/scene/node/
        src/component/
        src/component/shape/
        src/shader_manager/
        src/shader/
        .
)
add_executable(main main.cpp
        # stbi
        external/stbi/stb_image.h external/stbi/stb_image_write.h external/stbi/include_stbi.h
        # utils
        src/utils/printer.h src/utils/printer.cpp
        # common
        src/common/Transform.cpp src/common/Transform.h
        src/common/Ray.cpp src/common/Ray.h
        # texture
        src/texture/Texture.cpp src/texture/Texture.h
        src/texture/TextureColor.cpp src/texture/TextureColor.h
        src/texture/TextureImage.cpp src/texture/TextureImage.h
        # shader
        src/shader/vertex_shader.glsl src/shader/fragment_shader.glsl
        src/shader/depth_vertex_shader.glsl src/shader/depth_fragment_shader.glsl
        # shader_manager
        src/shader_manager/Shaders.cpp src/shader_manager/Shaders.h
        src/shader_manager/VertFragShaders.cpp src/shader_manager/VertFragShaders.h
        src/shader_manager/ShadersBufferManager.cpp src/shader_manager/ShadersBufferManager.h
        src/shader_manager/ShadersDataManager.cpp src/shader_manager/ShadersDataManager.h
        src/shader_manager/ShadowMap.cpp src/shader_manager/ShadowMap.h
        src/shader_manager/ShadowMapShaders.cpp src/shader_manager/ShadowMapShaders.h
        # node
        src/scene/node/AbstractNode.cpp src/scene/node/AbstractNode.h
        src/scene/node/RootNode.cpp src/scene/node/RootNode.h
        src/scene/node/Node.cpp src/scene/node/Node.h
        src/scene/node/NodeFactory.cpp src/scene/node/NodeFactory.h
        # component
        src/component/ComponentFactory.cpp src/component/ComponentFactory.h
        src/component/Component.cpp src/component/Component.h
        src/component/TransformComponent.cpp src/component/TransformComponent.h
        src/component/Material.cpp src/component/Material.h
        src/component/Camera.cpp src/component/Camera.h
        # shape
        src/component/Shape.cpp src/component/Shape.h
        src/component/shape/Sphere.cpp src/component/shape/Sphere.h
        src/component/shape/Triangle.cpp src/component/shape/Triangle.h
        # scene
        src/scene/SceneGraph.cpp src/scene/SceneGraph.h
        src/scene/Scene.cpp src/scene/Scene.h)

target_link_libraries(main
        ${ALL_LIBS}
        )

# Xcode and Visual working directories
set_target_properties(main PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/main/")
create_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/main/")


SOURCE_GROUP(src REGULAR_EXPRESSION ".*/src/.*")
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/shader/.*")


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode")
    add_custom_command(
            TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/"
    )
elseif (${CMAKE_GENERATOR} MATCHES "Xcode")

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode")