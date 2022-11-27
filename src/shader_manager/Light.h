//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_LIGHT_H
#define PHOTONEAR_LIGHT_H

#include <memory>
#include "Shaders.h"
#include "ShadowMap.h"

using namespace glm;

namespace shader_manager{
    class Light {
    private:
        int m_type{};
        vec3 m_albedo{};
        //Positionned Light
        vec3 m_position{};
        float m_constant_attenuation{};
        float m_linear_attenuation{};
        float m_quadratic_attenuation{};
        //Directed Light
        vec3 m_direction{};
        //SpotLight
        float m_inner_cut_off{};
        float m_outer_cut_off{}; // if inner == outer then no smooth edge
        //Depth and Shadow Map
        int m_generate_depth_map{};
        GLuint m_index_sampler_depth_map{};
//        float m_bias_depth_map{};
        mat4 m_depth_vp_mat{};
        // Attribute not in shader
        int m_resolution{};
        float m_bias{};
        std::shared_ptr<ShadowMap> m_shadow_map;
    public:

        int get_type() const;

        const vec3 & get_albedo() const;

        const vec3 & get_position() const;

        float get_constant_attenuation() const;

        float get_linear_attenuation() const;

        float get_quadratic_attenuation() const;

        const vec3 & get_direction() const;

        float get_inner_cut_off() const;

        float get_outer_cut_off() const;

        float get_bias() const;

        int get_generate_depth_map() const;

        int get_index_sampler_depth_map() const;

        mat4 get_depth_vp_mat() const;

        int get_resolution() const;

        std::shared_ptr<ShadowMap> get_shadow_map() const;

        void set_type(int type);

        void set_albedo(vec3 albedo);

        void set_position(vec3 position);

        void set_constant_attenuation(float c_att);

        void set_linear_attenuation(float l_att);

        void set_quadratic_attenuation(float q_att);

        void set_direction(vec3 direction);

        void set_inner_cut_off(float in_co);

        void set_outer_cut_off(float out_co);

        void set_bias(float bias);

        void set_generate_depth_map(bool gen_depth_map);

        void set_depth_vp_mat(mat4 depth_vp_mat);

        void set_index_sampler_depth_map(int index_sampler_depth_map);

        void set_resolution(int resolution);

        void set_shadow_map(std::shared_ptr<ShadowMap> shadow_map);

        /**
         * Load the depth View and Projection Matrix
         * @param shaders
         */
        void load_depth_vp_matrix(Shaders* shaders);

    };

    struct LightShader{
        int type{};
        glsl_vec3 m_albedo{};
        //Positionned Light
        glsl_vec3 position{};
        float constant_attenuation{};
        float linear_attenuation{};
        float quadratic_attenuation{};
        //Directed Light
        glsl_vec3 direction{};
        //SpotLight
        float inner_cut_off{};
        float outer_cut_off{}; // if inner == outer then no smooth edge
        //Depth and Shadow Map
        int generate_depth_map{};
        int index_shadow_map{};
//        float bias_depth_map{};
        glsl_mat4 depth_vp_mat{};
        float bias{};

        LightShader();

        /**
         * Constructor of a LightSHader with LightInfo
         * @param li
         */
        explicit LightShader(const Light& light);
    };
}



#endif //PHOTONEAR_LIGHT_H
