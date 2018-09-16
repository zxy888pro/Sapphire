#version 330 core

//输出
out vec4 FragColor;

//用到的点光源数量
#define POINT_LIGHTS_NUM 4
//最大点光源数组数量
#define POINT_LIGHTS_MAX_NUM 16

//材质
struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

//方向光属性
struct DirLight
{
	//方向
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
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


//聚光灯属性
struct SpotLight 
{

    vec3 position;
    vec3 direction;
	//内切角余弦
    float cutOff;
	//外切角余弦
    float outerCutOff;
    //kc
    float constant;
	//kl
    float linear;
	//kq
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

//输入流属性
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//观察点
uniform vec3 viewPos;
//场景中的方向光，一般就一个
uniform DirLight dirLight;
//点光源数组
uniform PointLight pointLights[POINT_LIGHTS_MAX_NUM];
//聚光灯
uniform SpotLight spotLight;
//材质
uniform Material material;

uniform int  pointLightCount;


vec3  calcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//首先对light.direction向量取反。我们目前使用的光照计算需求一个从片段至光源的光线方向，但人们更习惯定义定向光为一个从光源出发的全局方向
	vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3  calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  = ambient * attenuation;
    diffuse  = diffuse * attenuation;
    specular = specular * attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    //漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    //镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //距离衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 聚光灯强度
    float theta = dot(lightDir, normalize(-light.direction));   //LightDir向量和SpotDir向量之间的夹角
    float epsilon = light.cutOff - light.outerCutOff;  //内切角和外切角之间的余弦值差
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  //计算光切角与外切角的线性衰减
    //合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	//距离衰减 基础上再加上聚光灯衰减
    ambient  = ambient * attenuation;
    diffuse  = diffuse * attenuation;
    specular = specular * attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
	 vec3 norm = normalize(Normal);
	 vec3 viewDir = normalize(viewPos - FragPos);
	// 定义一个输出颜色值
	vec3 outCol;
	 outCol = vec3(texture(material.diffuse, TexCoords));
	////计算定向光照明
	outCol = calcDirectionalLight(dirLight, norm, viewDir);
	////计算所有点光源照明
	 for(int i = 0; i < pointLightCount; i++)
        outCol += calcPointLight(pointLights[i], norm, FragPos, viewDir); 
	////计算聚光灯照明
	outCol += calcSpotLight(spotLight, norm, FragPos, viewDir);
	outCol += vec3(0,0,0);
   
	FragColor = vec4(outCol,1.0);

}