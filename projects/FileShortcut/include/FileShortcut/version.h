
#pragma once

#define FILESHORTCUT_MAJOR 0
#define FILESHORTCUT_MINOR 0
#define FILESHORTCUT_PATCH 5


#if defined _MSC_VER
#   ifdef _DEBUG
        const char FILESHORTCUT_DebugFlag[] = "DEBUG";
#   else
        const char FILESHORTCUT_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char FILESHORTCUT_DebugFlag[] = "DEBUG";
#   else
        const char FILESHORTCUT_DebugFlag[] = "RELEASE";
#   endif
#endif


