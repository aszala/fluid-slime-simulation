#version 430

// uint hash(uint x) {
//     x += (x << 10u);
//     x ^= (x >>  6u);
//     x += (x <<  3u);
//     x ^= (x >> 11u);
//     x += (x << 15u);
//     return x;
// }

// uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
// uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
// uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

// float floatConstruct(uint m) {
//     const uint ieeeMantissa = 0x007FFFFFu;
//     const uint ieeeOne = 0x3F800000u;

//     m &= ieeeMantissa;
//     m |= ieeeOne;

//     float f = uintBitsToFloat(m);
//     return f - 1.0;
// }

// float random(float x) { return floatConstruct(hash(floatBitsToUint(x))) / 1000000000; }

float random(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

struct VertexData {
	vec4 position;
	vec4 color;
	vec4 angle;
};

layout(binding = 3, std430) buffer block {
	VertexData data[];
} outBuffer;

// layout(binding = 0, std430) buffer agent {
// 	float agentPos[];
// } agentBuffer;

layout(binding = 0, std430) buffer screenHeatmap {
	float screenHeatmap[];
} heatmapBuffer;


layout(location = 0) uniform float in_dt;
layout(location = 1) uniform float time;

uniform int screen_width;
uniform int screen_height;
uniform int agent_count;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

float M_PI = 3.14159265358979323846;
float speed = 0.1;

float SENSOR_OFFSET_DISTANCE = 0.03;
float SENSOR_SIZE = 5;
float SENSOR_ANGLE = 90;

uint mapGPUCoordToNormal(float coord, uint maxNorm) {
	return uint(((coord + 1) * maxNorm) / 2);
}

float sense(VertexData agent, float sensorAngleOffset) {
	float sensorAngle = agent.angle.x + sensorAngleOffset;
	vec2 sensorDir = vec2(cos(sensorAngle), sin(sensorAngle));

	vec2 sensorPos = agent.position.xy + sensorDir * SENSOR_OFFSET_DISTANCE;
	uint sensorCentreX = mapGPUCoordToNormal(sensorPos.x, screen_width);
	uint sensorCentreY = mapGPUCoordToNormal(sensorPos.y, screen_height);
	
	float sum = 0;

	for (float offsetX = -SENSOR_SIZE; offsetX <= SENSOR_SIZE; offsetX++) {
		for (float offsetY = -SENSOR_SIZE; offsetY <= SENSOR_SIZE; offsetY++) {
			uint sampleX = uint(sensorCentreX + offsetX);
			uint sampleY = uint(sensorCentreY + offsetY);

			sum += heatmapBuffer.screenHeatmap[sampleX + screen_width * sampleY];
		}
	}

	return sum;
}

float bounds = 0.99;

void main() {
	uint index = gl_GlobalInvocationID.x;

	VertexData agent = outBuffer.data[index];
	vec2 position = agent.position.xy;

	float randomNum = random(outBuffer.data[index].position.xy);

	float sensorAngleRad = SENSOR_ANGLE * (M_PI / 180);
	float weightForward = sense(agent, 0);
	float weightLeft = sense(agent, -sensorAngleRad);
	float weightRight = sense(agent, sensorAngleRad);

	float randomSteerStrength = randomNum;
	float turnSpeed = 50;

	// TODO: Delay sense time for 5 seconds
	if (time > 5) {
		// Continue in same direction
		if (weightForward > weightLeft && weightForward > weightRight) {
			outBuffer.data[index].angle.x += 0;
		}
		else if (weightForward < weightLeft && weightForward < weightRight) {
			outBuffer.data[index].angle.x += (randomSteerStrength - 0.5) * 2 * turnSpeed * in_dt;
		}
		// Turn right
		else if (weightRight > weightLeft) {
			outBuffer.data[index].angle.x -= randomSteerStrength * turnSpeed * in_dt;
		}
		// Turn left
		else if (weightLeft > weightRight) {
			outBuffer.data[index].angle.x += randomSteerStrength * turnSpeed * in_dt;
		}
	}

	float rotation = outBuffer.data[index].angle.x;

	vec2 delta = vec2(cos(rotation), sin(rotation)) * speed * in_dt;
	vec2 predicted = delta + position;

	if (predicted.x < -bounds || predicted.x > bounds || predicted.y < -bounds || predicted.y > bounds) {
		predicted = vec2(min(bounds, max(-bounds, predicted.x)), min(bounds, max(-bounds, predicted.y)));

		randomNum = random(outBuffer.data[index].position.xy);
		// outBuffer.data[index].angle.x += randomNum * 1000 * M_PI;
		outBuffer.data[index].angle.x += M_PI / 2;
		// outBuffer.data[index].angle.x *= -1;
	}

	outBuffer.data[index].position.xy = predicted;

	// agentBuffer.agentPos[index] = mapGPUCoordToNormal(predicted.x, screen_width);
	// agentBuffer.agentPos[index + agent_count] = screen_height - mapGPUCoordToNormal(predicted.y, screen_height);

	heatmapBuffer.screenHeatmap[mapGPUCoordToNormal(predicted.x, screen_width) + screen_width * mapGPUCoordToNormal(predicted.y, screen_height)] = 1;
}