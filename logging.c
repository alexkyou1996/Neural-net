#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <syslog.h>
#include <stdbool.h>

#include "logging.h"
#define BUFFER_SIZE 1024

//! Function to log errors
int log_error(const char *, const char *, int, char *, ...);
//! Function to log informational stuff
int log_info(const char *, const char *, int, char *, ...);
bool verbose = false;

//! Function to initiate logger
void init_log()
{
    setlogmask(LOG_UPTO(LOG_ERR));
    openlog("test_log", LOG_NDELAY | LOG_PID, LOG_LOCAL0);
}

//! Function to set logger to print to stdout instead of using syslog
void set_verbose()
{
    verbose = true;
}

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
int log_error(const char *file, const char * function, int line, char *format, ...)
{
    char buffer[BUFFER_SIZE] = {0};
    size_t log_length = 0;
    va_list args ={0};
    int ret = 0;

    snprintf(buffer, BUFFER_SIZE, "%s: In function (%s), line (%d): ", file, function, line);
    log_length = strlen(buffer);
    va_start(args, format);
    vsnprintf(buffer + log_length, BUFFER_SIZE - log_length, format, args);
    va_end(args);

    if (verbose) {
        printf("%s\n", buffer);
        return ret;
    }
    syslog(LOG_ERR,"%s\n", buffer);
    return ret;
}

//! Function to log informational stuff
/*
 * @params  const char *        The file name
 * @params  const char *        The function name
 * @params  int line            The line number
 * @params  char *              The format string
 * @params  ...                 Variadic arguments
 *
 * @returns int                 Number of characters logged. -1 on error
 */
int log_info(const char *file, const char * function, int line, char *format, ...)
{
    char buffer[BUFFER_SIZE] = {0};
    size_t log_length = 0;
    va_list args ={0};
    int ret = 0;

    snprintf(buffer, BUFFER_SIZE, "%s: In function (%s), line (%d): ", file, function, line);
    log_length = strlen(buffer);
    va_start(args, format);
    vsnprintf(buffer + log_length, BUFFER_SIZE - log_length, format, args);
    va_end(args);

    if (verbose) {
        printf("%s\n", buffer);
        return ret;
    }
    syslog(LOG_INFO,"%s\n", buffer);
    return ret;
}
