//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_TRANSFORM_H
#define PHOTONEAR_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#define ORDER_ZYX 0
#define ORDER_ZXY 1
#define ORDER_YXZ 2
#define ORDER_YZX 3
#define ORDER_XYZ 4
#define ORDER_XZY 5

using namespace glm;
typedef vec3 point;
typedef vec3 vector;
typedef vec3 versor;


namespace common {

    enum OrderRotation : int {
        OrderZYX,
        OrderZXY,
        OrderYXZ,
        OrderYZX,
        OrderXYZ,
        OrderXZY,
    };

    enum TransformComputing : int {
        Classic,
        Inverse,
        Both
    };

/// Class representing a 4 by 4 Matrix (Translation + Rotation + Scale)
    class Transform {
    protected:
        //Local space information
        vec3 m_translate{};
        vec3 m_rot{}; //In degrees
        vec3 m_scale{};
        //Global space information concatenate in matrix
        mat4 m_matrix{};
        mat4 m_inverse_matrix{};
        bool m_up_to_date{};
        bool m_inverse_up_to_date{};
        OrderRotation m_order_rotation{};

        /**
         * Compute a matrix with given values
         * @param tr
         * @param rot
         * @param sc
         * @param order_rotation
         * @return matrix
         */
        static mat4
        local_get_matrix_with_values(vec3 tr, vec3 rot, vec3 sc, OrderRotation order_rotation, bool inverse = false);

        /**
         * Apply the Transform to a vec3 (General method)
         * @param v
         * @param with_translation
         * @param with_normalization
         * @param inverse
         * @return vector
         */
        vec3 apply_to_vec3(vec3 &v, bool with_translation, bool with_normalization, bool inverse = false);

        /**
         * Apply the Transform to a list of vec3 (General method)
         * @param vects
         * @param with_translation
         * @param with_normalization
         * @param inverse
         */
        void apply_to_vec3_list(std::vector<vec3> *vects, bool with_translation,
                                bool with_normalization, bool inverse = false);   //General method
    public:

        /**
         * Constructor of the Transform
         * @param translation
         * @param rotation
         * @param scale
         * @param order_rotation
         */
        explicit Transform(vec3 translation = {0.0f, 0.0f, 0.0f}, vec3 rotation = {0.0f, 0.0f, 0.0f},
                           vec3 scale = {1.0f, 1.0f, 1.0f}, OrderRotation order_rotation = OrderYXZ);

        /**
         * Constructor of a temporary Transdform
         * @param matrix
         */
        explicit Transform(glm::mat4 matrix);

        /**
         * Decompose a matrix to 3 matrices(TRS)
         * @param matrix_to_decompose
         * @param t
         * @param r
         * @param s
         */
        static void matrix_to_trs(mat4 matrix_to_decompose, mat4 &t, mat4 &r, mat4 &s);

        /**
         * Init method also used to reset the Transform
         * @param translation
         * @param rotation
         * @param scale
         * @param order_rotation
         */
        void init(vec3 translation = {0.0f, 0.0f, 0.0f}, vec3 rotation = {0.0f, 0.0f, 0.0f},
                  vec3 scale = {1.0f, 1.0f, 1.0f}, OrderRotation order_rotation = OrderYXZ);

        /// Compute the matrix of the Transform
        void compute(TransformComputing computing = Classic);

        /**
         * Setter of the order of rotation
         * @param order_rotation
         */
        void set_order_rotation(OrderRotation order_rotation);

        /**
         * Setter of the translation
         * @param new_translation
         */
        void set_translation(const vec3 &new_translation);

        /**
         * Setter of the rotation
         * @param new_rotation
         */
        void set_rotation(const vec3 &new_rotation);

        /// Setter of the scale
        void set_scale(const vec3 &new_scale);

        /// Setter of the scale with an uniform scale
        void set_uniform_scale(float scale);

        /**
         * Setter of the matrix
         * @param new_matrix
         */
        void set_matrix(const mat4 &new_matrix, bool inverse = false);

        /**
         * Getter of the translation
         * @return translation
         */
        const vec3 &get_translation();

        /**
         * Getter of the scale
         * @return scale
         */
        const vec3 &get_scale();

        /**
         * Getter of the order
         * @return scale
         */
        OrderRotation get_order_rotation() const;

        /**
         * Getter of the rotation
         * @return rotation
         */
        const vec3 &get_rotation();

        /**
         * Getter of the matrices
         * @param inverse
         * @return matrix
         */
        const mat4 &get_matrix(bool inverse = false);


        /**
         * Getter of whether the matrix is up to date or no
         * @param inverse
         * @return up to date
         */
        bool is_up_to_date(bool inverse = false) const;


        /**
         * Apply the Transform to a point
         * @param v
         * @return point
         */

        /**
         * Apply the Transform to a point
         * @param v
         * @param inverse
         * @return point
         */
        point apply_to_point(point &v, bool inverse = false);


        /**
         * Apply the Transform to a vector
         * @param v
         * @param inverse
         * @return vector
         */
        vector apply_to_vector(vector &v, bool inverse = false);

        /**
         * Apply the Transform to a versor
         * @param v
         * @param inverse
         * @return versor
         */
        versor apply_to_versor(versor &v, bool inverse = false);

        /**
         * Apply the Transform to a list of point_
         * @param points
         * @param inverse
         */
        void apply_point_list(std::vector<point> *points, bool inverse = false);

        /**
         * Apply the Transform to a list of vector
         * @param vectors
         * @param inverse
         */
        void apply_vector_list(std::vector<vector> *vectors, bool inverse = false);

        /**
         * Apply the Transform to a list of versor
         * @param versors
         * @param inverse
         */
        void apply_versor_list(std::vector<versor> *versors, bool inverse = false);

        /**
         * Operator equal
         * @param trsf1
         * @param trsf2
         * @return is_equal
         */
        friend bool operator==(const Transform &trsf1, const Transform &trsf2) {
            return trsf1.m_translate == trsf2.m_translate &&
                   trsf1.m_rot == trsf2.m_rot &&
                   trsf1.m_scale == trsf2.m_scale &&
                   trsf1.m_matrix == trsf2.m_matrix &&
                   trsf1.m_up_to_date == trsf2.m_up_to_date;
        }

        /**
         * Operator inequal
         * @param trsf1
         * @param trsf2
         * @return is_inequal
         */
        friend bool operator!=(const Transform &trsf1, const Transform &trsf2) {
            return !(trsf1 == trsf2);
        }
    };
}

#endif //PHOTONEAR_TRANSFORM_H
