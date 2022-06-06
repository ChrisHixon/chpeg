//
// chpeg: chpeg_api.h {
//

#ifndef CHPEG_API_H
#define CHPEG_API_H

#ifdef CHPEG_AMALGAMATION
#define CHPEG_API
#define CHPEG_DEF
#endif

#ifndef CHPEG_API
#define CHPEG_API extern
#define CHPEG_DEF
#endif

// define to enable ALL extensions
#ifdef CHPEG_EXTENSIONS
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

#endif // #ifndef CHPEG_API_H

// } chpeg: chpeg_api.h

