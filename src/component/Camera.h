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
    public:
        Camera(int priority = 0);

        ComponentType get_type() override;

        int get_priority() const;

        static void update_view_mat(Transform trsf, std::shared_ptr<VertFragShaders> shaders);

        static void update_view_pos(Transform trsf, std::shared_ptr<VertFragShaders> shaders);
    };
}

#endif //PHOTONEAR_CAMERA_H
