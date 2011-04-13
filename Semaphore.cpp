// Copyright(C) 2011 Ben Murrell

#include "Semaphore.hpp"

Semaphore::Semaphore
    ( 
    uint32 aCount
    )
{
    #if defined( _WIN32 )
        mSemaphore = NULL;
        mSemaphore = CreateSemaphore( NULL, 0, aCount, NULL );
    #else
        if( aCount != 1 )
        {
            assert( false );
        }
        else
        {
            pthread_mutex_init( &mMutex, NULL );
        }
    #endif
} // End of Semaphore::Semaphore()

Semaphore::~Semaphore()
{
    // Release all locks on the semaphore
    while( unlock() );

    #if defined( _WIN32 )
        CloseHandle( mSemaphore );
    #else
        pthread_mutex_destroy( &mMutex );
    #endif
} // End of Semaphore::~Semaphore()

bool Semaphore::lock
    (
    sint32 aTimeoutMs
    )
{
    #if defined( _WIN32 )
        DWORD waitTime = aTimeoutMs == -1 ? INFINITE : aTimeoutMs;
        return WaitForSingleObject( mSemaphore, waitTime ) == WAIT_TIMEOUT ? false : true;
    #else
        return 0 == pthread_mutex_lock( &mMutex ) ? true : false;
    #endif
} // End of Semaphore::lock()

bool Semaphore::unlock()
{
    #if defined( _WIN32 )
        return ReleaseSemaphore( mSemaphore, 1, NULL ) ? true : false;
    #else
        return 0 == pthread_mutex_unlock( &mMutex ) ? true : false;
    #endif
} // End of Semaphore::unlock()
