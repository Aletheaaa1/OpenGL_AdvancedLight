#pragma once

#include "Render.h"
#include <glad/glad.h>
#include <string>
#include <vector>
#include <iostream>

class Texture
{
private:
	int width, height, number_of_channel;
	unsigned char* data;
	unsigned int m_texture_buffer;
	unsigned int m_slot_id;
	bool gamma;
	bool flip;

public:
	Texture() = default;
	Texture(const char* image_file, bool gamma = false, bool flip = true);
	Texture(std::vector<std::string> cubemaps, bool gamma = false, bool flip = false);
	~Texture();

	void Bind();
	void Unbind();

	void Usetexture(unsigned int texture_slot);

	unsigned char* ReadImage(const char* image_file);

	unsigned int GetSlot();

	unsigned int GetTextureID();
};
