#include<sys/type.h>
#include<sys/socket.h>
#include<error.c>

#define MAXLINE 4096

/* Ptint message and return to caller
 * Caller specifies "errnoflag" and "level" */
/*static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
  int errno_save, n;
  char buf[MAXLINE + 1];

  errno_save = errno;                          //value caller might want printed
#ifdef HAVE_VSNPRINTF
  vsnprintf(buf, MAXLINE, fmt, ap);            //safe
#else
  vsprintf(buf, fmt, ap);                      //not safe
#endif
  n = strlen(buf);
  if (errnoflag)
    snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
  strcat(buf, "\n");

  if (daemon_proc) {
    syslog();
  }
}
*/
/* Fatal error related to system call
 * Print message and terminate*/
/*void err_sys(const char *fmt, ...) 
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(1);
}
*/

int Socket(int family, int type, int protocol)
{
  int n ;
  if ((n = socket(family, type, protocol)) < 0) {
    err_sys("socket error");                       //err_sys is in error.c
  }
 return(n);
}
