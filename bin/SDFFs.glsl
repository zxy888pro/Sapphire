#version 330 core
in vec2 TexCoords;
out vec4 color;
//Ƭ����ɫ��������uniform������һ���ǵ���ɫͨ��������λͼ������һ������ɫuniform�����������������ı���������ɫ���������ȴ�λͼ�����в�����ɫֵ��
//�������������н��洢�ź�ɫ���������ǾͲ��������r��������Ϊȡ����alphaֵ��ͨ���任��ɫ��alphaֵ�����յ���ɫ�����α�����ɫ�ϻ���͸���ģ������������ַ��������ǲ�͸���ġ�����Ҳ��RGB��ɫ��textColor���uniform��ˣ����任�ı���ɫ��

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