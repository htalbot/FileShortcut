
#include "test_FileShortcutAppLayer.h"
#include "FileShortcut/FileShortcut.h"

using namespace FileShortcut_ns;

#include <stdio.h>

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

Test_FileShortcutAppLayer::Test_FileShortcutAppLayer()
{
}

bool Test_FileShortcutAppLayer::run()
{
    printf("***** Test_FileShortcutAppLayer (%s) *****\n\n", TEST_FILESHORTCUT_DebugFlag);

    int major, minor, patch;
    FileShortcut_get_version(major, minor, patch);
    printf("FileShortcut version: %d.%d.%d\n", major, minor, patch);

    printf("%s\n", FileShortcut_ns::FileShortcut::resolve("C:\\dev3\\src\\thirdparty\\htalbot\\FileShortcut\\projects\\FileShortcut\\etc\\FileShortcut\\test.txt").c_str());
    printf("%s\n", FileShortcut_ns::FileShortcut::resolve("C:\\dev3\\src\\thirdparty\\htalbot\\FileShortcut\\projects\\FileShortcut\\var\\FileShortcut\\test.txt.lnk").c_str());

    //printf("%s\n", FileShortcut_ns::FileShortcut::resolve("..\\..\\var\\FileShortcut\\test.txt").c_str());
    //wprintf(L"%s\n", FileShortcut_ns::FileShortcut::resolve(L"..\\..\\var\\FileShortcut\\test.txt").c_str());

    // Recusivity?
    printf("%s\n", FileShortcut_ns::FileShortcut::resolve("C:\\dev3\\src\\thirdparty\\htalbot\\FileShortcut\\projects\\FileShortcut\\var\\FileShortcut\\test2.txt.lnk").c_str());
    // ... yes

    if (FileShortcut_ns::FileShortcut::create_link(
        "C:\\dev3\\src\\thirdparty\\htalbot\\FileShortcut\\projects\\FileShortcut\\etc\\FileShortcut\\test.txt", 
        "C:\\dev3\\src\\thirdparty\\htalbot\\FileShortcut\\projects\\FileShortcut\\var\\test_created.txt.lnk",
        "blabla"))
    {
        printf("create link ok.\n");
    }
    else
    {
        printf("Failed to create link.\n");
    }

    return true;
}

