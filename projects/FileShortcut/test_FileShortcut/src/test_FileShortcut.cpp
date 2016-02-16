
#include "test_FileShortcutAppLayer.h"

int main(int, char *[])
{
    Test_FileShortcutAppLayer app;

    if (!app.run())
    {
        // log
        return -1;
    }

    return 0;
}
