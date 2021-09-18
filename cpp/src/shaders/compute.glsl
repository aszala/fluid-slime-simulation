#version 430

struct VertexData {
	vec2 position;
	vec4 color;
	//vec2 vel;
};

layout(binding = 0) buffer block {
	VertexData data[];
} outBuffer;



layout(location = 0) uniform float in_dt;  
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

float M_PI = 3.14159265358979323846;
float speed = 0.1;

void main() {
	uint index = gl_GlobalInvocationID.x;

	float actualRotation = 180;
	float rotation = (actualRotation+90) * M_PI / 180;

	outBuffer.data[index].position.xy = vec2(speed * sin(rotation), speed * -cos(rotation))*in_dt + outBuffer.data[index].position.xy;
	outBuffer.data[index].color += vec4(outBuffer.data[index].position.xy, 0, 1) * in_dt;
	
}