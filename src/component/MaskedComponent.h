//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_MASKEDCOMPONENT_H
#define PHOTONEAR_MASKEDCOMPONENT_H

#include "Component.h"

namespace component{
    class MaskedComponent : public Component{
        //TODO uncomment
//        void generate_ui_node_editor_ui() override;

        void generate_ui_component_editor() override;
    };
}


#endif //PHOTONEAR_MASKEDCOMPONENT_H
