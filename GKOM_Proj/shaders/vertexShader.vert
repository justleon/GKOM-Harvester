#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 vecNormals;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    vecNormals = normals;
    TexCoord = texCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0f);
} 