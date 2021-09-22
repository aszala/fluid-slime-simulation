#version 400 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_color;

out VS_OUT {
    vec4 color;
} vs_out;

void main() {
    vs_out.color = in_color;
    gl_Position = in_position; 
}