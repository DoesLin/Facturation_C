/*
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
 * $Id: CatalogDB.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogDB.h>
#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>

/** The catalog file name */
const char * CATALOGDB_FILENAME = BASEPATH "/data/Catalog.db";

/** Create a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_create)(const char * filename) {
    FILE *pfile;
    pfile = fopen(filename, "wb+");
    CatalogDB *cdb = malloc(sizeof(CatalogDB));
    cdb->file = pfile;
    cdb->recordCount = 0;
    fprintf(pfile, "%d", 0);
    return cdb;
//    return provided_CatalogDB_create(filename);
}

/** Open an existing database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_open)(const char * filename) {
    FILE *pfile;
    if(!(pfile = fopen(filename, "rb+"))){
        return NULL;
    }
    CatalogDB *cdb = malloc(sizeof(CatalogDB));
    cdb->file = pfile;
    fscanf(pfile, "%d", &(cdb->recordCount));
    return cdb;
//    return provided_CatalogDB_open(filename);
}

/** Open if exists or create otherwise a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_openOrCreate)(const char * filename) {
    FILE *pfile;
    CatalogDB *cdb = malloc(sizeof(CatalogDB));
    if(!(pfile = fopen(filename, "rb+"))){  //create
        pfile = fopen(filename, "wb+");
        cdb->recordCount = 0;
        cdb->file = pfile;
        fprintf(cdb->file, "%d", 0);
    }else{  //open
        fscanf(pfile, "%d", &(cdb->recordCount));
    }
    cdb->file = pfile;
    return cdb;
//    return provided_CatalogDB_openOrCreate(filename);
}

/** Flush cached data, close a database and free the structure representing the opened database
 * @param catalogDB a pointer on a CatalogDB representing the opened database
 */
void IMPLEMENT(CatalogDB_close)(CatalogDB * catalogDB) {
    if(fclose(catalogDB->file))
       fatalError("\nError on close file\n");
    free(catalogDB);
//    provided_CatalogDB_close(catalogDB);
}

/** Get the number of records of the database
 * @param catalogDB the database
 * @return the number of records
 */
int IMPLEMENT(CatalogDB_getRecordCount)(CatalogDB * catalogDB) {
    return catalogDB->recordCount;
//    return provided_CatalogDB_getRecordCount(catalogDB);
}

/** Create a new string on the heap containing the value of the specified field for the specified record of a database
 * @param catalogDB the database
 * @param recordIndex the record index
 * @param field the field to query
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * CatalogDB_getFieldValueAsString(CatalogDB * catalogDB, int recordIndex, int field) {
    char * content = NULL;
    if (catalogDB != NULL) {
        CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(field);
        CatalogRecord record;
        CatalogRecord_init(&record);
        CatalogDB_readRecord(catalogDB, recordIndex, &record);
        content = (*properties.getValue)(&record);
        CatalogRecord_finalize(&record);
    }
    return content;
}

/** Append the specified record at the end of the database
 * @param catalogDB the database
 * @param record the record
 */
void IMPLEMENT(CatalogDB_appendRecord)(CatalogDB * catalogDB, CatalogRecord *record) {
    CatalogDB_insertRecord(catalogDB, catalogDB->recordCount, record);
//    provided_CatalogDB_appendRecord(catalogDB, record);
}

/** Insert the specified record at the given position in the database
 * @param catalogDB the database
 * @param recordIndex the insertion position
 * @param record the record
 */
void IMPLEMENT(CatalogDB_insertRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record) {
    catalogDB->recordCount++;
    fseek(catalogDB->file, 0, SEEK_SET);
    fprintf(catalogDB->file, "%d", catalogDB->recordCount);

    CatalogRecord recordtmp;
    CatalogRecord_init(&recordtmp);
    for(int i = catalogDB->recordCount - 2;i >= recordIndex;i--){
        CatalogDB_readRecord(catalogDB, i, &recordtmp);
        CatalogDB_writeRecord(catalogDB, i + 1, &recordtmp);
    }
    CatalogRecord_finalize(&recordtmp);

    CatalogDB_writeRecord(catalogDB, recordIndex, record);
//    provided_CatalogDB_insertRecord(catalogDB, recordIndex, record);
}

/** Remove a record at a given position from the database
 * @param catalogDB the database
 * @param recordIndex the removal position
 */
void IMPLEMENT(CatalogDB_removeRecord)(CatalogDB * catalogDB, int recordIndex) {
    CatalogRecord recordtmp;
	CatalogRecord_init(&recordtmp);
	for (int i = recordIndex + 1; i < catalogDB->recordCount; i++) {
		CatalogDB_readRecord(catalogDB, i, &recordtmp);
		CatalogDB_writeRecord(catalogDB, i - 1, &recordtmp);
	}
	CatalogRecord_finalize(&recordtmp);

	catalogDB->recordCount--;
	fseek(catalogDB->file, 0, SEEK_SET);
	fprintf(catalogDB->file, "%d", catalogDB->recordCount);
//    provided_CatalogDB_removeRecord(catalogDB, recordIndex);
}

/** Read a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to read
 * @param record the record to fill with data
 */
void IMPLEMENT(CatalogDB_readRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record) {
    int whence =(int)sizeof(int) + recordIndex * (int)CATALOGRECORD_SIZE; //4 + n * ...
    fseek(catalogDB->file, whence, SEEK_SET);
    CatalogRecord_read(record, catalogDB->file);
//    provided_CatalogDB_readRecord(catalogDB, recordIndex, record);
}

/** Write a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to write
 * @param record the record containing the data
 */
void IMPLEMENT(CatalogDB_writeRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record) {
    if(catalogDB->recordCount < recordIndex + 1){
        catalogDB->recordCount++;
        fseek(catalogDB->file, 0, SEEK_SET);
        fprintf(catalogDB->file, "%d", catalogDB->recordCount);
    }

    int whence =(int)sizeof(int) + recordIndex * (int)CATALOGRECORD_SIZE; //4 + n * ...
    fseek(catalogDB->file, whence, SEEK_SET);
    CatalogRecord_write(record, catalogDB->file);
//    provided_CatalogDB_writeRecord(catalogDB, recordIndex, record);
}

