#version 430

// uint hash(uint state) {
//     state ^= 2747636419u;
//     state *= 2654435769u;
//     state ^= state >> 16;
//     state *= 2654435769u;
//     state ^= state >> 16;
//     state *= 2654435769u;
//     return state;
// }

// float scaleToRange01(uint state) {
//     return float(state) / 4294967295.0;
// }

struct VertexData {
	vec4 position;
	vec4 color;
	vec4 angle;
};

layout(binding = 0) buffer block {
	VertexData data[];
} outBuffer;


layout(location = 0) uniform float in_dt;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

float M_PI = 3.14159265358979323846;
float speed = 0.1;

vec4 simulationBounds = vec4(0, 0, 1280, 720);

void main() {
	uint index = gl_GlobalInvocationID.x;

	float rotation = outBuffer.data[index].angle.x;

	vec2 delta = vec2(speed * sin(rotation), speed * -cos(rotation))*in_dt;
	vec2 predicted = delta + outBuffer.data[index].position.xy;

	// uint random = hash(uint(outBuffer.data[index].position.y * simulationBounds.z + outBuffer.data[index].position.x + hash(uint(index + in_dt) * 100000)));

	// if (predicted.x < simulationBounds.x || predicted.x > simulationBounds.z || predicted.y < simulationBounds.y || predicted.y > simulationBounds.w) {
	// 	//predicted = vec2(min(simulationBounds.z, max(0, predicted.x)), min(simulationBounds.w, max(0, predicted.y)));

	// 	outBuffer.data[index].angle.x = scaleToRange01(random) * 2 * M_PI;
	// }

	outBuffer.data[index].position.xy = predicted;
	outBuffer.data[index].color += vec4(outBuffer.data[index].position.xy, 0, 1) * in_dt;
	
}