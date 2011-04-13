// Copyright(C) 2011 Ben Murrell

#pragma once
#ifndef Semaphore_hpp_
#define Semaphore_hpp_

#include "types.hpp"

#if defined( _WIN32 )
    #include <windows.h>
#else
    #include <pthread.h>
#endif

//-----------------------------------------------------------------------------
//! Semaphore abstraction class
//-----------------------------------------------------------------------------
class Semaphore
{
    Semaphore( const Semaphore& aRight );            //!< Thou shalt not copy
    Semaphore& operator=( const Semaphore& aRight ); //!< Thou shalt not assign
    
    //-------------------------------------------------------------------------
    // Public functions
    //-------------------------------------------------------------------------
    public:
        Semaphore
            ( 
            uint32 aCount = 1
            );

        virtual ~Semaphore();

        bool lock
            (
            sint32 aTimeoutMs = -1
            );

        bool unlock();

    //-------------------------------------------------------------------------
    // Private types
    //-------------------------------------------------------------------------
    private:
        #if defined( _WIN32 )
            HANDLE mSemaphore; //!< The windows semaphore handle
        #else
            pthread_mutex_t mMutex; //!< The posix mutex
        #endif

}; // End of class Semaphore

#endif
