#version 330 core

//����
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

//���������
struct DirLight {
	//����
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
//���Դ����
struct PointLight {

    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
//�õ��ĵ��Դ����
#define POINT_LIGHTS_NUM 4
//�����Դ��������
#define POINT_LIGHTS_MAX_NUM 16

//�۹������
struct SpotLight {

    vec3 position;
    vec3 direction;
	//���н�����
    float cutOff;
	//���н�����
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

//�����еķ���⣬һ���һ��
uniform DirLight dirLight;
//���Դ����
uniform PointLight pointLights[POINT_LIGHTS_MAX_NUM];
//�۹��
uniform SpotLight spotLight;
//����
uniform Material material;


//����������
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//���
out vec4 FragColor;

vec3  calcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//���ȶ�light.direction����ȡ��������Ŀǰʹ�õĹ��ռ�������һ����Ƭ������Դ�Ĺ��߷��򣬵����Ǹ�ϰ�߶��嶨���Ϊһ���ӹ�Դ������ȫ�ַ���
	vec3 lightDir = normalize(-light.direction);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3  calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    //��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    //�������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //����˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // �۹��ǿ��
    float theta = dot(lightDir, normalize(-light.direction));   //LightDir������SpotDir����֮��ļн�
    float epsilon = light.cutOff - light.outerCutOff;  //���нǺ����н�֮�������ֵ��
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  //������н������нǵ�����˥��
    //�ϲ����
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	//����˥�� �������ټ��Ͼ۹��˥��
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
	 vec3 norm = normalize(Normal);
	 vec3 viewDir = normalize(viewPos - FragPos);
	// ����һ�������ɫֵ
	vec3 outCol;
	//���㶨�������
	outCol += calcDirectionalLight();
	//�������е��Դ����
	 for(int i = 0; i < POINT_LIGHTS_NUM; i++)
        outCol += calcPointLight(pointLights[i], norm, FragPos, viewDir); 
	//����۹������
	outCol += calcSpotLight(spotLight, norm, FragPos, viewDir);
	FragColor = vec4(output,1.0);

}