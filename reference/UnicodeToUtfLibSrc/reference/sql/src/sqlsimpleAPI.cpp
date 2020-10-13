/** 
 * @file sqlsimpleAPI.cpp
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
#include "sqlsimpleAPI.h"
#include <fcntl.h>

#define MAX_CHARACTER_SIZE 8
#define PATH_LENGTH 100
static int print = 0;
static int bApiOpen = 1;
static int bApiClose = 1;
//==============================Useful Work Function==============================
/*db_name is the database you want to open and sql is the sql sentence to execute, this
**sql sentence should not be sql select sentence***************************/
static struct API api;
//static int g_first_time;
#ifdef USE_IN_LINUX
static void *handle = NULL;
#endif

#ifdef USE_IN_WINDOWS
HINSTANCE handle;
#endif

int SQL_Exec(const char* db_name, const char* sql)
{
    if(NULL == db_name || NULL == sql)
        return false;

    int res = 1; //for the value to be returned
	sqlite3 *db = NULL;	//the database to be opened
	//returned data after execting sqlite function

    if((db = OpenDB(db_name)) == NULL)
    {
       return false;
    }
	//execute sql sentence
//	ExcuteSQL(db,"BEGIN TRANSACTION;");

    char *errr_msg = NULL;
    if(api.sqlite3_exec(db,(char*)sql,NULL,NULL,&errr_msg) != SQLITE_OK)
    {
    	res = 0;
       //return false;
    }

//close database
    if(false == CloseDB(db))
		printf("Some error happen in closeDB\n");

    if(0 == res)
    {
        return false;
    }
	return true;
}

/*db_name is the database you want to open and sql is the sql sentence to execute, this
**sql sentence must be sql select sentence***************************/
int SQL_Select(const char* db_name, const char* sql, char ***pResult, int *nRow, int *nCol)
{
    if(NULL == db_name || NULL == sql)
        return false;

    sqlite3 *db = NULL;	//the database to be opened
    int res = 1; //for the value to be returned

    if((db = OpenDB(db_name)) == NULL)
    {
    	return false;
    }

//execute select sentence
    if(false == SelectFromTable(db, sql, pResult, nRow, nCol))
    {
		res = 0;
        //return false;
    }

	//close database
    if(false == CloseDB(db));
		

    if(0 == res)
    {
        return false;
    }
    return true;
}

//We use this function to free the memory allocated by pResult of SQL_Select function
void SQL_FreeMemory(char** result)
{
    if(NULL != result)
    {
        (api.sqlite3_free_table)(result);
        result = NULL;
    }
}

void release(int n)
{
    (api.sqlite3_release_memory)(n);
}

//===============================inside use==========================================
/*open database*/
static sqlite3* OpenDB(const char *szdb_file)
{
	if(szdb_file == NULL)
        return NULL;
    #ifdef USE_IN_LINUX
    if(bApiOpen == 1)
    {

        bApiOpen = 0;
        if(false == GetApi("libsqlite3.so", handle))
        {
//            printf("add***%s(%d)of%s*****\r\n",__FUNCTION__,__LINE__,__FILE__);
            return NULL;
        }
     }
    #endif

    #ifdef USE_IN_WINDOWS
	if(bApiOpen == 1)
    {
		bApiOpen = 0;
		if(false == GetApi("sqlite3.dll"))
			return NULL;
	}
    #endif
    
if(!dbfile_exist(szdb_file))
{
	printf("dbfile not exist\n");
	return NULL;
};
			
	
	sqlite3 *db = NULL;	//the database to be opened
	char* zErr = NULL;		//error code,return NULL when successfully executed
	int rc;		//returned data after execting sqlite function

	(api.sqlite3_initialize)();

	rc = (api.sqlite3_open)(szdb_file, &db);

	if(rc != SQLITE_OK)
	{
		(api.sqlite3_close)(db);
		if(db != NULL)
		{
        #ifdef DEBUG
//            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        #endif
		    (api.sqlite3_free)(db);
		    db = NULL;
		}//end if
		return NULL;
	}//end if

    if(db != NULL)
    {
        return db;
    }
    else
    {
        return NULL;
    }

}

//close database
static bool CloseDB(sqlite3* db)
{
    if(db != NULL)
    {	
        if(SQLITE_OK != (api.sqlite3_close)(db))
        {
            printf("Some error happened in sqlite3_close()\n");
        }
		(api.sqlite3_shutdown)();
        
#ifdef USE_IN_LINUX
        if(bApiClose == 1)
        {
            bApiClose = 0;
            CloseHandle(handle);
        }
#endif
/*
#ifdef USE_IN_WINDOWS
		if(bApiClose == 1)
        {
//            bApiClose = 0;
			FreeLibrary(handle);
		}
#endif
*/


        return true;
    }
	return false;
}


#if 0
static bool ExcuteSQL(sqlite3* db, const char *sql)
{
	if((sql == NULL) && (NULL == db))
		return false;

	char* zErrMsg = NULL;		//error code,return NULL when successfully executed
	int rc;		//returned data after execting sqlite function

	rc=(api.sqlite3_exec)(db, (char*)sql, NULL, NULL, &zErrMsg);

	if(rc != SQLITE_OK)
		{
		    if(NULL != zErrMsg)
				{
				#ifdef DEBUG
                    printf("SQL error: %s\n",zErrMsg);
                #endif
                    (api.sqlite3_free)(zErrMsg);
                    zErrMsg = NULL;
				}//end if
				return false;
		}//end if
	else
		return true;
}
#else
static bool ExcuteSQL(sqlite3* db, const char *sql)
{
	if(NULL == db && NULL == sql)
		return false;
	int res;
    sqlite3_stmt *stmt = NULL;

    //insert data
    if(0 != api.sqlite3_prepare_v2(db, sql, -1, &stmt, 0))
	{
		return false;
	}

    res = api.sqlite3_step(stmt);
    if(res != SQLITE_DONE)
    {
        printf("%s\n", api.sqlite3_errmsg(db));
    }
    if(0 != api.sqlite3_finalize(stmt))
	{
		return false;
	}
	return true;
}
#endif

static bool SelectFromTable(sqlite3* db,const char *sql, char ***pResult, int *nRow, int *nCol)
{
	if(sql == NULL || db == NULL)
		return false;

    int resCode = 1;
	char *errr_msg = NULL;
	int rc;

	rc = (api.sqlite3_exec)(db,"BEGIN;",NULL,NULL,&errr_msg);
	if(rc != SQLITE_OK)
    {
        resCode = 0;
        if(NULL != errr_msg)
		{
		#ifdef DEBUG
            printf("SQL error: %s\n",errr_msg);
        #endif
			(api.sqlite3_free)(errr_msg);
			errr_msg = NULL;
		}//end if
    }//end if

	rc = (api.sqlite3_get_table)(db,sql,pResult, nRow, nCol, &errr_msg);
	if(rc != SQLITE_OK)
    {
        resCode = 0;
        if(NULL != errr_msg)
		{
		#ifdef DEBUG
            printf("SQL error: %s\n",errr_msg);
        #endif
			(api.sqlite3_free)(errr_msg);
			errr_msg = NULL;
		}//end if
		if(NULL != *pResult)
		{
		    (api.sqlite3_free_table)(*pResult);
		    pResult = NULL;
		}
    }//end if

	rc = api.sqlite3_exec(db,"COMMIT;",NULL,NULL,&errr_msg);
    if(rc != SQLITE_OK)
    {
        resCode = 0;
        if(NULL != errr_msg)
		{
		#ifdef DEBUG
            printf("SQL error: %s\n",errr_msg);
        #endif
			(api.sqlite3_free)(errr_msg);
			errr_msg = NULL;
		}//end if
    }//end if

    if(0 == resCode)
	{	
        return false;
	}
    else
	{
		return true;
	}
}

#ifdef USE_IN_WINDOWS
static bool GetApi(const char* soname)
{
//	HINSTANCE handle;               // Handle to DLL

	if(NULL == soname)
        return false;

	handle = LoadLibrary(soname);
	if( NULL == handle)
	{
    #ifdef DEBUG
        printf("cannot find so file\n");
    #endif
    return false;
	}

	api.sqlite3_open = (OPEN)GetProcAddress(handle, "sqlite3_open");
    if (NULL == api.sqlite3_open)
   {
      // handle the error
      FreeLibrary(handle);
      return FALSE;
   }

	api.sqlite3_close = (CLOSE)GetProcAddress(handle, "sqlite3_close");
    if (NULL == api.sqlite3_close)
   {
      // handle the error
      FreeLibrary(handle);
      return FALSE;
   }

	api.sqlite3_exec = (EXEC)GetProcAddress(handle, "sqlite3_exec");
    if (NULL == api.sqlite3_exec)
   {
      // handle the error
      FreeLibrary(handle);
      return FALSE;
   }

	api.sqlite3_get_table = (GET)GetProcAddress(handle, "sqlite3_get_table");
    if (NULL == api.sqlite3_get_table)
   {
      // handle the error
      FreeLibrary(handle);
      return FALSE;
   }

	api.sqlite3_free = (FREE)GetProcAddress(handle, "sqlite3_free");
    if (NULL == api.sqlite3_free)
   {
      // handle the error
      FreeLibrary(handle);
      return FALSE;
   }

	api.sqlite3_free_table = (FTABLE)GetProcAddress(handle, "sqlite3_free_table");
    if (NULL == api.sqlite3_free_table)
   {
      // handle the error
      FreeLibrary(handle);
      return FALSE;
   }
    api.sqlite3_shutdown = (SHUTDOWN)GetProcAddress(handle, "sqlite3_shutdown");
    if (NULL == api.sqlite3_shutdown)
    {
      // handle the error
		FreeLibrary(handle);
		return FALSE;
    }
    api.sqlite3_initialize = (INIT)GetProcAddress(handle, "sqlite3_initialize");
    if (NULL == api.sqlite3_initialize)
    {
      // handle the error
		FreeLibrary(handle);
		return FALSE;
    }
	
    api.sqlite3_prepare_v2 = (PREPARE)GetProcAddress(handle, "sqlite3_prepare_v2");
    if (NULL == api.sqlite3_prepare_v2)
    {
      // handle the error
		FreeLibrary(handle);
		return FALSE;
    }
    api.sqlite3_step = (STEP)GetProcAddress(handle, "sqlite3_step");
    if (NULL == api.sqlite3_step)
    {
      // handle the error
		FreeLibrary(handle);
		return FALSE;
    }
    api.sqlite3_errmsg = (ERRMSG)GetProcAddress(handle, "sqlite3_errmsg");
    if (NULL == api.sqlite3_errmsg)
    {
      // handle the error
		FreeLibrary(handle);
		return FALSE;
    }

    api.sqlite3_finalize = (FINALIZE)GetProcAddress(handle, "sqlite3_finalize");
    if (NULL == api.sqlite3_finalize)
    {
      // handle the error
		FreeLibrary(handle);
		return FALSE;
    }
	

	return TRUE;
}
#endif //end of USE_IN_WINDOWS

#ifdef USE_IN_LINUX
static bool GetApi(const char* soname, void *handle)
{
    if(NULL == soname)
        return false;

    char *error = NULL;
    handle = dlopen (soname, RTLD_LAZY);
	if (!handle)
	{
    #ifdef DEBUG
	    printf("so file cannot find %s\n", soname);
    #endif
	    return false;
    }

    api.sqlite3_open = (OPEN)dlsym(handle, "sqlite3_open");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_close = (CLOSE)dlsym(handle, "sqlite3_close");
     if ((error = dlerror()) != NULL)
     {
     	free(error);
		error = NULL;
         return false;
     }

    api.sqlite3_exec = (EXEC)dlsym(handle, "sqlite3_exec");
    if ((error = dlerror()) != NULL)
    {
		free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_get_table = (GET)dlsym(handle, "sqlite3_get_table");
    if ((error = dlerror()) != NULL)
    {
	    free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_free_table = (FTABLE)dlsym(handle, "sqlite3_free_table");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_free = (FREE)dlsym(handle, "sqlite3_free");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_release_memory = (RELEASE)dlsym(handle, "sqlite3_release_memory");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_prepare_v2 = (PREPARE)dlsym(handle, "sqlite3_prepare_v2");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
    api.sqlite3_step = (STEP)dlsym(handle, "sqlite3_step");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
    api.sqlite3_errmsg = (ERRMSG)dlsym(handle, "sqlite3_errmsg");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }

    api.sqlite3_finalize = (FINALIZE)dlsym(handle, "sqlite3_finalize");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
    api.sqlite3_reset = (RESET)dlsym(handle, "sqlite3_reset");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }

	api.sqlite3_memory_used = (MEM_USED)dlsym(handle, "sqlite3_memory_used");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
    api.sqlite3_shutdown = (SHUTDOWN)dlsym(handle, "sqlite3_shutdown");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
    api.sqlite3_initialize = (INIT)dlsym(handle, "sqlite3_initialize");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
/*    api.sqlite3_complete = (COMPLETE)dlsym(handle, "sqlite3_complete");
    if ((error = dlerror()) != NULL)
    {
    	free(error);
		error = NULL;
        return false;
    }
*/
    return true;
}

static void CloseHandle(void *handle)
{
    if(handle != NULL)
    {
        dlclose(handle);
    }
}
#endif //end of USE_IN_LINUX


unsigned char * Utf16CharacterToUTF8( int unicode, unsigned char *p)
{
	unsigned char *e = NULL;
	if((e = p))
	{
		if(unicode < 0x80)
	    {
			*e++ = unicode;
	    }
	     else if(unicode < 0x800)
	     {
	      /* <11011111> < 000 0000 0000> */
			*e++ = ((unicode >> 6) & 0x1f)|0xc0;
			*e++ = (unicode & 0x3f)|0x80;
	     }
	     else if(unicode < 0x10000)
	     {
			/* <11101111> <0000 0000 0000 0000> */
			*e++ = ((unicode >> 12) & 0x0f)|0xe0;
	        *e++ = ((unicode >> 6) & 0x3f)|0x80;
	        *e++ = (unicode & 0x3f)|0x80;
	      }
	      else if(unicode < 0x200000)
	      {
	         /* <11110111> <0 0000 0000 0000 0000 0000> */
	        *e++ = ((unicode >> 18) & 0x07)|0xf0;
	        *e++ = ((unicode >> 12) & 0x3f)|0x80;
	        *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
		  }
        else if(unicode < 0x4000000)
        {
            /* <11111011> <00 0000 0000 0000 0000 0000 0000> */
            *e++ = ((unicode >> 24) & 0x03)|0xf8 ;
            *e++ = ((unicode >> 18) & 0x3f)|0x80;
            *e++ = ((unicode >> 12) & 0x3f)|0x80;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
        }
        else
        {
            /* <11111101> <0000 0000 0000 0000 0000 0000 0000 0000> */
            *e++ = ((unicode >> 30) & 0x01)|0xfc;
            *e++ = ((unicode >> 24) & 0x3f)|0x80;
            *e++ = ((unicode >> 18) & 0x3f)|0x80;
            *e++ = ((unicode >> 12) & 0x3f)|0x80;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;
            *e++ = (unicode & 0x3f)|0x80;
        }
    }
    /* Return One step over the end of the utf-8 character buffer */
    return e;
}


unsigned char * UnicodeStrToUTF8Str (const unsigned short * unicode_str,
                            unsigned char * utf8_str, int utf8_str_size)
{
    int unicode = 0;
    unsigned char *e = NULL, *s = NULL;
    unsigned char utf8_ch[MAX_CHARACTER_SIZE];
    s = utf8_str;
    if ((unicode_str) && (s))
    {
        while ((unicode = (int) (*unicode_str++)))
        {
            memset (utf8_ch, 0, sizeof (utf8_ch));

            if ((e = Utf16CharacterToUTF8 (unicode, utf8_ch)) > utf8_ch)
            {
                *e = '\0';

                /* Judge whether exceed the destination buffer */
                if ((s - utf8_str + strlen ((const char *) utf8_ch)) >= utf8_str_size)
                {
                    return s;
                }
                else
                {
                    memcpy (s, utf8_ch, strlen ((const char *) utf8_ch));
                    s += strlen ((const char *) utf8_ch);
                    *s = '\0';
                }
            }
            else
            {
                /* Converting error occurs */
                return s;
            }
        }
    }

    return s;
}

static int dbfile_exist(const char* dbname)
{
  if(NULL == dbname)
  	return 0;
  	  
	int fd, num = 0;
	char* p = (char *)dbname;
//�ж��Ƿ�ǰĿ¼
	while('\0' != *p)
	{
    if(*p == '/')
    {
    	num++;
    	break;
    }
    p++;
	}
	//��ǰĿ¼
	if(num == 0)
	{
   return 1;
	}
	else
	{
		num = 0;
	}
	   
	p = (char *)dbname + strlen(dbname);
	while(*p != '/')
	{
		num++;
		p--;
	}
	char *file = (char*)malloc(sizeof(char)*(strlen(dbname)- num + 1));
	memset(file, 0, sizeof(char)*(strlen(dbname) - num + 1));
	strncpy(file, dbname, strlen(dbname) - num);
	fd = open(file, O_RDONLY);
	if(-1 == fd)
	{
		printf("file not exist\n");
		free(file);
		file = NULL;
		return 0;
	}
	else
	{
		close(fd);
	}
	
	if(NULL != file)
	{
		free(file);
		file == NULL;
	}

	return 1;

}

/*
int SQL_OpenAndClose(const char* db_name)
{
	if(db_name == NULL)
       	return false;

	sqlite3 *db = NULL;	//the database to be opened
	//returned data after execting sqlite function

    if((db = OpenDB(db_name)) == NULL)
    {
    	printf("Some error happen in openDB\n");
       	return false;
    }
	printf("database just opened, now close\n");
	 if(false == CloseDB(db))
	 {
		printf("Some error happen in closeDB\n");
		return false;
	 }
	 return true;
}
*/
