
#ifndef GAME_ENGINE_PRINTER_HPP
#define GAME_ENGINE_PRINTER_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "vec3_type.h"

/**
 * print a vec3
 * @param v
 */
void print_vec3(vec3 v);

/**
 * print a vec4
 * @param v
 */
void print_vec4(vec4 v);

/**
 * print a mat4
 * @param m
 */
void print_mat4(mat4 m);

/**
 * print a mat3
 * @param m
 */
void print_mat3(mat3 m);

#endif //GAME_ENGINE_PRINTER_HPP
