
#include <src/utils/printer.h>

using namespace glm;

void print_vec3(glm::vec3 v) {
    std::cout << v[0] << "\t" << v[1] << "\t" << v[2] << std::endl;
}

void print_vec4(glm::vec4 v) {
    std::cout << v[0] << "\t" << v[1] << "\t" << v[2] << "\t" << v[3] << std::endl;
}

void print_mat4(glm::mat4 m) {
    std::cout << m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << m[0][3] << "\t" << std::endl;
    std::cout << m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << m[1][3] << "\t" << std::endl;
    std::cout << m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\t" << m[2][3] << "\t" << std::endl;
    std::cout << m[3][0] << "\t" << m[3][1] << "\t" << m[3][2] << "\t" << m[3][3] << "\n" << std::endl;
}

void print_mat3(glm::mat3 m) {
    std::cout << m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << std::endl;
    std::cout << m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << std::endl;
    std::cout << m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\n" << std::endl;
}