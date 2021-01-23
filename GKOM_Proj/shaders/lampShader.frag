#version 330 core
out vec4 FragColor;

uniform float intensity;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor * (intensity * 0.7f + 0.3f), 1.0f);
}