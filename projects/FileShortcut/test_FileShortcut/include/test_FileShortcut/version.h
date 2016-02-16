
#ifndef __TEST_FILESHORTCUT_VERSION_H__
#define __TEST_FILESHORTCUT_VERSION_H__

#define TEST_FILESHORTCUT_MAJOR 0
#define TEST_FILESHORTCUT_MINOR 0
#define TEST_FILESHORTCUT_PATCH 5


#if defined _MSC_VER
#   ifdef _DEBUG
        const char TEST_FILESHORTCUT_DebugFlag[] = "DEBUG";
#   else
        const char TEST_FILESHORTCUT_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char TEST_FILESHORTCUT_DebugFlag[] = "DEBUG";
#   else
        const char TEST_FILESHORTCUT_DebugFlag[] = "RELEASE";
#   endif
#endif


#endif // __TEST_FILESHORTCUT_VERSION_H__
