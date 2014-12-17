#version 440 core
// Rendering Params Constants
const int MAX_NUM_LIGHTS = 4;
// Uniforms
struct Matrix
{
	mat4 modelViewProjection;
	mat4 modelView;
	mat4 model;
	mat4 view;
	mat4 projection;
};
struct Light 
{	
	vec3 position;
	vec3 color;
	float intensity;
	float attenuation;
	// Spot Light - Directional
	vec3 direction;
	float innerConeAngle;
	float outerConeAngle;
	// Control parameters
	int lightType;
};
uniform sampler2D diffuseMap;
uniform Matrix inputMatrices;
uniform Light light;
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoords;
layout(location = 2) in vec3 vertexNormal;
// Vertex shader ouput data


void main()
{
	vec4 vertexPos = vec4(vertexPosition, 1.0f);
	gl_Position = inputMatrices.modelViewProjection * vertexPos;
}