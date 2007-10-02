

#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "afpclient_log.h"

static int log_method=0;

void set_log_method(int m) 
{
	log_method=m;
}

void make_log_entry(enum loglevels loglevel, int logtype,
                    char *message, ...)
{

	va_list args;
	char temp_buffer[MAXLOGSIZE];
	int sysloglevel = LOG_ERR;
	/* Initialise the Messages */
	va_start(args, message);

	vsnprintf(temp_buffer, sizeof(temp_buffer), message, args);

	/* Finished with args for now */
	va_end(args);

	if (log_method & LOG_METHOD_SYSLOG) 
		syslog(sysloglevel, "%s", temp_buffer);
	if (log_method & LOG_METHOD_STDOUT) 
		printf("%s",temp_buffer);
}

void stdout_log_for_client(struct client * c,
        enum loglevels loglevel, int logtype, char *message, ...)
{
	va_list args;
	char toprint[1024];
	char new_message[1024];

	va_start(args, message);
	vsnprintf(new_message,1024,message,args);
	va_end(args);


	snprintf(toprint,1024, new_message);
	/* Finished with args for now */
	va_end(args);
	printf("%s\n",toprint);
}

