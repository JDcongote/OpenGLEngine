#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#define GL_LOG_FILE "gl.log"

using namespace std;

static class Log
{
public:
	Log();
	bool restart_gl_log();
	bool gl_log(const char * message, ...);
	bool gl_log_err(const char * message, ...);
	~Log();
};

