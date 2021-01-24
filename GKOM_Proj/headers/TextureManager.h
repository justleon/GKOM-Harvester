#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "ShaderProgram.h"
#include <vector>
#include <string>
#include <iostream>

struct Texture {
	std::string path;
	int width;
	int height;
	int channels;
	unsigned int textureID;
};

class TextureManager
{
private:
	std::vector<Texture> textures;
public:
	void addTexture(std::string& path);
	unsigned int useTexture(std::string& path);
	unsigned int useTexture(int index);

};