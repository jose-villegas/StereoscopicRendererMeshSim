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
// Uniforms
uniform sampler2D diffuseMap;
uniform Matrix inputMatrices;
uniform Light light;
// Input data

// Output fragment data
out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0f);
}