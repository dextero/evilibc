#include <evil-config.h>

#include "stdlib.h"

#include "signal.h"

_Noreturn void abort(void)
{
    /*
     * 7.22.4.1, 2:
     * > The abort function causes abnormal program termination to occur,
     * > unless the signal SIGABRT is being caught and the signal handler
     * > does not return. Whether open streams with unwritten buffered data
     * > are flushed, open streams are closed, or temporary files are removed
     * > is implementation-defined. An implementation-defined form of the
     * > status unsuccessful termination is returned to the host environment
     * > by means of the function call raise(SIGABRT).
     */
    raise(SIGABRT);
    
    /*
     * 7.22.4.1, 3:
     * > The abort function does not return to its caller.
     */
    __builtin_unreachable();
}
