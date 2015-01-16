#version 440 core
//--include shared_data.glsl
// Uniforms
uniform sampler2D diffuseMap;
//uniform Light light[MAX_LIGHTS];
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
	normal = normalize((matrix.normal * vec4(vertexNormal, 0.0f)).xyz);
	position = vec3(matrix.modelView * vertexPos);

	gl_Position = matrix.modelViewProjection * vertexPos;
}