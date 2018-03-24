#version 440 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;
layout (location = 2) in vec3 vNorm;
layout (location = 3) in vec3 vTang;
layout (location = 4) in vec3 vBinor;

uniform mat4 P;
uniform mat4 MV;
uniform mat3 normalMatrix;

out vec2 TexCoords;
out vec3 WorldPos;
out mat3 TBN;//Normal;

void main()
{
    gl_Position = P * MV * vec4(vPos, 1.0);
	
	TexCoords = vec2(vTex.x, 1.0 - vTex.y);
	WorldPos = (MV * vec4(vPos, 1.0)).xyz;
	
	vec3 T = normalize(vec3(MV * vec4(vTang, 0.0)));
	vec3 B = normalize(vec3(MV * vec4(vBinor, 0.0)));
	vec3 N = normalize(vec3(MV * vec4(vNorm, 0.0)));
	TBN = mat3(T, B, N);
	
	//Normal = normalMatrix * vNorm;
}