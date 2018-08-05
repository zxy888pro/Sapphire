#version 330 core
in vec2 TexCoords;
out vec4 color;
//Ƭ����ɫ��������uniform������һ���ǵ���ɫͨ��������λͼ������һ������ɫuniform�����������������ı���������ɫ���������ȴ�λͼ�����в�����ɫֵ��
//�������������н��洢�ź�ɫ���������ǾͲ��������r��������Ϊȡ����alphaֵ��ͨ���任��ɫ��alphaֵ�����յ���ɫ�����α�����ɫ�ϻ���͸���ģ������������ַ��������ǲ�͸���ġ�����Ҳ��RGB��ɫ��textColor���uniform��ˣ����任�ı���ɫ��

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}