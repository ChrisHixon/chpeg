//
// chpeg: chpeg_api.h {
//

#ifndef CHPEG_API_H
#define CHPEG_API_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHPEG_AMALGAMATION
#ifdef CHPEG_AMALGAMATION_ONE_FILE

// amalgamation: single .h file
#define CHPEG_API static
#define CHPEG_DEF static

#else

// amalgamation: separate .c and .h files
#define CHPEG_API extern
#define CHPEG_DEF

#endif
#endif

// no amalgamation
#ifndef CHPEG_API
#define CHPEG_API extern
#define CHPEG_DEF

#endif

// define CHPEG_EXTENSION_ALL to enable all grammar extensions (currently must be all or none)
#ifdef CHPEG_EXTENSION_ALL
#define CHPEG_EXTENSION_TRIM 1
#define CHPEG_EXTENSION_REFS 1
#define CHPEG_EXTENSION_ESCAPES 1
#define CHPEG_EXTENSION_HEX 1
#define CHPEG_EXTENSION_NCHRCLS 1
#define CHPEG_EXTENSION_MINMAX 1
#endif

#ifndef CHPEG_EXTENSION_TRIM
#define CHPEG_EXTENSION_TRIM 0
#endif

#ifndef CHPEG_EXTENSION_REFS
#define CHPEG_EXTENSION_REFS 0
#endif

#ifndef CHPEG_EXTENSION_ESCAPES
#define CHPEG_EXTENSION_ESCAPES 0
#endif

#ifndef CHPEG_EXTENSION_HEX
#define CHPEG_EXTENSION_HEX 0
#endif

#ifndef CHPEG_EXTENSION_NCHRCLS
#define CHPEG_EXTENSION_NCHRCLS 0
#endif

#ifndef CHPEG_EXTENSION_MINMAX
#define CHPEG_EXTENSION_MINMAX 0
#endif

// used to check if using ANY extension
#if CHPEG_EXTENSION_TRIM \
    || CHPEG_EXTENSION_REFS \
    || CHPEG_EXTENSION_ESCAPES \
    || CHPEG_EXTENSION_HEX \
    || CHPEG_EXTENSION_NCHRCLS \
    || CHPEG_EXTENSION_MINMAX
#define CHPEG_USES_EXTENSIONS 1
#endif

// undefined behavior results when grammar allows syntax extensions but code to deal
// with those extension is not compiled in...
#if CHPEG_USES_EXTENSIONS && ( \
    ! CHPEG_EXTENSION_TRIM || \
    ! CHPEG_EXTENSION_REFS || \
    ! CHPEG_EXTENSION_ESCAPES || \
    ! CHPEG_EXTENSION_HEX || \
    ! CHPEG_EXTENSION_NCHRCLS || \
    ! CHPEG_EXTENSION_MINMAX )
#error "Selective use of grammar extensions is currently not supported. Please define CHPEG_EXTENSION_ALL"
#endif

#ifdef CHPEG_AMALGAMATION
// Define the guards of the *unwanted* bytecode .c and .h files
// to keep them from being compiled. (there is conflict in their #defines)
#if CHPEG_USES_EXTENSIONS
#define CHPEG_CHPEG_BYTECODE_C
#define CHPEG_CHPEG_BYTECODE_H
#else
#define CHPEG_CHPEG_EXT_BYTECODE_C
#define CHPEG_CHPEG_EXT_BYTECODE_H
#endif
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef CHPEG_API_H

// } chpeg: chpeg_api.h

