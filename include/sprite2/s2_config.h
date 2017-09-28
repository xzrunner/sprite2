#define S2_VIRTUAL_INHERITANCE

#ifdef NDEBUG
#define S2_DISABLE_STATISTICS
#endif // NDEBUG

#ifdef _WIN32
#define	S2_DEBUG
#endif // _WIN32

#ifndef _WIN32
#define SPR_ACTORS_HASH
#endif // _WIN32

//#define S2_MULTITHREAD
