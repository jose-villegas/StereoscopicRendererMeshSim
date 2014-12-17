#version 440 core
// Rendering Params Constants
//--include Constrains
// Structures 
struct Matrix
{
	mat4 modelViewProjection;
	mat4 modelView;
	mat4 model;
	mat4 view;
	mat4 projection;
	mat3 normal;
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
struct Material {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};
// Uniforms
uniform sampler2D diffuseMap;
uniform int lightsCount;
uniform Matrix inputMatrices;
uniform Material material;
uniform Light light[MAX_LIGHTS];
// Input vertex data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoords;
layout(location = 2) in vec3 vertexNormal;
// Vertex shader ouput data
out vec2 texCoord;
out vec3 normal;
out vec3 position;

void main()
{
	vec4 vertexPos = vec4(vertexPosition, 1.0f);

	texCoord = vertexTexCoords;
	normal = normalize(inputMatrices.normal * vertexNormal);
	position = vec3(inputMatrices.modelView * vertexPos);

	gl_Position = inputMatrices.modelViewProjection * vertexPos;
}