#version 330 core



//材质
struct Material {
    sampler2D diffuse;  //漫反射贴图, 可通过glUniform1i设定纹理单元
    sampler2D specular;      //高光贴图  可通过glUniform1i设定纹理单元
    float shininess;     //高光指数
}; 

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
uniform Material material;  //材质属性
//点光源数组
uniform PointLight pointLights[POINT_LIGHTS_MAX_NUM];


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//输出颜色
out vec4 FragColor;

vec3  calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//环境光
    float ambientStrength = 0.2;
    vec3 ambient =  light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
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
	////计算所有点光源照明
	 for(int i = 0; i < pointLightCount; i++)
        outCol += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        
    vec3 result = outCol;

    FragColor = vec4(result,1.0);

}