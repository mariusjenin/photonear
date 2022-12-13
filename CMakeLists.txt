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
        .
        external/glm/
        external/glew/include/
        external/glfw/include/
        external/imgui/
        external/stbi/
        src/
        src/common/
        src/utils/
        src/texture/
        src/scene/
        src/scene/example/
        src/scene/node/
        src/component/
        src/component/shape/
        src/component/material/
        src/shader_manager/
        src/shader/
        src/ray_tracing/
)
add_executable(main main.cpp
        src/Photonear.cpp src/Photonear.h
        # imgui
        external/imgui/imconfig.h
        external/imgui/imgui.cpp external/imgui/imgui.h
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_impl_glfw.cpp external/imgui/imgui_impl_glfw.h
        external/imgui/imgui_impl_opengl3.cpp external/imgui/imgui_impl_opengl3.h
        external/imgui/imgui_impl_opengl3_loader.h
        external/imgui/imgui_internal.h
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imstb_rectpack.h
        external/imgui/imstb_textedit.h
        external/imgui/imstb_truetype.h
        # stbi
        external/stbi/stb_image.h external/stbi/stb_image_write.h external/stbi/include_stbi.h
        # utils
        src/utils/printer.h src/utils/printer.cpp
        # common
        src/common/Transform.cpp src/common/Transform.h
        src/common/vec3_type.h
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
        src/shader_manager/Light.cpp src/shader_manager/Light.h
        src/shader_manager/TextureManager.cpp src/shader_manager/TextureManager.h
        # node
        src/scene/node/AbstractNode.cpp src/scene/node/AbstractNode.h
        src/scene/node/RootNode.cpp src/scene/node/RootNode.h
        src/scene/node/Node.cpp src/scene/node/Node.h
        src/scene/node/NodeFactory.cpp src/scene/node/NodeFactory.h
        # component
        src/component/Component.cpp src/component/Component.h
        src/component/TransformComponent.cpp src/component/TransformComponent.h
        src/component/RootTransformComponent.cpp src/component/RootTransformComponent.h
        src/component/Camera.cpp src/component/Camera.h
        src/component/MaskedComponent.cpp src/component/MaskedComponent.h
        src/component/BoundingBox.cpp src/component/BoundingBox.h
        # shape
        src/component/shape/Shape.cpp src/component/shape/Shape.h
        src/component/shape/Sphere.cpp src/component/shape/Sphere.h
        src/component/shape/Quad.cpp src/component/shape/Quad.h
        # material
        src/component/material/Material.cpp src/component/material/Material.h
        src/component/material/DiffuseMaterial.cpp src/component/material/DiffuseMaterial.h
        src/component/material/DielectricMaterial.cpp src/component/material/DielectricMaterial.h
        src/component/material/ConductorMaterial.cpp src/component/material/ConductorMaterial.h
        src/component/material/EmissiveMaterial.cpp src/component/material/EmissiveMaterial.h
        src/component/material/DirectedEmissiveMaterial.cpp src/component/material/DirectedEmissiveMaterial.h
        src/component/material/PositionnedEmissiveMaterial.cpp src/component/material/PositionnedEmissiveMaterial.h
        src/component/material/SpotEmissiveMaterial.cpp src/component/material/SpotEmissiveMaterial.h
        # scene
        src/scene/SceneGraph.cpp src/scene/SceneGraph.h
        src/scene/Scene.cpp src/scene/Scene.h
        src/scene/example/CornellBox.cpp src/scene/example/CornellBox.h
        src/scene/example/QuadScene.cpp src/scene/example/QuadScene.h
        # ray_tracing
        src/ray_tracing/Ray.cpp src/ray_tracing/Ray.h
        src/ray_tracing/RayTracer.cpp src/ray_tracing/RayTracer.h
        src/ray_tracing/PhotonMapper.cpp src/ray_tracing/PhotonMapper.h
        src/ray_tracing/Photon.cpp src/ray_tracing/Photon.h
        src/ray_tracing/RayCastHit.cpp src/ray_tracing/RayCastHit.h
        src/ray_tracing/RayTraceHit.cpp src/ray_tracing/RayTraceHit.h src/ray_tracing/PhotonMap.cpp src/ray_tracing/PhotonMap.h src/scene/example/PhotonMapTestScene.cpp src/scene/example/PhotonMapTestScene.h)

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