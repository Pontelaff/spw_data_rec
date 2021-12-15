/**
 * \file version.h
 *
 * \brief Version information for a module.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the current version information used by each of the
 * modules in STAR-System, the STAR-Dundee software stack.  Before including
 * this file, <code>VERSION_NAME</code> and <code>VERSION_AUTHOR</code> macros
 * should be defined to provide the name and authors of the module, e.g.
 *
 * \code
 * #define VERSION_NAME    ("SpaceWire PCI Driver for Windows")
 * #define VERSION_AUTHOR    ("Stuart Mills, STAR-Dundee Ltd.")
 * \endcode
 *
 * Copyright &copy; 2015 STAR-Dundee Ltd.
 */



#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED


#ifndef __KERNEL__
#include <stdio.h>

#include "star-dundee_types.h"
#endif


/** The module's major version number. */
#define VERSION_MAJOR (4)

/** The module's minor version number. */
#define VERSION_MINOR (5)

/** The module's edit number. */
#define VERSION_EDIT (10264)

/** The module's patch number. */
#define VERSION_PATCH    (0)


/**
 * Length of strings stored in a version information structure, inclusive of
 * null terminator.
 */
#define STAR_STR_MAX_LEN 256

/** A structure used to store version information for a module. */
typedef struct
{
    /** The name of the module. */
    char name[STAR_STR_MAX_LEN];

    /** The author of the module. */
    char author[STAR_STR_MAX_LEN];

    /** The major version number of this module. */
    U16 major;

    /** The minor version number of this module. */
    U16 minor;

    /** The edit number of this module. */
    U16 edit;

    /** The patch number of this module. Edit will be 0 if patch is non zero */
    U16 patch;

} STAR_VERSION_INFO;


/**
 * Macro to print the full version information, including name and author.
 *
 * @param o the output stream
 */
#define PRINT_FULL_VERSION_INFO(o)                          \
    {                                                       \
        fprintf(o, "Module Name:   %s\n", VERSION_NAME);    \
        fprintf(o, "Module Author: %s\n", VERSION_AUTHOR);  \
        fprintf(o, "Version:       ");                      \
        PRINT_VERSION_NUM(o);                               \
        fprintf(o, "\n");                                   \
    }

/**
 * Macro to print the version information.
 *
 * @param o the output stream
 */
#define PRINT_VERSION(o)                         \
    {                                            \
        (void)fprintf(o, "%s", VERSION_NAME);    \
        PRINT_VERSION_NUM(o);                    \
    }


/**
 * Macro to print the version's edit number if present.  Used by
 * {@link PRINT_VERSION_NUM(FILE *)} but contained in a separate macro to avoid
 * "conditional expression is constant" warnings.
 *
 * @param o the output stream
 */
#if VERSION_EDIT
    #define PRINT_VERSION_EDIT(o) (void)fprintf(o, "(%d)", VERSION_EDIT)
#else
    #define PRINT_VERSION_EDIT(o)
#endif

/**
 * Macro to print the version's patch number if present.  Used by
 * {@link PRINT_VERSION_NUM(FILE *)} but contained in a separate macro to avoid
 * "conditional expression is constant" warnings.
 *
 * @param o the output stream
 */
#if VERSION_PATCH
    #define PRINT_VERSION_PATCH(o) (void)fprintf(o, "p%d", VERSION_PATCH)
#else
    #define PRINT_VERSION_PATCH(o)
#endif

/**
 * Macro to print the version number.
 *
 * @param o the output stream
 */
#define PRINT_VERSION_NUM(o)                                         \
    {                                                                \
        (void)fprintf(o, "v%d.%02d", VERSION_MAJOR, VERSION_MINOR);  \
        PRINT_VERSION_EDIT(o);                                       \
        PRINT_VERSION_PATCH(o);                                      \
    }

/**
 * Macro to populate a version information structure.
 *
 * @param v the version information structure to be updated
 */
#define POPULATE_VERSION(v)                                      \
    {                                                            \
        (v).major  = VERSION_MAJOR;                              \
        (v).minor  = VERSION_MINOR;                              \
        (v).edit   = VERSION_EDIT;                               \
        (v).patch  = VERSION_PATCH;                              \
                                                                 \
        strcpy_s((v).name, STAR_STR_MAX_LEN, VERSION_NAME);      \
                                                                 \
        strcpy_s((v).author, STAR_STR_MAX_LEN, VERSION_AUTHOR);  \
    }

#endif
