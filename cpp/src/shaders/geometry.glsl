#version 400 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 color;
} gs_in[];

out vec4 fColor;

void main() {
    vec4 position = gl_in[0].gl_Position;

    fColor = vec4(gl_Position.x, gl_Position.y, gl_Position.x, 1);
    gl_Position = position + vec4(-0.002, -0.002, 0.0, 0.0); 
    EmitVertex();
    fColor = vec4(1-gl_Position.x, gl_Position.y, gl_Position.x, 1);
    gl_Position = position + vec4(0.002, -0.002, 0.0, 0.0);
    EmitVertex();
    fColor = vec4(gl_Position.x, 1-gl_Position.y, gl_Position.y, 1);
    gl_Position = position + vec4(-0.002, 0.002, 0.0, 0.0);
    EmitVertex();
    fColor = vec4(1-gl_Position.x, 1-gl_Position.y, gl_Position.y, 1);
    gl_Position = position + vec4(0.002, 0.002, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}