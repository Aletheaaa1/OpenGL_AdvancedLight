#include "Texture.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* image, bool gamma, bool flip) :gamma(gamma), flip(flip)
{
	GLCall(glGenTextures(1, &m_texture_buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture_buffer));
	data = ReadImage(image);

	const char* hdr = &image[std::strlen(image) - 3];
	int is_hdr = strcmp(hdr, "hdr");

	unsigned int internal_format, format, type;

	type = GL_UNSIGNED_BYTE;
	if (number_of_channel == 1)
	{
		internal_format = GL_RED;
		format = GL_RED;
	}
	else if (number_of_channel == 3)
	{
		if (gamma == true)
		{
			internal_format = GL_SRGB;
		}
		else
		{
			internal_format = GL_RGB;
		}
		format = GL_RGB;
	}
	else if (number_of_channel == 4)
	{
		if (gamma == true)
		{
			internal_format = GL_SRGB_ALPHA;
		}
		else
		{
			internal_format = GL_RGBA;
		}
		format = GL_RGBA;
	}

	if (is_hdr == 0)
	{
		format = GL_RGB;
		internal_format = GL_RGB16F;
		type = GL_FLOAT;
	}

	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << image << " not exist!" << std::endl;
	}
	stbi_image_free(data);
}

Texture::Texture(std::vector<std::string> cubemaps, bool gamma, bool flip) : gamma(gamma), flip(flip)
{
	for (unsigned int i = 0; i < cubemaps.size(); i++)
	{
		data = nullptr;
		data = ReadImage(cubemaps[i].c_str());

		unsigned int  format, internal_format;
		if (number_of_channel == 1)
		{
			internal_format = GL_RED;
			format = GL_RED;
		}
		else if (number_of_channel == 3)
		{
			if (gamma == true)
			{
				internal_format = GL_SRGB;
			}
			else
			{
				internal_format = GL_RGB;
			}
			format = GL_RGB;
		}
		else if (number_of_channel == 4)
		{
			if (gamma == true)
			{
				internal_format = GL_SRGB_ALPHA;
			}
			else
			{
				internal_format = GL_RGBA;
			}
			format = GL_RGBA;
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		else
		{
			std::cout << cubemaps[i] + "skybox error \n";
		}
		stbi_image_free(data);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_texture_buffer));
}

void Texture::Bind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture_buffer));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Usetexture(unsigned int texture_slot)
{
	m_slot_id = texture_slot + GL_TEXTURE0;
	GLCall(glActiveTexture(m_slot_id));
}

unsigned char* Texture::ReadImage(const char* image_file)
{
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(image_file, &width, &height, &number_of_channel, 0);
	return data;
}

unsigned int Texture::GetSlot()
{
	return m_slot_id;
}

unsigned int Texture::GetTextureID()
{
	return this->m_texture_buffer;
}