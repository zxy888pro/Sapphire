#version 330 core

in vec2 texCoord;
in vec3 outColor;
uniform sampler2D myTexture1;


out vec4 texColor;

void main()
{
	texColor = texture(myTexture1, texCoord);
	//texColor = vec4(outColor,1.0f);
}
