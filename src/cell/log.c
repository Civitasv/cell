#include "cell/log.h"
#include <stdarg.h>
#include <stdio.h>

static char *level_to_string(enum Level l)
{
	switch (l) {
	case INFO:
		return "[info]";
	case WARN:
		return "[warn]";
	case ERROR:
		return "[error]";
	case FATAL:
		return "[fatal]";
	default:
		return "[unknown]";
	}
}

void cell_log(enum Level l, const char *__restrict__ fmt, ...)
{
	printf("%s ", level_to_string(l));

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	printf("%c", '\n');
}

void cell_log_info(const char *__restrict__ fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	cell_log(INFO, fmt, args);
	va_end(args);
}

void cell_log_warn(const char *__restrict__ fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	cell_log(WARN, fmt, args);
	va_end(args);
}

void cell_log_error(const char *__restrict__ fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	cell_log(ERROR, fmt, args);
	va_end(args);
}

void cell_log_fatal(const char *__restrict__ fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	cell_log(FATAL, fmt, args);
	va_end(args);
}