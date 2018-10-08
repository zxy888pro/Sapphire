#version 330 core

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
//���Դ����
uniform PointLight pointLights[POINT_LIGHTS_MAX_NUM];


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3  calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * light.ambient;
	vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);

    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, normal);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
	return ambient + diff + spec;
}

void main()
{

	vec3 outCol;
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	////�������е��Դ����
	 for(int i = 0; i < pointLightCount; i++)
        outCol += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        
    vec3 result = objectColor * outCol;

    FragColor = vec4(result,1.0);

}