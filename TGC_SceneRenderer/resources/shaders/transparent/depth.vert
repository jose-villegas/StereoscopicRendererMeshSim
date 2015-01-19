#version 440 core

//--include shared_data.glsl

// Input vertex data
layout(location = 0) in vec3 vertexPosition;

void main() {
	gl_Position = matrix.modelViewProjection * vec4(vertexPosition, 1.0f);
}