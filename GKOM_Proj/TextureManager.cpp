#include "headers/TextureManager.h"

void TextureManager::addTexture(std::string& path)
{
	Texture newTexture;
	newTexture.path = path;
	glGenTextures(1, &newTexture.textureID);
	glBindTexture(GL_TEXTURE_2D, newTexture.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image = SOIL_load_image(path.c_str(), &newTexture.width,
		&newTexture.height, &newTexture.channels, SOIL_LOAD_RGBA);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTexture.width, newTexture.height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load new texture." << std::endl;
	}

	SOIL_free_image_data(image);
}

unsigned int TextureManager::useTexture(std::string& path)
{
	for (int i = 0; i < this->textures.size(); ++i) {
		if (this->textures[i].path == path) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].textureID);
			return i;
		}
	}
	return 0;
}

unsigned int TextureManager::useTexture(int index)
{
	if (index < this->textures.size() && index >= 0) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, this->textures[index].textureID);
		return index;
	}
	return 0;
}