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
	GLuint textureID;
};

class TextureManager
{
private:
	std::vector<Texture*> textures;
public:
	TextureManager() {};
	~TextureManager();

	void addTexture(std::string path);
	GLuint getTextureID(std::string path);
	GLuint useTexture(std::string path);
	GLuint useTexture(int index);

};