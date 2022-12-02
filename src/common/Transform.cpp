//
// Created by mariusjenin on 19/11/22.
//

#include "Transform.h"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using namespace common;

Transform::Transform(vec3 translation, vec3 rotation, vec3 scale, OrderRotation order_rotation) {
    init(translation, rotation, scale, order_rotation);
}

Transform::Transform(mat4 matrix) {
    init({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, OrderYXZ);
    m_matrix = matrix;
}

void Transform::matrix_to_trs(mat4 matrix_to_decompose, mat4 &t, mat4 &r, mat4 &s) {
    t = mat4(1.f);
    t[3][0] = matrix_to_decompose[3][0];
    t[3][1] = matrix_to_decompose[3][1];
    t[3][2] = matrix_to_decompose[3][2];

    matrix_to_decompose[3][0] = 0;
    matrix_to_decompose[3][1] = 0;
    matrix_to_decompose[3][2] = 0;

    s = mat4(1.f);
    for (int i = 0; i < 3; i++) {
        s[i][i] = matrix_to_decompose[3][3] * std::sqrt(
                matrix_to_decompose[i][0] * matrix_to_decompose[i][0] +
                matrix_to_decompose[i][1] * matrix_to_decompose[i][1] +
                matrix_to_decompose[i][2] * matrix_to_decompose[i][2]);
        if (s[i][i] != 0) {
            matrix_to_decompose[i][0] /= s[i][i];
            matrix_to_decompose[i][1] /= s[i][i];
            matrix_to_decompose[i][2] /= s[i][i];
        }
    }
    matrix_to_decompose[3][3] = 1.0f;

    vec3 tmp_z_axis = cross(vec3(matrix_to_decompose[0]), vec3(matrix_to_decompose[1]));
    if (dot(tmp_z_axis, vec3(matrix_to_decompose[2])) < 0) {
        s[0][0] *= -1;
        matrix_to_decompose[0][0] = -matrix_to_decompose[0][0];
        matrix_to_decompose[0][1] = -matrix_to_decompose[0][1];
        matrix_to_decompose[0][2] = -matrix_to_decompose[0][2];
    }

    r = matrix_to_decompose;
}

mat4
Transform::local_get_matrix_with_values(const vec3 tr, const vec3 rot, const vec3 sc,
                                        OrderRotation order_rotation, bool inverse) {

    mat4 res_mat;
    vec3 translat;
    vec3 rotat;
    vec3 scal;
    if (inverse) {
        translat = {tr.x * -1, tr.y * -1, tr.z * -1};
        rotat = {rot.x * -1, rot.y * -1, rot.z * -1};
        scal = {1.0f / sc.x, 1.0f / sc.y, 1.0f / sc.z};
    } else {
        translat = tr;
        rotat = rot;
        scal = sc;
    }
    const mat4 transformX = rotate(mat4(1.0f),
                                             radians(rotat.x),
                                             vec3(1.0f, 0.0f, 0.0f));
    const mat4 transformY = rotate(mat4(1.0f),
                                             radians(rotat.y),
                                             vec3(0.0f, 1.0f, 0.0f));
    const mat4 transformZ = rotate(mat4(1.0f),
                                             radians(rotat.z),
                                             vec3(0.0f, 0.0f, 1.0f));
    mat4 rotationMatrix;
    mat4 rot1, rot2, rot3;
    switch (order_rotation) {
        case ORDER_ZYX:
            rot1 = transformZ;
            rot2 = transformY;
            rot3 = transformX;
            break;
        case ORDER_ZXY:
            rot1 = transformZ;
            rot2 = transformX;
            rot3 = transformY;
            break;
        case ORDER_YXZ:
            rot1 = transformY;
            rot2 = transformX;
            rot3 = transformZ;
            break;
        case ORDER_YZX:
            rot1 = transformY;
            rot2 = transformZ;
            rot3 = transformX;
            break;
        case ORDER_XYZ:
            rot1 = transformX;
            rot2 = transformY;
            rot3 = transformZ;
            break;
        case ORDER_XZY:
            rot1 = transformX;
            rot2 = transformZ;
            rot3 = transformY;
            break;
        default:
            throw std::runtime_error("Bad order of rotat");
    }
    if (inverse) {
        rotationMatrix = rot3 * rot2 * rot1;
        res_mat = scale(mat4(1.0f), scal) *
                  rotationMatrix *
                  translate(mat4(1.0f), translat);
    } else {
        rotationMatrix = rot1 * rot2 * rot3;
        res_mat = translate(mat4(1.0f), translat) *
                  rotationMatrix *
                  scale(mat4(1.0f), scal);
    }

    return res_mat;
}

void Transform::compute(TransformComputing computing) {
    bool is_both_computing = computing == Both;
    if(computing == Classic || is_both_computing){
        if (!m_up_to_date) {
            m_matrix = local_get_matrix_with_values(m_translate, m_rot, m_scale, m_order_rotation, false);
            m_up_to_date = true;
        }
    }
    if(computing == Inverse || is_both_computing){
        if (!m_inverse_up_to_date) {
            m_inverse_matrix = local_get_matrix_with_values(m_translate, m_rot, m_scale, m_order_rotation, true);
            m_inverse_up_to_date = true;
        }
    }

}

const vec3 &Transform::get_translation() {
    return m_translate;
}

const vec3 &Transform::get_rotation() {
    return m_rot;
}

const vec3 &Transform::get_scale() {
    return m_scale;
}


const mat4 &Transform::get_matrix(bool inverse) {
    return inverse? m_inverse_matrix :m_matrix;
}

bool Transform::is_up_to_date(bool inverse) const {
    return m_up_to_date;
}

void Transform::set_translation(const vec3 &new_translation) {
    bool same_translation = new_translation == m_translate;
    m_up_to_date = m_up_to_date && same_translation;
    m_inverse_up_to_date = m_inverse_up_to_date && same_translation;
    m_translate = new_translation;
}

void Transform::set_rotation(const vec3 &new_rotation) {
    bool same_rotation = new_rotation == m_rot;
    m_up_to_date = m_up_to_date && same_rotation;
    m_inverse_up_to_date = m_inverse_up_to_date && same_rotation;
    m_rot = new_rotation;
}

void Transform::set_scale(const vec3 &new_scale) {
    bool same_scale = new_scale == m_scale;
    m_up_to_date = m_up_to_date && same_scale;
    m_inverse_up_to_date = m_inverse_up_to_date && same_scale;
    m_scale = new_scale;
}

void Transform::set_uniform_scale(float scale) {
    vec3 new_scale = {scale, scale, scale};
    set_scale(new_scale);
}

vec3 Transform::apply_to_vec3(vec3 &v, bool with_translation, bool with_normalization, bool inverse) {
    mat4 matrix = inverse ? m_inverse_matrix: m_matrix;
    float w = with_translation ? 1.f : 0.f;
    vec3 u = vec3(matrix * vec4(v, w));
    if (with_normalization) normalize(u);
    return u;
}

point Transform::apply_to_point(point &v, bool inverse) {
    return apply_to_vec3(v, true, false, inverse);
}

vec3 Transform::apply_to_vector(vec3 &v, bool inverse) {
    return apply_to_vec3(v, false, false, inverse);
}

versor Transform::apply_to_versor(versor &v, bool inverse) {
    return apply_to_vec3(v, false, true, inverse);
}

void Transform::apply_to_vec3_list(std::vector<vec3> *vects, bool with_translation, bool with_normalization, bool inverse) {
    int size_vector = (int)vects->size();
    for (int i = 0; i < size_vector; i++) {
        vects->at(i) = apply_to_vec3(vects->at(i), with_translation, with_normalization,inverse);
    }
}

void Transform::apply_point_list(std::vector<point> *points, bool inverse) {
    apply_to_vec3_list(points, true, false,inverse);
}

void Transform::apply_vector_list(std::vector<vec3> *vectors, bool inverse) {
    apply_to_vec3_list(vectors, false, false,inverse);
}

void Transform::apply_versor_list(std::vector<versor> *versors, bool inverse) {
    apply_to_vec3_list(versors, false, true,inverse);
}

void Transform::init(vec3 translation, vec3 rotation, vec3 scale, OrderRotation order_rotation) {
    m_translate = translation;
    m_rot = rotation;
    m_scale = scale;
    m_matrix = mat4(1.0f);
    m_inverse_matrix = mat4(1.0f);
    m_up_to_date = false;
    m_inverse_up_to_date = false;
    m_order_rotation = order_rotation;
}

void Transform::set_matrix(const mat4 &new_matrix, bool inverse) {
    if(inverse){
        m_inverse_matrix = new_matrix;
        m_inverse_up_to_date = m_inverse_up_to_date && new_matrix == m_inverse_matrix;
    }else {
        m_matrix = new_matrix;
        m_up_to_date = m_up_to_date && new_matrix == m_matrix;
    }
}


void Transform::set_order_rotation(OrderRotation order_rotation) {
    bool same_order = order_rotation == m_order_rotation;
    m_up_to_date = m_up_to_date && same_order;
    m_inverse_up_to_date = m_inverse_up_to_date && same_order;
    m_order_rotation = order_rotation;
}

OrderRotation Transform::get_order_rotation() const {
    return m_order_rotation;
}

