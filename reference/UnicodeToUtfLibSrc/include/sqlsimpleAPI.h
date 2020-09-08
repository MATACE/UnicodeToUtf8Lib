/********************************************************************
  Copyright, 2012, Kinco (Shanghai) Ltd.
  All rights reserved.

  FileName: sqlsimpleAPI.h
  Description:定义sqlite数据调用的接口

  Revision history:
      <author>  	<time>  	 	<version >   	<desc>
       lianxx       2012/06 	     0.1
********************************************************************/
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
typedef int (*RELEASE)(int); //2012-10-10

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

//==============================工作中可能用得比较多的函数()==============================
/*db_name is the database you want to open and sql is the sql sentence to execute, this
**sql sentence should not be sql select sentence***************************/
int SQL_Exec(const char* db_name, const char* sql);

/*db_name is the database you want to open and sql is the sql sentence to execute, this
**sql sentence must be sql select sentence pResult is a two dimension char* array with
** the selected data from the database, nRow and nCol is the rows and columns of selected data***/
int SQL_Select(const char* db_name, const char* sql, char ***pResult, int *nRow, int *nCol);

//We use this function to free the memory allocated by pResult of SQL_Select function
void SQL_FreeMemory(char** result);

int SQL_OpenAndClose(const char* db_name); // lxx add for debug

/*convert utf-16 character to utf-8*/
unsigned char * Utf16CharacterToUTF8( int unicode, unsigned char *p);

/*convert UTF-16 string to UTF-8*/
unsigned char * UnicodeStrToUTF8Str (const unsigned short * unicode_str,
                           unsigned char * utf8_str, int utf8_str_size);

void release(int);       // lxx add 2012-10-10
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

	//lxx add for debug
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
	//lxx add for debug
};

#endif  //end of #define _SQLSIMPLEAPI_H_
