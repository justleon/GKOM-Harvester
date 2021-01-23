#version 330 core

struct Material {
    sampler2D tex;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 vecNormals;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 color;

void main()
{
    vec3 ambient = light.ambient * texture(material.tex, TexCoord).rgb;
    
    vec3 norm = normalize(vecNormals);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(vecNormals, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.tex, TexCoord).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular); 

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
