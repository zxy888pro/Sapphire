#version 330 core
//声明输入图元类型
layout (points) in;  //输入点
layout (line_strip, max_vertices = 2) out;   //输出直线

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}