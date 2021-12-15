/**
 * \file star-dundee_types.h
 *
 * \brief Definitions of STAR-Dundee commonly used types.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the definitions of common types used by STAR-Dundee
 * software drivers and APIs.
 *
 * <b>IMPORTANT NOTE:</b>
 * \note            If you are experiencing compilation errors indicating that
 *                  U8 is already defined, for example, please add the following
 *                  line to your code prior to including this file:\n
 *                      <code>\#define NO_STAR_TYPES</code>\n
 *                  Alternatively you can compile your code with a flag of
 *                  <code>-DNO_STAR_TYPES</code>.\n
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 *
 * The content of this source file is CONFIDENTIAL and PROPRIETARY property of
 * STAR-Dundee Ltd.
 */


/**
 * \defgroup STARDundeeTypes STAR-Dundee Types
 * This section contains the definitions of types used in STAR-Dundee software
 * drivers and APIs.
 */

/**
 * \def TRUE
 * \ingroup STARDundeeTypes
 * A value that can be used to represent the boolean value of true.
 */

/**
 * \def FALSE
 * \ingroup STARDundeeTypes
 * A value that can be used to represent the boolean value of false.
 */

/**
 * \typedef U8
 * \ingroup STARDundeeTypes
 * A type that can be used to represent an unsigned 8-bit number.
 */

/**
 * \typedef U16
 * \ingroup STARDundeeTypes
 * A type that can be used to represent an unsigned 16-bit number.
 */

/**
 * \typedef U16_MAX
 * \ingroup STARDundeeTypes
 * The maximum value that can be represented using an unsigned 16-bit number.
 */

/**
 * \typedef U32
 * \ingroup STARDundeeTypes
 * A type that can be used to represent an unsigned 32-bit number.
 */

/**
 * \typedef REGISTER
 * \ingroup STARDundeeTypes
 * A type that can be used to represent a 4-byte register.
 */


#ifndef STAR_DUNDEE_TYPES
#define STAR_DUNDEE_TYPES


#ifndef NO_STAR_TYPES

#if (defined(__linux__) || defined(LINUX) || defined(__LINUX__) || \
    defined(__CYGWIN__)) && defined(__KERNEL__)

    #include <linux/types.h>

    /* Define U8, U16 and U32 in the Linux kernel */
    #ifndef U8
        typedef u8  U8;
    #endif
    #ifndef U16
        typedef u16 U16;
    #endif
    #ifndef U32
        typedef u32 U32;
    #endif

#else

    #include <limits.h>

    /* Define U8 */
    #ifndef U8
        #if (UCHAR_MAX == 0xff)
            typedef unsigned char   U8;
        #elif (UINT_MAX == 0xff)
            typedef unsigned int    U8;
        #else
            #error "No valid definition of U8 available"
        #endif
    #endif

    /* Define U16 */
    #ifndef U16
        #if (USHRT_MAX == 0xffff)
            typedef unsigned short  U16;
        #elif (UINT_MAX == 0xffff)
            typedef unsigned int    U16;
        #elif (UCHAR_MAX == 0xffff)
            typedef unsigned char   U16;
        #else
            #error "No valid definition of U16 available"
        #endif

        #define U16_MAX (0xffff)
    #endif

    /* Define U32 */
    #ifndef U32
        #if (UINT_MAX == 0xffffffff)
            typedef unsigned int    U32;
        #elif (ULONG_MAX == 0xffffffff)
            typedef unsigned long   U32;
        #elif (USHRT_MAX == 0xffffffff)
            typedef unsigned short  U32;
        #elif (UCHAR_MAX == 0xffffffff)
            typedef unsigned char   U32;
        #else
            #error "No valid definition of U32 available"
        #endif
    #endif

#endif /* Linux kernel test */


/* Define TRUE and FALSE */
#ifndef TRUE
    #define TRUE  1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

/* Define REGISTER */
#ifndef REGISTER
    typedef U32 REGISTER;
#endif


#endif  /* NO_STAR_TYPES */


#endif  /* STAR_DUNDEE_TYPES */


