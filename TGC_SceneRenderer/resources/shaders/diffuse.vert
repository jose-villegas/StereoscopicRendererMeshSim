#version 440 core
uniform mat4 MVP;
uniform sampler2D normalMap;
uniform sampler2D diffuseMap;

out vec4 color;
in vec2 UV;

void main()
{
    color = texture(diffuseMap, UV).rgba;
}