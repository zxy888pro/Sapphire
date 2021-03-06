#version 330 core
in vec2 TexCoords;
out vec4 color;
//片段着色器有两个uniform变量：一个是单颜色通道的字形位图纹理，另一个是颜色uniform，它可以用来调整文本的最终颜色。我们首先从位图纹理中采样颜色值，
//由于纹理数据中仅存储着红色分量，我们就采样纹理的r分量来作为取样的alpha值。通过变换颜色的alpha值，最终的颜色在字形背景颜色上会是透明的，而在真正的字符像素上是不透明的。我们也将RGB颜色与textColor这个uniform相乘，来变换文本颜色。

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = texture(text, TexCoords);
	float d = sampled.r;
	if( d >0.5)
	{
		color = vec4(1.0,1.0,1.0,1.0);
	}else
	{
		color = vec4(0,0,0,0.0);
	}
	
}