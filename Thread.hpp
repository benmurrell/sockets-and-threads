// Copyright(C) 2011 Ben Murrell

#pragma once
#ifndef Thread_hpp
#define Thread_hpp

#include "types.hpp"

#if defined( _WIN32 )
    #include <Windows.h>
#else
    #include <pthread.h>
    #include "Semaphore.hpp"
#endif

#include <boost/function.hpp>

//-----------------------------------------------------------------------------
//! Thread abstraction class
//-----------------------------------------------------------------------------
class Thread
{
	//-------------------------------------------------------------------------
	// Public types
	//-------------------------------------------------------------------------
	public:
        typedef boost::function<void()> FunctionType;

	//-------------------------------------------------------------------------
	// Public functions
	//-------------------------------------------------------------------------
	public:
		Thread
			(
			FunctionType aFunction
			);

		virtual ~Thread();

		void start();

	//-------------------------------------------------------------------------
	// Private functions
	//-------------------------------------------------------------------------
	private:
        #if defined( _WIN32 )
		    static DWORD threadProc
			    ( 
			    void* aThis 
			    );
        #else
            static void* threadProc
                (
                void* aThis
                );
        #endif

	//-------------------------------------------------------------------------
	// Private variables
	//-------------------------------------------------------------------------
	private:
        FunctionType mFunction;	//!< The function to call
		
        #if defined( _WIN32 )
            HANDLE mThread;         //!< The windows thread handle
        #else
            pthread_t mThread;      //!< The pthread thread
            
            bool mStarted;
            Semaphore mSemaphore;   //!< Semaphore to prevent immediate start
        #endif
}; // End of class Thread

#endif