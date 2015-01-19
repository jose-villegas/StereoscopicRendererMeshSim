#version 440 core

//--include shared_data.glsl

// Input vertex data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoords;
layout(location = 2) in vec3 vertexNormal;
// Vertex shader ouput data
out vec2 texCoord;
out vec3 normal;
out vec3 position;
// shadowing
out vec4 shadowCoord[MAX_SHADOWS];

//--include shared_functions.glsl

void main()
{
	vec4 vertexPos = vec4(vertexPosition, 1.0f);

	texCoord = vertexTexCoords;
	normal = normalize((matrix.normal * vec4(vertexNormal, 0.0f)).xyz);
	position = vec3(matrix.modelView * vertexPos);

	if(shadowing.enabled > 0) {
		for(int i = 0; i < shadowing.shadowCount; i++) {
			shadowCoord[i] = shadowing.source[i].biasModelViewProjection * vec4(vertexPos);
		}
	}

	gl_Position = matrix.modelViewProjection * vertexPos;
}