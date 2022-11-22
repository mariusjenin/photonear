#version 430 core

//VERTEX DATA
layout(location = 0) in vec3 vertex_position;

//DEPTH MVP
uniform mat4 depth_vp_mat;
uniform mat4 model_mat;
uniform bool debug_rendering;

void main(){
    if(debug_rendering){
        gl_Position = depth_vp_mat * vec4(vertex_position,1);
    } else {
        gl_Position = depth_vp_mat * model_mat * vec4(vertex_position,1);
    }
}
