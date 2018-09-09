#version 330 core
//输出颜色
out vec4 FragColor;

//材质
struct Material {
    sampler2D diffuse;  //漫反射贴图, 可通过glUniform1i设定纹理单元
    sampler2D specular;      //高光贴图  可通过glUniform1i设定纹理单元
    float shininess;     //高光指数
}; 

//灯光参数
struct Light {
    vec3 position;

    vec3 ambient;    //h环境光颜色
    vec3 diffuse;    //漫反射光颜色
    vec3 specular;      //高光颜色
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;    //光源位置
uniform Material material;  //材质属性
uniform Light light;  //灯光属性

void main()
{
    // 环境光用漫反射贴图颜色与灯光颜色乘积
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // 漫反射光 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // 高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);   //求反射方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 