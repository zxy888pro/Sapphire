#version 330 core

layout (points) in;  ////声明输入图元类型   输入点  可以是points/lines/lines_adjacency/triangles/triangles_adjacency
layout (line_strip, max_vertices = 2) out;   //声明输出图元类型  输出直线   可以是points/line_strip/triangle_strip


//接受一个点图元作为输入，以这个点为中心，创建一条水平的线图元
void main() {    

	//gl_in是个来自前一阶段shader输出的接口块,它被声明为一个数组,几何着色器的输入是一个图元的所有顶点
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
	//有了之前顶点着色器阶段的顶点数据，我们就可以使用2个几何着色器函数，EmitVertex和EndPrimitive，来生成新的数据了。
	//几何着色器希望你能够生成并输出至少一个定义为输出的图元。在我们的例子中，我们需要至少生成一个线条图元。
    EmitVertex();
	//每次我们调用EmitVertex时，gl_Position中的向量会被添加到图元中来。
    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);

    EmitVertex();
	//当EndPrimitive被调用时，所有发射出的(Emitted)顶点都会合成为指定的输出渲染图元。在一个或多个EmitVertex调用之后重复调用EndPrimitive能够生成多个图元。
    EndPrimitive();
}