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
    light.set_type(LightType::DirectionType);
    light.set_direction(direction);
    return light;
}

Ray DirectedEmissiveMaterial::get_random_ray(glm::mat4 matrix) {
    float half_flt_max = ((double)RAND_MAX*0.5f);
    versor direction = normalize(versor((float)rand() - half_flt_max,(float)rand()-half_flt_max,(float)rand()-half_flt_max));
    direction = vec3(matrix * vec4(direction,0));
    point origin = vec3(matrix * vec4(vec3(0,0,0),1));
    return {origin,direction};
}