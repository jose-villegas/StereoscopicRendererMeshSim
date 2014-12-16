#version 440 core

uniform mat4 MVP;
uniform sampler2D normalMap;
uniform sampler2D diffuseMap;
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelSpace;
layout(location = 1) in vec2 vertexUV;
out vec2 UV;

void main()
{
    // Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPositionModelSpace, 1); // Transform an homogeneous 4D vector
    UV = vertexUV;
    gl_Position = MVP * v;
}