#version 440 core

uniform mat4 MVP;
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelSpace;

void main()
{
    ttt = vec3(x.y);
    asd = 1.0f;
    // Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPositionModelSpace, 1) * asd; // Transform an homogeneous 4D vector, remember ?
    gl_Position = MVP * v;
}