
#include "FileShortcut/FileShortcut.h"
#include "FileShortcut/version.h"
#include "BasicEventHandling/BasicEventHandling.h"
#include <fstream>

#ifdef ACE_WIN32
#include "windows.h"
#include "shobjidl.h"
#include "shlguid.h"
#include "strsafe.h"
//#include "winnls.h"
//#include "shobjidl.h"
//#include "objbase.h"
//#include "objidl.h"
#endif

BasicEventHandling_ns::BasicEvent ev_copy_failed("FileShortcut", 1, "wcscpy() failed with '%1(%s)'.");
BasicEventHandling_ns::BasicEvent ev_no_lnk_file("FileShortcut", 2, "Neither '%1(%s)' nor '%2(%s)' exist.");
BasicEventHandling_ns::BasicEvent ev_resolve_failure("FileShortcut", 3, "Can't resolve '%1(%s)'.");
BasicEventHandling_ns::BasicEvent ev_itf_not_found("FileShortcut", 4, "Interface not found: '%1(%s)'.");
BasicEventHandling_ns::BasicEvent ev_ishelllink_resolve_failed("FileShortcut", 5, "IShellLink::resolve failed: %1(%d).");
BasicEventHandling_ns::BasicEvent ev_ipersitsave_failed("FileShortcut", 5, "IPersistFile::Save failed: %1(%d).");
BasicEventHandling_ns::BasicEvent ev_create_link_failure("FileShortcut", 6, "create_link failed.");

#ifdef ACE_WIN32
// ResolveIt
// ---------
// Parameters:
// hwnd         - A handle to the parent window. The Shell uses this window to 
//                display a dialog box if it needs to prompt the user for more 
//                information while resolving the link.
// lpszLinkFile - Address of a buffer that contains the path of the link,
//                including the file name.
// lpszPath     - Address of a buffer that receives the path of the link
//                target, including the file name.
// lpszDesc     - Address of a buffer that receives the description of the 
//                Shell link, stored in the Comment field of the link
//                properties.
HRESULT ResolveIt(HWND hwnd, LPSTR lpszLinkFile, LPWSTR lpszPath, int iPathBufferSize) 
{ 
    HRESULT hres; 
    IShellLink* psl; 
    WCHAR szGotPath[MAX_PATH]; 
    WCHAR szDescription[MAX_PATH]; 
    WIN32_FIND_DATA wfd; 
 
    *lpszPath = 0; // Assume failure 

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called. 
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 
 
        // Get a pointer to the IPersistFile interface. 
        hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
        
        if (SUCCEEDED(hres)) 
        { 
            WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, MAX_PATH); 
 
            // Add code here to check return value from MultiByteWideChar 
            // for success.
 
            // Load the shortcut. 
            hres = ppf->Load(wsz, STGM_READ); 
            
            if (SUCCEEDED(hres)) 
            { 
                // Resolve the link. 
                hres = psl->Resolve(hwnd, 0); 

                if (SUCCEEDED(hres)) 
                { 
                    // Get the path to the link target. 
                    hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_RAWPATH); 

                    if (SUCCEEDED(hres)) 
                    { 
                        // Get the description of the target. 
                        hres = psl->GetDescription(szDescription, MAX_PATH); 

                        if (SUCCEEDED(hres)) 
                        {
                            wchar_t * cpy = wcscpy(lpszPath, szGotPath);
                            if (wcslen(cpy) != wcslen(szGotPath))
                            {
                                char szBuffer[MAX_PATH];
                                wcstombs(szBuffer, szGotPath, MAX_PATH);

                                EVENT_HANDLER_SYSTEMLOG->handle_event(
                                    "(FileShortcut) - ResolveIt",
                                    ev_copy_failed,
                                    LM_ERROR,
                                    szBuffer);
                            }
                        }
                    }
                } 
                else
                {
                    EVENT_HANDLER_SYSTEMLOG->handle_event(
                         "(FileShortcut) - ResolveIt",
                        ev_ishelllink_resolve_failed,
                        LM_ERROR,
                        hres);
                }
            } 

            // Release the pointer to the IPersistFile interface. 
            ppf->Release(); 
        } 

        // Release the pointer to the IShellLink interface. 
        psl->Release(); 
    } 
    else
    {
        EVENT_HANDLER_SYSTEMLOG->handle_event(
            "(FileShortcut) - ResolveIt",
            ev_itf_not_found,
            LM_ERROR,
            "IShellLink");
    }

    return hres; 
}

// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces
//              to create and store a shortcut to the specified object.
//
// Returns the result of calling the member functions of the interfaces.
//
// Parameters:
// lpszPathObj  - Address of a buffer that contains the path of the object,
//                including the file name.
// lpszPathLink - Address of a buffer that contains the path where the
//                Shell link is to be stored, including the file name.
// lpszDesc     - Address of a buffer that contains a description of the
//                Shell link, stored in the Comment field of the link
//                properties.
HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc)
{
    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Add code here to check return value from MultiByteWideChar
            // for success.

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            if (FAILED(hres))
            {
                EVENT_HANDLER_SYSTEMLOG->handle_event(
                    "(FileShortcut) - CreateLink",
                    ev_ipersitsave_failed,
                    LM_ERROR,
                    hres);
            }
            ppf->Release();
        }
        else
        {
            EVENT_HANDLER_SYSTEMLOG->handle_event(
                "(FileShortcut) - CreateLink",
                ev_itf_not_found,
                LM_ERROR,
                "IPersistFile");
        }

        psl->Release();
    }
    else
    {
        EVENT_HANDLER_SYSTEMLOG->handle_event(
            "(FileShortcut) - CreateLink",
            ev_itf_not_found,
            LM_ERROR,
            "IShellLink");
    }

    return hres;
}
#endif // ACE_WIN32

namespace FileShortcut_ns
{
    void FileShortcut_get_version(int & major, int & minor, int & patch)
    {
        major = FILESHORTCUT_MAJOR;
        minor = FILESHORTCUT_MINOR;
        patch = FILESHORTCUT_PATCH;
    }

    std::string FileShortcut::resolve(const std::string & file_in)
    {
        wchar_t file[MAX_PATH];
        mbstowcs(file, file_in.c_str(), MAX_PATH);

        const std::wstring in(file);
        std::wstring out = resolve(in);

        char szBuffer[MAX_PATH];
        wcstombs(szBuffer, out.c_str(), MAX_PATH);

        return szBuffer;
    }

    std::wstring FileShortcut::resolve(const std::wstring & file_in)
    {
#ifdef ACE_WIN32

        bool add_lnk_ext(true);

        if (file_in.find(L".lnk") == file_in.npos) 
        {
            // *** not a .lnk file ***

            std::ifstream stream(file_in);
            if(stream)
            {
                return file_in;
            }
        }
        else
        {
            add_lnk_ext = false;
        }

        std::wstring file = file_in;
        
        if (add_lnk_ext)
        {
            file += L".lnk";
        }

        std::ifstream stream_lnk(file);
        if(!stream_lnk)
        {
            char szBuffer[MAX_PATH];
            wcstombs(szBuffer, file.c_str(), MAX_PATH);

            char szBuffer2[MAX_PATH];
            wcstombs(szBuffer2, file_in.c_str(), MAX_PATH);

            EVENT_HANDLER_SYSTEMLOG->handle_event(
                _SRC,
                ev_no_lnk_file,
                LM_ERROR,
                szBuffer2,
                szBuffer);

            return file_in;
        }


        char szBuffer[MAX_PATH];
        wcstombs(szBuffer, file.c_str(), MAX_PATH);


        bool couninit(true);
        HRESULT hr = CoInitialize(NULL);
        if (hr == S_FALSE)
        {
            couninit = false;
        }

        wchar_t path[MAX_PATH];
        path[0] = 0;
        if (FAILED(ResolveIt(0, szBuffer, path, MAX_PATH)))
        {
            char szBuffer2[MAX_PATH];
            wcstombs(szBuffer2, file_in.c_str(), MAX_PATH);

            EVENT_HANDLER_SYSTEMLOG->handle_event(
                _SRC,
                ev_resolve_failure,
                LM_ERROR,
                szBuffer2);
        }

        if (couninit)
        {
            CoUninitialize();
        }

        return path;
#else
        return file_in;
#endif
    }

    bool FileShortcut::create_link(const std::string & path_obj_in, const std::string & path_link_in, const std::string & desc_in)
    {
        wchar_t path_obj[MAX_PATH];
        mbstowcs(path_obj, path_obj_in.c_str(), MAX_PATH);

        wchar_t path_link[MAX_PATH];
        mbstowcs(path_link, path_link_in.c_str(), MAX_PATH);

        wchar_t desc[MAX_PATH];
        mbstowcs(desc, desc_in.c_str(), MAX_PATH);

        return create_link(path_obj, path_link, desc);
    }

    bool FileShortcut::create_link(const std::wstring & path_obj, const std::wstring & path_link, const std::wstring & desc)
    {
        bool couninit(true);
        HRESULT hr = CoInitialize(NULL);
        if (hr == S_FALSE)
        {
            couninit = false;
        }

        char szPathLink[MAX_PATH];
        wcstombs(szPathLink, path_link.c_str(), MAX_PATH);

        bool rc = SUCCEEDED(CreateLink(path_obj.c_str(), szPathLink, desc.c_str()));
        if (!rc)
        {
            EVENT_HANDLER_SYSTEMLOG->handle_event(
                _SRC,
                ev_create_link_failure,
                LM_ERROR);
        }

        if (couninit)
        {
            CoUninitialize();
        }

        return rc;
    }

} // FileShortcut_ns namespace
