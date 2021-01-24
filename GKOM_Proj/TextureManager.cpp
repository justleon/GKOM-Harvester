#include "headers/TextureManager.h"

TextureManager::~TextureManager()
{
	for (int i = 0; i < textures.size(); i++) {
		delete textures[i];
	}
}

void TextureManager::addTexture(std::string path)
{
	Texture* newTexture = new Texture();
	newTexture->path = path;
	glGenTextures(1, &newTexture->textureID);
	glBindTexture(GL_TEXTURE_2D, newTexture->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image = SOIL_load_image(path.c_str(), &newTexture->width,
		&newTexture->height, &newTexture->channels, SOIL_LOAD_RGBA);
	if (image != nullptr) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTexture->width, newTexture->height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		textures.push_back(newTexture);
	}
	else {
		std::cout << "Failed to load new texture." << std::endl;
	}

	SOIL_free_image_data(image);
}

GLuint TextureManager::getTextureID(std::string path)
{
	for (auto texture : textures) {
		if (texture->path == path) {
			return texture->textureID;
		}
	}
	return 0;
}

GLuint TextureManager::useTexture(std::string path)
{
	for (int i = 0; i < textures.size(); ++i) {
		if (textures[i]->path == path) {
			std::cout << textures[i]->textureID << std::endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[i]->textureID);
			return i;
		}
	}
	return 0;
}

GLuint TextureManager::useTexture(int index)
{
	if (index < textures.size() && index >= 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textures[index]->textureID);
		return index;
	}
	return 0;
}