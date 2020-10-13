/** 
 * @file sqlsimpleAPI.h
 * @author HXQ(huangxinquan@matace.cn)
 * @brief
 * @version 1.0
 * @date 2020-9-6
 * 
 * @copyright
 * Copyright (c) 2010-2020, the copyright holders HXQ(huangxinquan@matace.cn)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided that
 * the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 
 * Neither the name of the copyright holders HXQ(huangxinquan@matace.cn) nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS
 * LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 
#ifndef _SQLSIMPLEAPI_H_
#define _SQLSIMPLEAPI_H_

#define USE_IN_LINUX

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

#ifdef USE_IN_LINUX
    #include <dlfcn.h>
    #include<unistd.h>
#endif

#ifdef USE_IN_WINDOWS
    #include <windows.h>
    #include<io.h>
#endif

#if 0 //def USE_IN_LINUX
typedef enum
{
    FALSE = 0,
    TRUE
}bool;
#endif

#define TRUE 1
#define FALSE 0

#pragma comment(linker," /NODEFAULTLIB:LIBC.lib")
#define DEBUG

#ifdef DEBUG
#define MSG(str,args) printf("**%s-%d** " str,__FILE__,__LINE__,##args)
#else
#define MSG(str,args)
#endif

#ifdef USE_IN_LINUX
static bool GetApi(const char* soname, void *handle);

static void CloseHandle(void *handle);
#endif

typedef int (*OPEN)(const char*, sqlite3**);
typedef int (*CLOSE)(sqlite3*);
typedef int (*EXEC)(sqlite3*, char*, int (*)(void *, int, 	char**, char**),
					void*, char**);
typedef int (*GET)(sqlite3*, const char *,char***, int *, int *, char**);
typedef void (*FTABLE)(char**);
typedef void (*FREE)(void*);
typedef int (*RELEASE)(int);

typedef int (*PREPARE)(sqlite3 *, const char *zSql, int, sqlite3_stmt **, const char **);
typedef int (*STEP)(sqlite3_stmt*);
typedef const char *(*ERRMSG)(sqlite3*);
typedef int (*FINALIZE)(sqlite3_stmt *);
typedef int (*RESET)(sqlite3_stmt *);
typedef int (*SHUTDOWN)(void);
typedef unsigned long (*MEM_USED)(void);
typedef int (*INIT)(void);
typedef int (*COMPLETE)(const char *sql);
#ifdef USE_IN_WINDOWS
static bool GetApi(const char* soname);
#endif

//==============================Useful Work Function==============================
/*db_name is the database you want to open and sql is the sql sentence to execute, this
**sql sentence should not be sql select sentence***************************/
int SQL_Exec(const char* db_name, const char* sql);

/*db_name is the database you want to open and sql is the sql sentence to execute, this
**sql sentence must be sql select sentence pResult is a two dimension char* array with
** the selected data from the database, nRow and nCol is the rows and columns of selected data***/
int SQL_Select(const char* db_name, const char* sql, char ***pResult, int *nRow, int *nCol);

//We use this function to free the memory allocated by pResult of SQL_Select function
void SQL_FreeMemory(char** result);

int SQL_OpenAndClose(const char* db_name);

/*convert utf-16 character to utf-8*/
unsigned char * Utf16CharacterToUTF8( int unicode, unsigned char *p);

/*convert UTF-16 string to UTF-8*/
unsigned char * UnicodeStrToUTF8Str (const unsigned short * unicode_str,
                           unsigned char * utf8_str, int utf8_str_size);

void release(int);
//========================================================================================

/*open database*/
static sqlite3* OpenDB(const char *szdb_file);

/*close database*/
static bool CloseDB(sqlite3* db);

/*exectute SQL sentence*/
static bool ExcuteSQL(sqlite3* db, const char *sql);

/*get data from database, the result is kept in pReslut, it contains nRow and nCol, there is sth wrong with this function
it can work rightly and cannot get the right result*/
static bool SelectFromTable(sqlite3* db,const char *sql, char ***pResult, int *nRow, int *nCol);

static int dbfile_exist(const char* dbname);

struct API
{
    int (*sqlite3_open)(const char*, sqlite3**);

    int (*sqlite3_close)(sqlite3*);

    int (*sqlite3_exec)(sqlite3*, char*,
                        int (*callback)(void*,int,char**,char**),void *,
                        char **errmsg);

    int (*sqlite3_get_table)(sqlite3 *db,
                            const char *zSql,
                            char ***pazResult,
                            int *pnRow,
                            int *pnColumn,
                            char **pzErrmsg);

    void (*sqlite3_free_table)(char **result);

    void (*sqlite3_free)(void*);

    int (*sqlite3_release_memory)(int);

	int (*sqlite3_prepare_v2)(
                    sqlite3 *db,            /* Database handle */
                    const char *zSql,       /* SQL statement, UTF-8 encoded */
                    int nByte,              /* Maximum length of zSql in bytes. */
                    sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
                    const char **pzTail     /* OUT: Pointer to unused portion of zSql */
                    );

    int (*sqlite3_step)(sqlite3_stmt*);

    const char *(*sqlite3_errmsg)(sqlite3*);

    int (*sqlite3_finalize)(sqlite3_stmt *pStmt);
    int (*sqlite3_reset)(sqlite3_stmt *pStmt);
    int (*sqlite3_shutdown)(void);
    unsigned long (*sqlite3_memory_used)(void);
    int (*sqlite3_initialize)(void);
    int (*sqlite3_complete)(const char *sql);
};

#endif  //end of #define _SQLSIMPLEAPI_H_
