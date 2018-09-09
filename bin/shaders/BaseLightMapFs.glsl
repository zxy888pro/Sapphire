#version 330 core
//�����ɫ
out vec4 FragColor;

//����
struct Material {
    sampler2D diffuse;  //��������ͼ, ��ͨ��glUniform1i�趨����Ԫ
    sampler2D specular;      //�߹���ͼ  ��ͨ��glUniform1i�趨����Ԫ
    float shininess;     //�߹�ָ��
}; 

//�ƹ����
struct Light {
    vec3 position;

    vec3 ambient;    //h��������ɫ
    vec3 diffuse;    //���������ɫ
    vec3 specular;      //�߹���ɫ
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;    //��Դλ��
uniform Material material;  //��������
uniform Light light;  //�ƹ�����

void main()
{
    // ����������������ͼ��ɫ��ƹ���ɫ�˻�
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // ������� 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // �߹�
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);   //���䷽��
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 