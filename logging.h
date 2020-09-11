#define LOG_ERROR(format, ...) log_error(__FILE__, __func__, __LINE__, format, ## __VA_ARGS__) 

//! Function to initiate logger
void init_log();

//! Function to set logger to print to stdout instead of using syslog
void set_verbose();

//! Function to log errors
/*
 * @params  const char *        The file name
 * @params  const char *        The function name
 * @params  int line            The line number
 * @params  char *              The format string 
 * @params  ...                 Variadic arguments
 *
 * @returns int                 Number of characters logged. -1 on error
 */
int log_error(const char *, const char *, int, char *, ...);
