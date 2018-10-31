#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - cameraPos); //入射向量
    vec3 R = reflect(I, normalize(Normal));  //反射向量
    FragColor = vec4(texture(skybox, R).rgb, 1.0);  //通过反射向量提取Cubemap纹理
}