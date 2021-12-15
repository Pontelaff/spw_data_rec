/**
 * \file star-dundee_annotations.h
 *
 * \brief Macros for previously undefined annotations macros.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the definitions of macros of annotations that are defined
 * in some operating systems but not others. The idea is that if we provide
 * blank macros for any undefined annotations then they will compile on
 * operating systems that don't provide the annotation support.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */


#ifndef STAR_PORT_ANNOTATIONS_H
#define STAR_PORT_ANNOTATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: DEPRECATED annotation not implemented for this compiler")
#define DEPRECATED(func) func
#endif


#if (!defined(_MSC_VER) || defined(NO_STAR_ANNOTATIONS))

    #if !defined(UNREFERENCED_PARAMETER)
        #define UNREFERENCED_PARAMETER(a)   ((void)(a))
    #endif

    #if !defined(_In_)
        #define _In_
    #endif
    #if !defined(_Out_)
        #define _Out_
    #endif

    #if !defined(_Deref_out_)
        #define _Deref_out_
    #endif

    #if !defined(_Inout_)
        #define _Inout_
    #endif

        #if !defined(_Inout_cap_)
        #define _Inout_cap_(a)
    #endif

    #if !defined(_In_opt_)
        #define _In_opt_
    #endif

    #if !defined(_Out_cap_)
        #define _Out_cap_(a)
    #endif

    #if !defined(_Out_opt_cap_)
        #define _Out_opt_cap_(a)
    #endif

    #if !defined(_Out_opt_z_cap_)
        #define _Out_opt_z_cap_(a)
    #endif

    #if !defined(_Out_cap_c_)
        #define _Out_cap_c_(a)
    #endif

    #if !defined(_Out_z_cap_c_)
        #define _Out_z_cap_c_(a)
    #endif

    #if !defined(_Out_bytecap_)
        #define _Out_bytecap_(a)
    #endif

    #if !defined(_Inout_count_)
        #define _Inout_count_(a)
    #endif

    #if !defined(_In_opt_count_)
        #define _In_opt_count_(a)
    #endif

    #if !defined(_In_bytecount_)
        #define _In_bytecount_(a)
    #endif

    #if !defined(_In_count_)
        #define _In_count_(a)
    #endif

    #if !defined(_In_opt_bytecount_)
        #define _In_opt_bytecount_(a)
    #endif

    #if !defined(_Out_opt_)
        #define _Out_opt_
    #endif

    #if !defined(_In_opt_z_)
        #define _In_opt_z_
    #endif

    #if !defined(_In_z_)
        #define _In_z_
    #endif

    #if !defined(_In_count_c_)
        #define _In_count_c_(a)
    #endif

     #if !defined(_In_opt_z_count_c_)
        #define _In_opt_z_count_c_(a)
    #endif

    #if !defined(_In_z_count_c_)
        #define _In_z_count_c_(a)
    #endif

    #if !defined(_Inout_bytecap_)
        #define _Inout_bytecap_(a)
    #endif

    #if !defined(_Check_return_)
        #define _Check_return_
    #endif

    #if !defined(_Ret_opt_bytecap_x_)
        #define _Ret_opt_bytecap_x_(a)
    #endif

    #if !defined(_Ret_opt_cap_)
        #define _Ret_opt_cap_(a)
    #endif

    #if !defined(_Ret_opt_cap_c_)
        #define _Ret_opt_cap_c_(a)
    #endif

    #if !defined(_Ret_opt_z_)
        #define _Ret_opt_z_
    #endif

    #if !defined(_Ret_maybenull_)
        #define _Ret_maybenull_
    #endif

    #if !defined(_Success_)
        #define _Success_(a)
    #endif


    #if !defined(__cdecl)
        #define __cdecl
    #endif

#else

    #include <sal.h>

#endif



#if defined(__vxworks)

/* Also define includes required for VxWorks library */
#include "stdlib.h"

#endif



#if !defined(_Post_ptr_invalid_)
    #define _Post_ptr_invalid_
#endif



#ifdef __cplusplus
}
#endif



#endif  /* STAR_PORT_ANNOTATIONS_H */
