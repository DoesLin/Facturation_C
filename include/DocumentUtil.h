/**
 * Copyright 2010 Sébastien Aupetit <sebtic@projectsforge.org>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */

#ifndef FACTURATION_BASE_DOCUMENTUTIL_H
#define FACTURATION_BASE_DOCUMENTUTIL_H

#include <Config.h>

/** @defgroup DocumentUtil Utility functions for documents
 * @ingroup Documents
 * @{
 */

/** Create a new string on the heap which represents the parameter as a number in basis 36.
 * @param id the number to convert
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(computeDocumentNumber)(long id);

/** Create a new string on the heap which represents the date in the format DD/MM/YYYY.
 * @param day the day
 * @param month the month
 * @param year the year
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(formatDate)(int day, int month, int year);

/** Write a string in a binary file
 * @param str the string
 * @param file the file
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(writeString)(const char * str, FILE * file);

/** Read a string from a binary file
 * @param file the file
 * @return a new string created on the heap which contains the read string
 * @see writeString()
 */
OVERRIDABLE_PREFIX char * OVERRIDABLE(readString)(FILE * file);

/** @} */

#include <provided/DocumentUtil.h>
#include <user/DocumentUtil.h>

#endif

