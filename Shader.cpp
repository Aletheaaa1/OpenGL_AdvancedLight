#include "Shader.h"

Shader::Shader(const std::string& vs_file, const std::string& fs_file, const std::string& gs_file)
	:vs_file(vs_file), fs_file(fs_file), gs_file(gs_file)
{
	std::string vertex_shader = ReadShaderFiles(this->vs_file);
	std::string fragment_shader = ReadShaderFiles(this->fs_file);
	std::string geometry_shader = "";
	if (this->gs_file != "")
	{
		geometry_shader = ReadShaderFiles(this->gs_file);
	}
	program = CreateProgram(vertex_shader, fragment_shader, geometry_shader);
}

void Shader::Bind()
{
	//std::cout << program << std::endl;
	GLCall(glUseProgram(program));
}

unsigned int Shader::CreateProgram(const std::string& vertex_source, const std::string& fragment_source, const std::string& geometry_source)
{
	program = glCreateProgram();
	unsigned int vs = CompileShader(vertex_source, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragment_source, GL_FRAGMENT_SHADER);
	unsigned int gs;
	if (geometry_source != "")
	{
		gs = CompileShader(geometry_source, GL_GEOMETRY_SHADER);
		GLCall(glAttachShader(program, gs));
	}

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	if (geometry_source != "")
	{
		GLCall(glDeleteShader(gs));
	}

	return program;
}

unsigned int Shader::CompileShader(const std::string& shader_name, unsigned int type)
{
	unsigned int shader_id = glCreateShader(type);
	const char* shader_name_c = shader_name.c_str();
	GLCall(glShaderSource(shader_id, 1, &shader_name_c, nullptr));
	GLCall(glCompileShader(shader_id));

	int status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	while (status == 0)
	{
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		char* error = new char[length];
		glGetShaderInfoLog(shader_id, length, &length, error);
		std::cout << error << std::endl;
		delete[] error;
		return -1;
	}
	return shader_id;
}

std::string Shader::ReadShaderFiles(const std::string& shader_file)
{
	std::string line;
	std::ifstream file(shader_file);
	std::stringstream ss;

	if (!file.good())
	{
		std::cout << shader_file << " is not exist! \n";
	}
	while (std::getline(file, line))
	{
		ss << line << "\n";
	}
	return ss.str();
}

void Shader::SetUniform4f(const std::string& name, const float* values)
{
	int location = GetLocation(name);
	GLCall(glUniform4f(location, values[0], values[1], values[2], values[3]));
}

void Shader::SetUniform3v(const std::string& name, const glm::vec3 values)
{
	int location = GetLocation(name);
	float* value = const_cast<float*>(glm::value_ptr(values));
	glUniform3f(location, value[0], value[1], value[2]);
}

void Shader::SetUniform3f(const std::string& name, const float* values)
{
	int location = GetLocation(name);
	GLCall(glUniform3f(location, values[0], values[1], values[2]));
}

void Shader::SetUniform3f(const std::string& name, std::initializer_list<float> values)
{
	int location = GetLocation(name);
	float val[3];
	int index = 0;
	for (const auto& i : values)
	{
		val[index] = i;
		index++;
	}
	glUniform3f(location, val[0], val[1], val[2]);
}

void Shader::SetUniform1i(const std::string& name, const int value)
{
	int location = GetLocation(name);
	GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, const float value)
{
	int location = GetLocation(name);
	GLCall(glUniform1f(location, value));
}

void Shader::SetUniformMat4(const std::string& name, const float* values)
{
	int location = GetLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, false, values));
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 values)
{
	int location = GetLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, false, glm::value_ptr(values)));
}

void Shader::SetUniformMateria(const std::string& name, Material& material)
{
	std::string ambient = ".ambient";
	std::string diffuse = ".diffuse";
	std::string specular = ".specular";
	std::string shininess = ".shininess";

	int ambient_location = GetLocation(name + ambient);
	int diffuse_location = GetLocation(name + diffuse);
	int specular_loaction = GetLocation(name + specular);
	int shininess_location = GetLocation(name + shininess);

	const char* texture_symbol = "unsigned int";

	GLCall(glUniform3f(ambient_location, material.GetAmbient()[0], material.GetAmbient()[1], material.GetAmbient()[2]));
	GLCall(glUniform3f(diffuse_location, material.GetDiffuse()[0], material.GetDiffuse()[1], material.GetDiffuse()[2]));
	// GLCall(glUniform1i(diffuse_location, 0));
	GLCall(glUniform3f(specular_loaction, material.GetSpecular()[0], material.GetSpecular()[1],
		material.GetSpecular()[2]));
	GLCall(glUniform1f(shininess_location, material.GetShininess()));
}

void Shader::SetUniformMateriaWithMaterial(const std::string& name, Material& material)
{
	std::string ambient = ".ambient";
	std::string diffuse = ".diffuse";
	std::string specular = ".specular";
	std::string shininess = ".shininess";

	int ambient_location = GetLocation(name + ambient);
	int diffuse_location = GetLocation(name + diffuse);
	int specular_loaction = GetLocation(name + specular);
	int shininess_location = GetLocation(name + shininess);

	const char* texture_symbol = "unsigned int";

	GLCall(glUniform3f(ambient_location, material.GetAmbient()[0], material.GetAmbient()[1], material.GetAmbient()[2]));
	GLCall(glUniform1i(diffuse_location, material.GetMaterialDiffuse()));
	GLCall(glUniform1i(specular_loaction, material.GetMaterialSpecular()));
	GLCall(glUniform1f(shininess_location, material.GetShininess()));
}

int Shader::GetLocation(const std::string& uniform_name)
{
	const char* name = uniform_name.c_str();
	return glGetUniformLocation(program, name);
}