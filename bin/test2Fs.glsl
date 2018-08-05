#version 330 core

in vec2 texCoord;
uniform sampler2D myTexture1;


out vec4 texColor;

void main()
{
	texColor = texture(myTexture1, texCoord);
}
