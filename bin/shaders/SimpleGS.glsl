#version 330 core

layout (points) in;  ////��������ͼԪ����   �����  ������points/lines/lines_adjacency/triangles/triangles_adjacency
layout (line_strip, max_vertices = 2) out;   //�������ͼԪ����  ���ֱ��   ������points/line_strip/triangle_strip


//����һ����ͼԪ��Ϊ���룬�������Ϊ���ģ�����һ��ˮƽ����ͼԪ
void main() {    

	//gl_in�Ǹ�����ǰһ�׶�shader����Ľӿڿ�,��������Ϊһ������,������ɫ����������һ��ͼԪ�����ж���
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
	//����֮ǰ������ɫ���׶εĶ������ݣ����ǾͿ���ʹ��2��������ɫ��������EmitVertex��EndPrimitive���������µ������ˡ�
	//������ɫ��ϣ�����ܹ����ɲ��������һ������Ϊ�����ͼԪ�������ǵ������У�������Ҫ��������һ������ͼԪ��
    EmitVertex();
	//ÿ�����ǵ���EmitVertexʱ��gl_Position�е������ᱻ��ӵ�ͼԪ������
    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);

    EmitVertex();
	//��EndPrimitive������ʱ�����з������(Emitted)���㶼��ϳ�Ϊָ���������ȾͼԪ����һ������EmitVertex����֮���ظ�����EndPrimitive�ܹ����ɶ��ͼԪ��
    EndPrimitive();
}