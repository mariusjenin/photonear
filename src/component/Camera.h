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
        bool m_capturing{};
        float m_fovy;
        float m_z_near;
        float m_z_far;

    public:
        explicit Camera(bool capturing = true, float fovy = 45.0f, float z_near = 0.1f, float z_far = 10000.0f);

        ComponentType get_type() override;

        int is_capturing() const;

        void set_capturing(bool capturing);

        void load_in_shaders(const std::shared_ptr<Shaders>& shaders, int width, int height);

        void generate_ui_component_editor() override;
    };
}

#endif //PHOTONEAR_CAMERA_H
