#include "Render.h"

#include <glad\glad.h>
#include <iostream>
void gl_clear_error()
{
	while (glGetError() != 0);
}

bool gl_log_call(const char* function, const char* file, int line)
{
	while (auto error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
	}
	return false;
}