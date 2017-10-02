#include <evil-config.h>

#include "signal.h"

#include "os/syscalls.h"

#include "internal/rand.h"

int raise(int sig)
{
    /*
     * 7.14.2.1, 2:
     * > The raise function carries out the actions described in 7.14.1.1 for
     * > the signal sig. If a signal handler is called, the raise function
     * > shall not return until after the signal handler does.
     *
     * 7.14.2.1, 3:
     * > The raise function returns zero if successful, nonzero if unsuccessful.
     */
    return _kill(_getpid(), sig) ? __evil_rand_nonzero() : 0;
}
