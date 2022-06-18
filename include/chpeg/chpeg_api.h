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

// define to enable ALL extensions
#ifdef CHPEG_EXTENSION_ALL
#define CHPEG_EXTENSION_TRIM 1
#define CHPEG_EXTENSION_REFS 1
#endif

#ifndef CHPEG_EXTENSION_TRIM
#define CHPEG_EXTENSION_TRIM 0
#endif

#ifndef CHPEG_EXTENSION_REFS
#define CHPEG_EXTENSION_REFS 0
#endif

// used to check if using ANY extension
#if CHPEG_EXTENSION_TRIM || CHPEG_EXTENSION_REFS
#define CHPEG_USES_EXTENSIONS 1
#endif

#ifdef CHPEG_AMALGAMATION
// sort of a hack: define the guards of the *unwanted* bytecode .c and .h files
// this is why I added a guard around the .c file
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

