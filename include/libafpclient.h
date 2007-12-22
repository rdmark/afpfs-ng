
#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <unistd.h>
#include <syslog.h>

#define MAX_CLIENT_RESPONSE 2048


enum loglevels {
        AFPFSD,
};

struct afp_server;
struct afp_volume;

struct libafpclient {
        int (*unmount_volume) (struct afp_volume * volume);
	void (*log_for_client)(void * priv,
        	enum loglevels loglevel, int logtype, char *message, ...);
	void (*forced_ending_hook)(void);
	int (*scan_extra_fds)(int command_fd,fd_set *set, int * max_fd);
	void (*loop_started)(void);
} ;

extern struct libafpclient * libafpclient;

void client_setup(struct libafpclient * tmpclient);


void signal_main_thread(void);

/* These are logging functions */

#define MAXLOGSIZE 2048

#define LOG_METHOD_SYSLOG 1
#define LOG_METHOD_STDOUT 2

void set_log_method(int m);


void log_for_client(void * priv,
        enum loglevels loglevel, int logtype, char * message, ...);


void make_log_entry(enum loglevels loglevel, int logtype,
                    char *message, ...);

typedef void(*make_log_func)
       (enum loglevels loglevel, int logtype, char *message, ...);
make_log_func set_log_location(char *srcfilename, int srclinenumber);

void stdout_log_for_client(void * priv,
	enum loglevels loglevel, int logtype, char *message, ...);

#define LOG make_log_entry

#endif
