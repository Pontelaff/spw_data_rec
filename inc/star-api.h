/**
 * \file star-api.h
 *
 * \brief Declarations of all functions provided by STAR-API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains includes to the declarations of all functions provided by
 * STAR-API, the STAR-Dundee software API.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 */

#ifndef STAR_API_H
#define STAR_API_H

#include "star-dundee_annotations.h"

/*
 * Relating to the API itself, managing devices and channels,
 * and getting device and channel information.
 */
#include "general.h"

/*
 * Relating to the creation and disposal of Stream Items, such as packets and control codes
 */
#include "stream_item.h"

/*
 * Relating to the transfer of Stream Items across channels
 */
#include "transfer.h"

#endif    /* STAR_API_H */
