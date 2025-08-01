//-*- C++ -*-
// The following configuration file contains defines for Borland compilers.

#ifndef ACE_CONFIG_WIN32_BORLAND_H
#define ACE_CONFIG_WIN32_BORLAND_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
# error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#define ACE_HAS_CUSTOM_EXPORT_MACROS 1
#define ACE_Proper_Export_Flag __declspec (dllexport)
#define ACE_Proper_Import_Flag __declspec (dllimport)
#define ACE_EXPORT_SINGLETON_DECLARATION(T) template class __declspec (dllexport) T
#define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#define ACE_IMPORT_SINGLETON_DECLARATION(T) template class __declspec (dllimport) T
#define ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllimport) SINGLETON_TYPE <CLASS, LOCK>;

// In later versions of C++Builder we will prefer inline functions by
// default. The debug configuration of ACE is built with functions
// out-of-line, so when linking your application against a debug ACE
// build, you can choose to use the out-of-line functions by adding
// ACE_NO_INLINE=1 to your project settings.
# if !defined (__ACE_INLINE__)
#  define __ACE_INLINE__ 1
# endif /* __ACE_INLINE__ */

#define ACE_CC_NAME ACE_TEXT ("Embarcadero C++ Builder")
#define ACE_CC_MAJOR_VERSION (__BORLANDC__ / 0x100)
#define ACE_CC_MINOR_VERSION (__BORLANDC__ % 0x100)
#define ACE_CC_BETA_VERSION (0)

#if !defined (WIN32)
# if defined (__WIN32__) || defined (_WIN32)
#  define WIN32 1
# endif
#endif

// When building a VCL application, the main VCL header file should be
// included before anything else. You can define ACE_HAS_VCL=1 in your
// project settings to have this file included for you automatically.
# if defined (ACE_HAS_VCL) && (ACE_HAS_VCL != 0)
#  include /**/ <vcl.h>
# endif

#define ACE_HAS_BCC64

#define ACE_CC_PREPROCESSOR_ARGS "--precompile -std=c++17 -q -o%s"
#define ACE_CC_PREPROCESSOR "BCC64X.EXE"

# include "ace/config-win32-common.h"

#define ACE_WSTRING_HAS_USHORT_SUPPORT 1
#define ACE_HAS_DIRENT

#define ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS
#define ACE_HAS_TIME_T_LONG_MISMATCH

#define ACE_LACKS_TERMIOS_H
#define ACE_LACKS_NETINET_TCP_H
#define ACE_LACKS_REGEX_H
#define ACE_LACKS_SYS_MSG_H
#define ACE_LACKS_PWD_H
#define ACE_LACKS_POLL_H
#define ACE_LACKS_SYS_SHM_H
#define ACE_LACKS_STRINGS_H
#define ACE_LACKS_SEMAPHORE_H
#define ACE_LACKS_INTTYPES_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_SYS_SELECT_H
#define ACE_LACKS_SYS_TIME_H
#define ACE_LACKS_SYS_RESOURCE_H
#define ACE_LACKS_SYS_WAIT_H
#define ACE_LACKS_DLFCN_H
#define ACE_LACKS_SYS_MMAN_H
#define ACE_LACKS_SYS_UIO_H
#define ACE_LACKS_SYS_SOCKET_H
#define ACE_LACKS_NETINET_IN_H
#define ACE_LACKS_NETDB_H
#define ACE_LACKS_NET_IF_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_SYS_IOCTL_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_WCSRTOMBS
#define ACE_LACKS_SET_ABORT_BEHAVIOR
#define ACE_HAS_CRTSETREPORTMODE
#define ACE_HAS_SETUNHANDLEDEXCEPTIONFILTER

#undef ACE_LACKS_REWINDDIR

#define ACE_HAS_WOPENDIR_EQUIVALENT ::_wopendir
#define ACE_HAS_WCLOSEDIR_EQUIVALENT ::_wclosedir
#define ACE_HAS_WREADDIR_EQUIVALENT ::_wreaddir
#define ACE_HAS_WREWINDDIR_EQUIVALENT ::_wrewinddir

#define ACE_LACKS_STRRECVFD

#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_USER_MODE_MASKS 1
#define ACE_LACKS_ACE_IOSTREAM 1
#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
#define ACE_ENDTHREADEX(STATUS) ::_endthreadex ((DWORD) STATUS)

#if defined(ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
// must have _MT defined to include multithreading
// features from win32 headers
# if !defined(__MT__)
// *** DO NOT *** defeat this error message by defining __MT__ yourself.
// You must link with the multi threaded libraries. Add -tWM to your
// compiler options
#  error You must link against multi-threaded libraries when using ACE (check your project settings)
# endif /* !__MT__ */
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

#define ACE_LACKS_ISWCTYPE
#define ACE_LACKS_ISCTYPE
#define ACE_LACKS_STRTOK_R
#define ACE_LACKS_ASCTIME_R
#define ACE_WCSDUP_EQUIVALENT ::_wcsdup
#define ACE_STRCASECMP_EQUIVALENT ::stricmp
#define ACE_STRNCASECMP_EQUIVALENT ::strnicmp
#define ACE_WTOF_EQUIVALENT ::_wtof
#define ACE_FILENO_EQUIVALENT(X) (_get_osfhandle (::_fileno (X)))
#define ACE_HAS_ITOA
#define ACE_HAS_BUILTIN_BSWAP16
#define ACE_HAS_BUILTIN_BSWAP32
#define ACE_HAS_BUILTIN_BSWAP64
#define ACE_LACKS_INLINE_ASSEMBLY

#define ACE_LACKS_GID_T
#undef ACE_LACKS_USECONDS_T
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_UID_T
#define ACE_LACKS_GMTIME_R
#define ACE_LACKS_LOCALTIME_R
#define ACE_LACKS_NLINK_T
#define ACE_HAS_3_PARAM_WCSTOK
#define ACE_LACKS_STRPTIME

#if __cplusplus >= 201103L
# define ACE_HAS_CPP11
#endif
#if __cplusplus >= 201402L
# define ACE_HAS_CPP14
#endif
#if __cplusplus >= 201703L
# define ACE_HAS_CPP17
#endif
#if __cplusplus >= 202002L
# define ACE_HAS_CPP20
#endif
#if __cplusplus >= 202302L
# define ACE_HAS_CPP23
#endif

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_BORLAND_H */
