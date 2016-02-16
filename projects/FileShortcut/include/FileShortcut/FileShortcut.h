
#ifndef __FILESHORTCUT_H__
#define __FILESHORTCUT_H__

#include "FileShortcut/FILESHORTCUT_Export.h"
#include "FileShortcut/version.h"
#include <string>

namespace FileShortcut_ns
{
    extern "C" FILESHORTCUT_Export void FileShortcut_get_version(int & major, int & minor, int & patch);

    class FILESHORTCUT_Export FileShortcut
    {
    public:
        static std::string resolve(const std::string &);
        static std::wstring resolve(const std::wstring &);
        static bool create_link(const std::string & path_obj, const std::string & path_link, const std::string & desc);
        static bool create_link(const std::wstring & path_obj, const std::wstring & path_link, const std::wstring & desc);
    };
} // FileShortcut_ns namespace


#endif // __FILESHORTCUT_H__
