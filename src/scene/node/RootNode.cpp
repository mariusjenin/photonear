//
// Created by mariusjenin on 19/11/22.
//

#include "RootNode.h"


using namespace scene::node;

glm::mat4 RootNode::get_matrix_recursive(bool inverse) {
    if(inverse) {
        return m_trsf->get_inverse();
    }else{
        return m_trsf->get_matrix();
    }
}

glm::mat4 RootNode::get_matrix_recursive_local(bool inverse) {
    return get_matrix_recursive(inverse);
}
