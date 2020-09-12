#define LOG_ERROR(format, ...) log_error(__FILE__, __func__, __LINE__, format, ## __VA_ARGS__)
#define LOG_LINE(format, ...) log_info(__FILE__, __func__, __LINE__, format, ## __VA_ARGS__)

//! Function to initiate logger
void init_log();

//! Function to set logger to print to stdout instead of using syslog
void set_verbose();

//! Function to log errors
int log_error(const char *, const char *, int, char *, ...);

//! Function to log informational stuff
int log_info(const char *, const char *, int, char *, ...);

