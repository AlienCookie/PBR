#include "stdafx.h"
#include "Texture.h"
#include "lodepng.h"
#include <iostream>

GLuint Texture::loadTexturePNG(const char* filename)
{
	GLuint texture;  //variable for texture
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error) {
		std::cerr << "Decoder error " << error << ": "
			<< lodepng_error_text(error) << std::endl;
		return -1;
	}

	//allocate the memory for texture
	glGenTextures(1, &texture);

	//Binding the texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// texture should tile
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/*for (int i = 0; i < 100000; i++)
		image[i] = rand() % 254;*/
	
	// build our texture mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 
				0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinding the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint Texture::loadTextureCubemap(std::vector<const char*> faces)
{

	GLuint textureID;
	//glActiveTexture(GL_TEXTURE0);

	unsigned width, height;
	std::vector<unsigned char> image;

	glGenTextures(1, &textureID);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		unsigned error = lodepng::decode(image, width, height, faces[i]);

		//if there's an error, display it
		if (error) {
			std::cerr << "Decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
			return -1;
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]
			);
		image.clear();
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}