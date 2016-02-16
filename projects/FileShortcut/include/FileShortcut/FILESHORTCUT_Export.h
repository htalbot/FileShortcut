
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FILESHORTCUT
// ------------------------------
#ifndef FILESHORTCUT_EXPORT_H
#define FILESHORTCUT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (FILESHORTCUT_HAS_DLL)
#  define FILESHORTCUT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && FILESHORTCUT_HAS_DLL */

#if !defined (FILESHORTCUT_HAS_DLL)
#  define FILESHORTCUT_HAS_DLL 1
#endif /* ! FILESHORTCUT_HAS_DLL */

#if defined (FILESHORTCUT_HAS_DLL) && (FILESHORTCUT_HAS_DLL == 1)
#  if defined (FILESHORTCUT_BUILD_DLL)
#    define FILESHORTCUT_Export ACE_Proper_Export_Flag
#    define FILESHORTCUT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FILESHORTCUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FILESHORTCUT_BUILD_DLL */
#    define FILESHORTCUT_Export ACE_Proper_Import_Flag
#    define FILESHORTCUT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FILESHORTCUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FILESHORTCUT_BUILD_DLL */
#else /* FILESHORTCUT_HAS_DLL == 1 */
#  define FILESHORTCUT_Export
#  define FILESHORTCUT_SINGLETON_DECLARATION(T)
#  define FILESHORTCUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FILESHORTCUT_HAS_DLL == 1 */

// Set FILESHORTCUT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FILESHORTCUT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FILESHORTCUT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FILESHORTCUT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FILESHORTCUT_NTRACE */

#if (FILESHORTCUT_NTRACE == 1)
#  define FILESHORTCUT_TRACE(X)
#else /* (FILESHORTCUT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define FILESHORTCUT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (FILESHORTCUT_NTRACE == 1) */

#endif /* FILESHORTCUT_EXPORT_H */

// End of auto generated file.
