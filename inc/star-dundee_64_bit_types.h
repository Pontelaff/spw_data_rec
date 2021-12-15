/**
 * \file star-dundee_64_bit_types.h
 * \brief Definitions of signed and unsigned 64-bit
 *        types used by STAR-Dundee software.
 * \author Stephen Mudie\n
 *         STAR-Dundee Ltd\n
 *         STAR House,\n
 *         166 Nethergate,\n 
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains definitions of signed and unsigned
 * 64-bit types used by STAR-Dundee software.
 *
 * IMPORTANT NOTE:
 * If you are experiencing compilation errors
 * indicating that U64 is already defined,
 * for example, please add the following
 * line to your code prior to including this file:
 * #define NO_STAR_TYPES
 * Alternatively you can compile your code with
 * a flag of -DNO_STAR_TYPES.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#ifndef STAR_FIRE_TYPES
#define STAR_FIRE_TYPES

#ifndef NO_STAR_TYPES

#if (defined(__linux__) || defined(LINUX) || defined(__LINUX__)) && \
    defined(__KERNEL__)

    #include <linux/types.h>

    /* Define U64 in the Linux kernel */    
    typedef u64	U64;
    
#else
    
    #include <limits.h>    

    /* Define U64 */
    #if (ULLONG_MAX == 0xffffffffffffffffULL)
        typedef unsigned long long	U64;
    #elif (UINT_MAX == 0xffffffffffffffffULL)
        typedef unsigned int	U64;
    #elif (ULONG_MAX == 0xffffffffffffffffULL)
        typedef unsigned long	U64;
    #elif (USHRT_MAX == 0xffffffffffffffffULL)
        typedef unsigned short	U64;
    #elif (UCHAR_MAX == 0xffffffffffffffffULL)
        typedef unsigned char	U64;
    #elif defined(_WIN32) || defined(_WIN64)
        typedef unsigned __int64    U64;
    #else
        #error "No valid definition of U64 available"
    #endif

    /* Define S64 */
    #if (LLONG_MAX == 0x7fffffffffffffffLL)
        typedef long long	S64;
    #elif (INT_MAX == 0x7fffffffffffffffLL)
        typedef int	S64;
    #elif (LONG_MAX == 0x7fffffffffffffffLL)
        typedef long	S64;
    #elif (SHRT_MAX == 0x7fffffffffffffffLL)
        typedef short	S64;
    #elif (CHAR_MAX == 0x7fffffffffffffffLL)
        typedef unsigned char	S64;
    #elif defined(_WIN32) || defined(_WIN64)
        typedef __int64    S64;
    #else
        #error "No valid definition of U64 available"
    #endif

#endif /* Linux kernel test */
        

#endif	/* NO_STAR_TYPES */


#endif	/* STAR_FIRE_TYPES */


