//#define S2_VIRTUAL_INHERITANCE

#define S2_SPR_DEFAULT_EDITABLE false

//#define S2_MATRIX_FIX

//#define S2_SPR_CACHE_LOCAL_MAT_SHARE

//#define S2_RES_LOG

#define S2_DISABLE_MODEL

#ifdef NDEBUG
#define S2_DISABLE_STATISTICS
#endif // NDEBUG

#ifdef _WIN32
#define	S2_DEBUG
#endif // _WIN32