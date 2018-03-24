#version 440 core

in vec2 tCord;

layout(location = 0)out vec4 o_fragColor;
uniform sampler2D gSampler;

void main()
{
     o_fragColor = texture(gSampler, tCord);
}