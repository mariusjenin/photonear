//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_CAMERA_H
#define PHOTONEAR_CAMERA_H

#include "Component.h"
#include "VertFragShaders.h"

using namespace shader_manager;

#define CAMERA_POSITION glm::vec3(0, 0, 0)
#define CAMERA_FORWARD glm::vec3(0, 0, -1)
#define CAMERA_UP glm::vec3(0, 1, 0)

namespace component {
    class Camera : public Component {

    private:
        int m_priority;
        float m_fovy;
        float m_z_near;
        float m_z_far;

    public:
        explicit Camera(int priority = 0, float fovy = 45.0f, float z_near = 0.1f, float z_far = 10000.0f);

        ComponentType get_type() override;

        int get_priority() const;

        void load_in_shaders(GLFWwindow *window, const std::shared_ptr<VertFragShaders>& shaders);
    };
}

#endif //PHOTONEAR_CAMERA_H
