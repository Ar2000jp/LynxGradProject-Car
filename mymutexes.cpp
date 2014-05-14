#include "mymutexes.h"

#include <ChibiOS_AVR.h>

// Mutexes
MUTEX_DECL(G_SPIMutex);
MUTEX_DECL(G_I2CMutex);

void lockSPI()
{
//     chMtxLock(&G_SPIMutex);
}

void unlockSPI()
{
//     chMtxUnlock();
}

void lockI2C()
{
//     chMtxLock(&G_I2CMutex);
}

void unlockI2C()
{
//     chMtxUnlock();
}
