#pragma once

enum Level { INFO = 1 << 0, WARN = 1 << 1, ERROR = 1 << 2, FATAL = 1 << 3 };

void cell_log(enum Level l, const char *__restrict__ fmt, ...);

void cell_log_info(const char *__restrict__ fmt, ...);
void cell_log_warn(const char *__restrict__ fmt, ...);
void cell_log_error(const char *__restrict__ fmt, ...);
void cell_log_fatal(const char *__restrict__ fmt, ...);