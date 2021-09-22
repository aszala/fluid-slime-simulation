#version 430

uint hash(uint x) {
    x += (x << 10u);
    x ^= (x >>  6u);
    x += (x <<  3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}

uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

float floatConstruct(uint m) {
    const uint ieeeMantissa = 0x007FFFFFu;
    const uint ieeeOne = 0x3F800000u;

    m &= ieeeMantissa;
    m |= ieeeOne;

    float f = uintBitsToFloat(m);
    return f - 1.0;
}

float random(float x) { return floatConstruct(hash(floatBitsToUint(x))); }
float random(vec2 v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec3 v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec4 v) { return floatConstruct(hash(floatBitsToUint(v))); }

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
float speed = 1.6;

void main() {
	uint index = gl_GlobalInvocationID.x;

	float rotation = outBuffer.data[index].angle.x;

	vec2 delta = vec2(speed * sin(rotation), speed * -cos(rotation))*in_dt;
	vec2 predicted = delta + outBuffer.data[index].position.xy;

	if (predicted.x < -1 || predicted.x > 1 || predicted.y < -1 || predicted.y > 1) {
		predicted = vec2(min(1, max(-1, predicted.x)), min(1, max(-1, predicted.y)));

		outBuffer.data[index].angle.x += random(predicted) * M_PI * 2;
	}

	outBuffer.data[index].position.xy = predicted;
}