#version 440 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 tCor;

out vec2 tCord;
void main()
{
    gl_Position = vec4(vPos, 1.0);
    tCord = vec2(tCor.x, 1.0 - tCor.y);
}