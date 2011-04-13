// Copyright(C) 2011 Ben Murrell

#include "Thread.hpp"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
Thread::Thread
	(
	Thread::FunctionType aFunction //!< The function to call in the new thread
	)
	: mFunction( aFunction )
{
	// The function must be non-null
	assert( NULL != mFunction );

    #if defined( _WIN32 )
        mThread = NULL;

	    // Create the Windows thread handle
	    mThread = CreateThread
		    (
		    NULL,				// Thread security attributes
		    0,					// Stack size - 0 for inherit from process
		    (LPTHREAD_START_ROUTINE)Thread::threadProc, // Thread function
		    this,				// Parameter to thread function
		    CREATE_SUSPENDED,	// Creation flags - create in suspended thread
		    NULL				// Optional - out, thread id
		    );
    #else
        mStarted = false;
        mSemaphore.lock();
        pthread_create( &mThread, NULL, &Thread::threadProc, (void*)this );
    #endif

} // End of Thread::Thread()

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Thread::~Thread()
{
    #if defined( _WIN32 )
	    // Block until the thread is done running
	    WaitForSingleObject( mThread, INFINITE );
        
        CloseHandle( mThread );
        mThread = NULL;
    #else
        if( !mStarted )
        {
            mSemaphore.unlock();
        }
        else
        {
            pthread_join( mThread, NULL );
        }
    #endif

} // End of Thread::~Thread()

//-----------------------------------------------------------------------------
//! Signals the thread to start running
//-----------------------------------------------------------------------------
void Thread::start()
{
    #if defined( _WIN32 )
	    assert( NULL != mThread );

	    // We created the thread in suspended state - resume it
	    ResumeThread( mThread );

    #else
        mStarted = true;
        mSemaphore.unlock();
    #endif
} // End of Thread::start()

#if defined( _WIN32 )
    //-----------------------------------------------------------------------------
    //! Thread marshalling function for Windows thread
    //-----------------------------------------------------------------------------
    DWORD Thread::threadProc
	    ( 
	    void* aThis //!< Thread* of the calling thread
	    )
    {
	    // Cast the param into a Thread*
	    Thread* aThread = static_cast<Thread*>( aThis );

	    // Run the thread
	    aThread->mFunction();

        return 0;
    	
    } // End of Thread::threadProc()
#else
    void* Thread::threadProc
        (
        void* aThis
        )
    {
        Thread* aThread = static_cast<Thread*>( aThis );

        // Lock until we are told to start
        mSemaphore.lock();
        if( mStarted )
        {
            aThread->mFunction();
        }

        return NULL;
    } // End of Thread::threadProc()
#endif
