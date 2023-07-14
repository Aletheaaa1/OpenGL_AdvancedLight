#pragma once

#define GLCall(x) \
	gl_clear_error(); \
	x;	\
	gl_log_call(#x, __FILE__, __LINE__); \

void gl_clear_error();
bool gl_log_call(const char* function, const char* file, int line);