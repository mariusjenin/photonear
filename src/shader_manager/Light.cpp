//
// Created by mariusjenin on 24/11/22.
//

#include "Light.h"
#include <ShadersDataManager.h>

#include <utility>

using namespace shader_manager;


int Light::get_type() const {
    return m_type;
}

const vec3 &Light::get_albedo() const {
    return m_albedo;
}

const vec3 &Light::get_position() const {
    return m_position;
}

float Light::get_linear_attenuation() const {
    return m_linear_attenuation;
}

float Light::get_quadratic_attenuation() const {
    return m_quadratic_attenuation;
}

const vec3 &Light::get_direction() const {
    return m_direction;
}

float Light::get_inner_cut_off() const {
    return m_inner_cut_off;
}

float Light::get_outer_cut_off() const {
    return m_outer_cut_off;
}

float Light::get_bias() const {
    return m_bias;
}

int Light::get_generate_depth_map() const {
    return m_generate_depth_map;
}

int Light::get_index_sampler_depth_map() const {
    return m_index_sampler_depth_map;
}

mat4 Light::get_depth_vp_mat() const {
    return m_depth_vp_mat;
}

int Light::get_resolution() const {
    return m_resolution;
}

std::shared_ptr<ShadowMap> Light::get_shadow_map() const {
    return m_shadow_map;
}

void Light::set_type(int type){
    m_type = type;
}

void Light::set_albedo(vec3 albedo){
    m_albedo = albedo; ;
}

void Light::set_position(vec3 position){
    m_position = position; ;
}
void Light::set_linear_attenuation(float l_att){
    m_linear_attenuation = l_att;
}

void Light::set_quadratic_attenuation(float q_att){
    m_quadratic_attenuation = q_att;
}

void Light::set_direction(vec3 direction){
    m_direction = direction; ;
}

void Light::set_inner_cut_off(float in_co){
    m_inner_cut_off = in_co;
}

void Light::set_outer_cut_off(float out_co){
    m_outer_cut_off = out_co;
}

void Light::set_bias(float bias){
    m_bias = bias;
}

void Light::set_generate_depth_map(bool gen_depth_map){
    m_generate_depth_map = gen_depth_map;
}

void Light::set_depth_vp_mat(mat4 depth_vp_mat){
    m_depth_vp_mat = depth_vp_mat;
}

void Light::set_index_sampler_depth_map(int index_sampler_depth_map) {
    m_index_sampler_depth_map = index_sampler_depth_map;
}

void Light::set_resolution(int resolution) {
    m_resolution = resolution;
}

void Light::set_shadow_map(std::shared_ptr<ShadowMap> shadow_map) {
    m_shadow_map = std::move(shadow_map);
}

void Light::load_depth_vp_matrix(Shaders *shaders) {
    glUniformMatrix4fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::SHADOW_MAP_DEPTH_VP_MAT_LOC_NAME), 1, GL_FALSE, &m_depth_vp_mat[0][0]);
}

LightShader::LightShader(const Light& light) {
    type = light.get_type();
    m_albedo = glsl_vec3(light.get_albedo());
    position = glsl_vec3(light.get_position());
    linear_attenuation = light.get_linear_attenuation();
    quadratic_attenuation = light.get_quadratic_attenuation();
    //Directed Light
    direction = glsl_vec3(light.get_direction());
    //SpotLight
    inner_cut_off = light.get_inner_cut_off();
    outer_cut_off = light.get_outer_cut_off(); // if inner == outer then no smooth edge
    //Depth and Shadow Map
    generate_depth_map = light.get_generate_depth_map();
    index_shadow_map = light.get_index_sampler_depth_map();
    bias = light.get_bias();
    depth_vp_mat = glsl_mat4(light.get_depth_vp_mat());
}

LightShader::LightShader() = default;
