//
// Created by mariusjenin on 04/12/22.
//

#ifndef PHOTONEAR_ROOTTRANSFORMCOMPONENT_H
#define PHOTONEAR_ROOTTRANSFORMCOMPONENT_H

#include "TransformComponent.h"

namespace component{
    class RootTransformComponent : public TransformComponent{
        void generate_ui_component_editor() override;

        void generate_ui_node_editor_ui() override;
    };
}


#endif //PHOTONEAR_ROOTTRANSFORMCOMPONENT_H
