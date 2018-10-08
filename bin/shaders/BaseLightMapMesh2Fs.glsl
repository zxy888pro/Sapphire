#version 330 core



//����
struct Material {
    sampler2D diffuse;  //��������ͼ, ��ͨ��glUniform1i�趨����Ԫ
    sampler2D specular;      //�߹���ͼ  ��ͨ��glUniform1i�趨����Ԫ
    float shininess;     //�߹�ָ��
}; 

//�õ��ĵ��Դ����
#define POINT_LIGHTS_NUM 4
//�����Դ��������
#define POINT_LIGHTS_MAX_NUM 16

//���Դ����
struct PointLight 
{

    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

uniform vec3 objectColor;
//�۲��
uniform vec3 viewPos;

uniform int  pointLightCount;
uniform Material material;  //��������
//���Դ����
uniform PointLight pointLights[POINT_LIGHTS_MAX_NUM];


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//�����ɫ
out vec4 FragColor;

vec3  calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//������
    float ambientStrength = 0.2;
    vec3 ambient =  light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    vec3 diff = max(dot(normal, lightDir), 0.0) * texture(material.diffuse, TexCoords).rgb;

    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, normal);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
	return ambient + diff + specular;
}

void main()
{

	vec3 outCol;
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	////�������е��Դ����
	 for(int i = 0; i < pointLightCount; i++)
        outCol += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        
    vec3 result = outCol;

    FragColor = vec4(result,1.0);

}