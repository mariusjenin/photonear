//
// Created by mariusjenin on 12/12/22.
//

#ifndef PHOTONEAR_PHOTONMAPTESTSCENE_H
#define PHOTONEAR_PHOTONMAPTESTSCENE_H


#include "Scene.h"

namespace scene {
    class PhotonMapTestScene : public Scene {
    public:
        PhotonMapTestScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    private:
        void init_scene_graph() override;
    };
}

#endif //PHOTONEAR_PHOTONMAPTESTSCENE_H
