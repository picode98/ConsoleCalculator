#include "GlobalBuildVariables.h"

// Microsoft Visual C++ generated include file.
// Used by buildinfo.rc

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_MAJOR               0
#define VERSION_MINOR               1
#define VERSION_REVISION            0
#define VERSION_BUILD               0

#ifdef MULTIPRECISION
#define VER_PRECISION_DESCRIPTION_STR	"arbitrary-precision"
#else
#define VER_PRECISION_DESCRIPTION_STR	"double-precision"
#endif

#define VER_FILE_DESCRIPTION_STR    "An advanced console-based calculator (" VER_PRECISION_DESCRIPTION_STR " build)"
#define VER_COMPANY_NAME_STR		"Saaman Khalilollahi"
#define VER_FILE_VERSION            VERSION_MAJOR,VERSION_MINOR,VERSION_REVISION// ,VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)    \
                                    "." STRINGIZE(VERSION_REVISION) \
//                                    "." STRINGIZE(VERSION_BUILD)    \

#define VER_PRODUCT_NAME_STR         "Console Calculator"
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR
// #define VER_ORIGINAL_FILENAME_STR   ""
// #define VER_INTERNAL_NAME_STR       VER_ORIGINAL_FILENAME_STR
#define VER_LICENSE_STR				"GPL license (version 3)"
#define VER_COPYRIGHT_STR           "Released under " VER_LICENSE_STR

#ifdef _DEBUG
#define VER_VER_DEBUG             VS_FF_DEBUG
#else
#define VER_VER_DEBUG             0x0L
#endif

#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILE_FLAGS               VER_VER_DEBUG
#define VER_FILE_TYPE                VFT_APP
