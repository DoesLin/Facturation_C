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

#ifndef FACTURATION_BASE_OPERATORTABLE_H
#define FACTURATION_BASE_OPERATORTABLE_H

#include <Config.h>

/** @defgroup Operators Operator related stuff */

/** @defgroup OperatorTable Operator database
 * @ingroup Operators
 * @{
 */

/** The operator database file name */
extern const char * OPERATORDB_FILENAME;

/** The maximal length in characters of the name of an operator */
#define OPERATORTABLE_MAXNAMESIZE 20UL
/** The maximal length in characters of the password of an operator */
#define OPERATORTABLE_MAXPASSWORDSIZE 20UL

/** The dynamic table of operators */
typedef struct
{
  /** The number of operators in the table */
  int recordCount;
  /** The data about the operators. It's a 2D array of strings.
   * @note records[operatorId][0] is the name of the operatorId'th operator
   * @note records[operatorId][1] is the password of the operatorId'th operator
   */
  char *** records;
} OperatorTable;

/**
 * Create an empty table of operators.
 * @return the new table
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX OperatorTable * OVERRIDABLE(OperatorTable_create)(void);

/** Free a table of operators.
 * @param table a pointer to the table to free
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(OperatorTable_destroy)(OperatorTable * table);
/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX OperatorTable * OVERRIDABLE(OperatorTable_loadFromFile)(const char * filename);

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(OperatorTable_saveToFile)(OperatorTable * table, const char * filename);

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(OperatorTable_getRecordCount)(OperatorTable * table);

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX const char * OVERRIDABLE(OperatorTable_getName)(OperatorTable * table, int recordIndex);

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX const char * OVERRIDABLE(OperatorTable_getPassword)(OperatorTable * table, int recordIndex);

/** Find the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(OperatorTable_findOperator)(OperatorTable * table, const char * name);

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX int OVERRIDABLE(OperatorTable_setOperator)(OperatorTable * table, const char * name, const char * password);

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 * @relates OperatorTable
 */
OVERRIDABLE_PREFIX void OVERRIDABLE(OperatorTable_removeRecord)(OperatorTable * table, int recordIndex);

/** @} */

#include <provided/OperatorTable.h>
#include <user/OperatorTable.h>

#endif
