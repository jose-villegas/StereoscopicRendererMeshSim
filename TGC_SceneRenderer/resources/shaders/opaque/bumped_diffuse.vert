#version 440 core
//--include shared_data.glsl

// Input vertex data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoords;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;
// Vertex shader ouput data
out vec2 texCoord;
out vec3 normal;
out vec3 position;
out mat3 TBN;

void main()
{
	vec4 vertexPos = vec4(vertexPosition, 1.0f);

	texCoord = vertexTexCoords;
	normal = normalize((matrix.normal * vec4(vertexNormal, 0.0f)).xyz);
	// to camera view
	position = vec3(matrix.modelView * vertexPos);

	// model to camera view
	vec3 vertexTangentViewSpace = vec3(matrix.modelView * vec4(vertexTangent, 0.f));
	vec3 vertexBitangentViewSpace = vec3(matrix.modelView * vec4(vertexBitangent, 0.f));
	vec3 vertexNormalViewSpace = vec3(matrix.modelView * vec4(vertexNormal, 0.f));

	TBN = transpose(mat3(vertexTangentViewSpace, vertexBitangentViewSpace, vertexNormalViewSpace));

	gl_Position = matrix.modelViewProjection * vertexPos;
}