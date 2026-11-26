#version 330 core

layout (location = 0) in vec2 oPosition;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position =  projectionMatrix * modelMatrix * vec4(oPosition, 1.0, 1.0);
}