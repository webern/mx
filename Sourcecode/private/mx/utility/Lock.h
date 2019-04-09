// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

// comment this out if you definitely
// will not have multiple threads and
// you don't care about const functions
// being thread safe
#define MX_CONST_THREAD_SAFE

#ifdef MX_CONST_THREAD_SAFE

    #include <mutex>
    #define MX_MUTEX mutable std::mutex myMutex;
    #define MX_LOCK std::lock_guard<std::mutex> mxLocalLockObject( myMutex );

#else

    #define MX_MUTEX
    #define MX_LOCK

#endif
