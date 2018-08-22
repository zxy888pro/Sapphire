#version 330 core   //∂®“Â∞Ê±æ openGL 3.3 core profile

layout(location=0) in vec3 position;
layout(location=1) in vec3 color;
layout(location=2) in vec2 uv;

uniform mat4 transMat;

out vec2 texCoord;
out vec3 outColor;

void main()
{
	gl_Position = transMat * vec4(position.xyz, 1.0f);
	texCoord = uv;
	outColor = color;
}