#include "mymutexes.h"

#include <Arduino.h>
#include <ChibiOS_AVR.h>

// Mutexes
MUTEX_DECL(G_SPIMutex);

void lockSPI()
{
    chMtxLock(&G_SPIMutex);
}

void unlockSPI()
{
    chMtxUnlock();
}
