//
// Created by mariusjenin on 24/11/22.
//

#include <utility>

#include "DirectedEmissiveMaterial.h"
#include "TransformComponent.h"


using namespace component::material;

DirectedEmissiveMaterial::DirectedEmissiveMaterial(std::shared_ptr<TextureColor> albedo) : EmissiveMaterial(
        std::move(albedo)) {}


Light DirectedEmissiveMaterial::generate_light() {
    auto node = Component::get_node(this);
    auto trsf_comp = Component::get_nearest_component_upper<TransformComponent>(&*node);
    auto matrix = trsf_comp->get_matrix_as_end_node();
    Transform trsf = Transform(matrix);
    vec3 direction = {0, 0, -1};
    direction = trsf.apply_to_versor(direction);

    Light light = EmissiveMaterial::generate_light();
    light.set_type(LIGHT_TYPE_DIRECTIONAL);
    light.set_direction(direction);
    return light;
}