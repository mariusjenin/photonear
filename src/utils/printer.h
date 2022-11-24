
#ifndef GAME_ENGINE_PRINTER_HPP
#define GAME_ENGINE_PRINTER_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

/**
 * print a glm::vec3
 * @param v
 */
void print_vec3(glm::vec3 v);

/**
 * print a glm::vec4
 * @param v
 */
void print_vec4(glm::vec4 v);

/**
 * print a glm::mat4
 * @param m
 */
void print_mat4(glm::mat4 m);

/**
 * print a glm::mat3
 * @param m
 */
void print_mat3(glm::mat3 m);

#endif //GAME_ENGINE_PRINTER_HPP
