#version 400 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 color;
} gs_in[];

uniform int screen_width;
uniform int screen_height;

out vec4 fColor;

float size = 2;

void main() {
    vec4 position = gl_in[0].gl_Position;

    float size_x = size / screen_width;
    float size_y = size / screen_height;

    fColor = vec4(gl_Position.x, gl_Position.y, gl_Position.x, 1);
    gl_Position = position + vec4(-size_x, -size_y, 0.0, 0.0); 
    EmitVertex();

    fColor = vec4(1-gl_Position.x, gl_Position.y, gl_Position.x, 1);
    gl_Position = position + vec4(size_x, -size_y, 0.0, 0.0);
    EmitVertex();

    fColor = vec4(gl_Position.x, 1-gl_Position.y, gl_Position.y, 1);
    gl_Position = position + vec4(-size_x, size_y, 0.0, 0.0);
    EmitVertex();

    fColor = vec4(1-gl_Position.x, 1-gl_Position.y, gl_Position.y, 1);
    gl_Position = position + vec4(size_x, size_y, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}