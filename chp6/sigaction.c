#include"signal.h"
#include"unistd.h"

typedef void (*sighandler)(int);
sighandler my_signal(int signum, sighandler handler)
{
    struct sigaciton  act, oldact;

    act.sa_handler = handler;
    sigempty(&act.sa_mask);

    act.sa_flags = 0;
    if(signum == SIGALRM)
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    else{
#ifdef SA_RESTART
        act.sa_flag _|= SA_RESTART;
#endif
    }

    if(sigaction(signum, &act, &oldact) < 0)
        return (SIG_ERR);

    return oldact.sa_handler;
}
