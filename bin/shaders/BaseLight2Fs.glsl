#version 330 core

//用到的点光源数量
#define POINT_LIGHTS_NUM 4
//最大点光源数组数量
#define POINT_LIGHTS_MAX_NUM 16

//点光源属性
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
//观察点
uniform vec3 viewPos;

uniform int  pointLightCount;
//点光源数组
uniform PointLight pointLights[POINT_LIGHTS_MAX_NUM];


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3  calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * light.ambient;
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
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
	////计算所有点光源照明
	 for(int i = 0; i < pointLightCount; i++)
        outCol += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        
    vec3 result = objectColor * outCol;

    FragColor = vec4(result,1.0);

}