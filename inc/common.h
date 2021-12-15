/**
 * \file common.h
 *
 * \brief STAR-Dundee API macros and types required by STAR API modules.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the definitions of macros, structures and types required
 * by modules in the STAR-Dundee software stack.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#ifndef STAR_API_COMMON_H
#define STAR_API_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __KERNEL__
    #include <string.h>

    #include "star-dundee_annotations.h"
#endif


/** The name of the module, used for logging purposes. */
#define STAR_API_MODULE_NAME    "STAR-API"


/**
 * \def STAR_API_CC
 * The calling convention used by functions exported by the API.
 *
 * \versionAdded \ref changes_v0_8
 */

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #if defined(_WIN64) || defined(__x86_64__)
        #define STAR_API_CC
    #else
        #define STAR_API_CC __stdcall
    #endif
#else
    #define STAR_API_CC
#endif


#ifdef __cplusplus
}
#endif

#endif  /* STAR_API_COMMON_H */
