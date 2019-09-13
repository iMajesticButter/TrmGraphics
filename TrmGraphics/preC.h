#ifndef preC_HEADER
#define preC_HEADER

#define UNREF_PARAM(x) (void)(x)

#if defined(_WIN32)
    #define PLATFORM_WINDOWS // Windows
#elif defined(_WIN64)
    #define PLATFORM_WINDOWS // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define PLATFORM_WINDOWS // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__linux__)
    #define PLATFORM_LINUX // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#endif

#endif
