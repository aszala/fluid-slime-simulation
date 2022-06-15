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

uniform int screen_width;
uniform int screen_height;
uniform int agent_count;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

float M_PI = 3.14159265358979323846;
float speed = 0.6;

float SENSOR_OFFSET_DISTANCE = 0.01;
float SENSOR_SIZE = 20;
float SENSOR_ANGLE = 45;

uint mapGPUCoordToNormal(float coord, uint maxNorm) {
	return uint(((coord + 1) * maxNorm) / 2);
}

float sense(VertexData agent, float sensorAngleOffset) {
	float sensorAngle = agent.angle.x + sensorAngleOffset;
	vec2 sensorDir = vec2(cos(sensorAngle), sin(sensorAngle));

	vec2 sensorPos = agent.position.xy + sensorDir * SENSOR_OFFSET_DISTANCE;
	uint sensorCentreX = mapGPUCoordToNormal(sensorPos.x, screen_width);
	uint sensorCentreY = screen_height-mapGPUCoordToNormal(sensorPos.y, screen_height);

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

void main() {
	uint index = gl_GlobalInvocationID.x;

	VertexData agent = outBuffer.data[index];
	vec2 position = agent.position.xy;

	float randomNum = random(position.y * screen_width + position.x + random(index + in_dt * 100000));

	float sensorAngleRad = SENSOR_ANGLE * (M_PI / 180);
	// float weightForward = sense(agent, 0);
	// float weightLeft = sense(agent, sensorAngleRad);
	// float weightRight = sense(agent, -sensorAngleRad);

	float randomSteerStrength = randomNum;
	float turnSpeed = speed * 2 * M_PI;

	// // Continue in same direction
	// if (weightForward > weightLeft && weightForward > weightRight) {
	// 	outBuffer.data[index].angle.x += 0;
	// }
	// else if (weightForward < weightLeft && weightForward < weightRight) {
	// 	outBuffer.data[index].angle.x += (randomSteerStrength - 0.5) * 2 * turnSpeed * in_dt;
	// }
	// // Turn right
	// else if (weightRight > weightLeft) {
	// 	outBuffer.data[index].angle.x -= randomSteerStrength * turnSpeed * in_dt;
	// }
	// // Turn left
	// else if (weightLeft > weightRight) {
	// 	outBuffer.data[index].angle.x += randomSteerStrength * turnSpeed * in_dt;
	// }

	float rotation = outBuffer.data[index].angle.x;

	vec2 delta = vec2(cos(rotation), sin(rotation)) * speed * in_dt;
	vec2 predicted = delta + position;

	if (predicted.x < -1 || predicted.x > 1 || predicted.y < -1 || predicted.y > 1) {
		predicted = vec2(min(1, max(-1, predicted.x)), min(1, max(-1, predicted.y)));

		outBuffer.data[index].angle.x = randomNum * M_PI * 2;
	}

	outBuffer.data[index].position.xy = predicted;

	// agentBuffer.agentPos[index] = mapGPUCoordToNormal(predicted.x, screen_width);
	// agentBuffer.agentPos[index + agent_count] = screen_height - mapGPUCoordToNormal(predicted.y, screen_height);

	heatmapBuffer.screenHeatmap[mapGPUCoordToNormal(predicted.x, screen_width) + screen_width * 
								(screen_height - mapGPUCoordToNormal(predicted.y, screen_height))] = 1;
}