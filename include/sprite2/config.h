//#define S2_EDITOR

#ifdef S2_EDITOR
#define S2_FILTER_FULL
#define S2_VIRTUAL_INHERITANCE
#define S2_SHAPE_SHARED_PTR
#define S2_SPR_DEFAULT_EDITABLE true
#define S2_MESH_VIRTUAL
#else
#define S2_SPR_DEFAULT_EDITABLE false
#endif // S2_EDITOR

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

#define S2_MULTITHREAD

#define S2_DISABLE_FLATTEN
//#define S2_DISABLE_DEFERRED