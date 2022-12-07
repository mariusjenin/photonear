//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_CAMERA_H
#define PHOTONEAR_CAMERA_H

#include "Component.h"
#include "VertFragShaders.h"

using namespace shader_manager;

#define CAMERA_POSITION point(0, 0, 0)
#define CAMERA_FORWARD versor(0, 0, -1)
#define CAMERA_UP versor(0, 1, 0)

namespace component {
    class Camera : public Component {

    private:
        bool m_capturing{};
        float m_fovy;
        float m_z_near;
        float m_z_far;

    public:
        explicit Camera(bool capturing = false, float fovy = 45.0f, float z_near = 0.001f, float z_far = 10000.0f);

        ComponentType get_type() override;

        float get_fovy() const;

        float get_z_near() const;

        float get_z_far() const;

        int is_capturing() const;

        void set_capturing(bool capturing);

        void load_in_shaders(const std::shared_ptr<Shaders>& shaders, int width, int height);

        void generate_ui_component_editor() override;
    };
}

#endif //PHOTONEAR_CAMERA_H
