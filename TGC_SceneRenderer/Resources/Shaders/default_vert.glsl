#version 440 core

uniform mat4 MVP;
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelSpace;

void main()
{
    // Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPositionModelSpace, 1); // Transform an homogeneous 4D vector, remember ?
    gl_Position = MVP * v;
}