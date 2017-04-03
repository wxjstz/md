#ifndef __CRITICALSECTION_H__
#define __CRITICALSECTION_H__

#ifdef __unix__

#include <pthread.h>
typedef pthread_mutex_t CS;
#define CS_INIT(m)      pthread_mutex_init(m,NULL)
#define CS_RELEASE(m)   pthread_mutex_destroy(m)
#define CS_ENTRY(m)     pthread_mutex_lock(m)
#define CS_LEAVE(m)     pthread_mutex_unlock(m)

#else

#include <windows.h>
typedef CRITICAL_SECTION CS;
#define CS_INIT(m)      InitializeCriticalSection(m,NULL)
#define CS_RELEASE(m)   DeleteCriticalSection(m)
#define CS_ENTRY(m)     EnterCriticalSection(m)
#define CS_LEAVE(m)     LeaveCriticalSection(m)

#endif //__unix__


#endif //__CRITICALSECTION_H__



