#include "Log.h"



Log::Log()
{
}

bool Log::restart_gl_log() {
	FILE* file = fopen(GL_LOG_FILE, "w");
	if (!file) {
		fprintf(stderr, "Can't open log file");
		return false;
	}

	time_t now = time(NULL);
	char* date = ctime(&now);

	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return true;
}

bool Log::gl_log(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "Error opening log file");
		return false;
	}
	va_start(argptr, message);

	vfprintf(file, message, argptr);
	vprintf(message, argptr);
	va_end(argptr);
	fclose(file);
	
	//cout << message, argptr;
	return true;
}

bool Log::gl_log_err(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	vprintf(message, argptr);
	va_end(argptr);
	fclose(file);
	
	return true;
}


Log::~Log()
{
}
