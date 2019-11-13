//// SQLConnect_ref.cpp  
//// compile with: odbc32.lib  
//#include <windows.h>  
//#include <sqlext.h>  
//#include <iostream>
//using namespace std;
//
//int main() {
//	SQLHENV henv;
//	SQLHDBC hdbc;
//	SQLHSTMT hstmt;
//	SQLRETURN retcode;
//
//	SQLCHAR * OutConnStr = ( SQLCHAR * )malloc( 255 );
//	SQLSMALLINT * OutConnStrLen = ( SQLSMALLINT * )malloc( 255 );
//
//	// Allocate environment handle  
//	retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );
//
//	// Set the ODBC version environment attribute  
//	if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
//		retcode = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION, ( void* )SQL_OV_ODBC3, 0 );
//
//		// Allocate connection handle  
//		if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
//			retcode = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );
//
//			// Set login timeout to 5 seconds  
//			if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
//				SQLSetConnectAttr( hdbc, SQL_LOGIN_TIMEOUT, ( SQLPOINTER )5, 0 );
//
//				// Connect to data source  
//				retcode = SQLConnect( hdbc, ( SQLCHAR* ) "2015182004_KIMHAYOON", SQL_NTS, ( SQLCHAR* )NULL, 0, NULL, 0 );
//				
//				// Allocate statement handle  
//				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
//					retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );
//
//					// Process data  
//					if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
//					{
//						cout << "연결됨" << endl;
//						system( "pause" );
//						SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
//					}
//
//					SQLDisconnect( hdbc );
//				}
//
//				SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
//			}
//		}
//		SQLFreeHandle( SQL_HANDLE_ENV, henv );
//	}
//}


// SQLBindCol_ref.cpp  
// compile with: odbc32.lib  
#include <windows.h>  
#include <stdio.h>  

#define UNICODE  
#include <sqlext.h>  

#define NAME_LEN 11  

void show_error() {
	printf( "error\n" );
}

/************************************************************************ 
/* HandleDiagnosticRecord : display error/warning information 
/* 
/* Parameters: 
/*      hHandle     ODBC handle 
/*      hType       Type of handle (SQL_HANDLE_STMT, SQL_HANDLE_ENV, SQL_HANDLE_DBC) 
/*      RetCode     Return code of failing command 
/************************************************************************/ 
void HandleDiagnosticRecord( SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode ) {
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];
	if ( RetCode == SQL_INVALID_HANDLE )
	{
		fwprintf( stderr, L"Invalid handle!\n" );
		return;
	}
	while ( SQLGetDiagRec( hType, hHandle, ++iRec, wszState, &iError, wszMessage, ( SQLSMALLINT )( sizeof( wszMessage ) / sizeof( WCHAR ) ), ( SQLSMALLINT * )NULL ) == SQL_SUCCESS )
	{
		// Hide data truncated.. 
		if ( wcsncmp( wszState, L"01004", 5 ) )
		{
			fwprintf( stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError );
		}
	}
}


int main() {
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;
	SQLINTEGER nID, nLEVEL;
	SQLWCHAR szName[NAME_LEN];
	SQLLEN cbName = 0, cbID = 0, cbLEVEL= 0;

	// Allocate environment handle  
	retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );

	// Set the ODBC version environment attribute  
	if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
		retcode = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION, ( SQLPOINTER* )SQL_OV_ODBC3, 0 );

		// Allocate connection handle  
		if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
			retcode = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );

			// Set login timeout to 5 seconds  
			if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
				SQLSetConnectAttr( hdbc, SQL_LOGIN_TIMEOUT, ( SQLPOINTER )5, 0 );

				// Connect to data source  
				retcode = SQLConnect( hdbc, ( SQLWCHAR* )L"TEST", SQL_NTS, ( SQLWCHAR* )NULL, 0, NULL, 0 );

				// Allocate statement handle  
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
					retcode = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );

					retcode = SQLExecDirect( hstmt, ( SQLWCHAR * )L"SELECT c_name, n_id, c_LEVEL FROM user_data ORDER BY 2, 1, 3", SQL_NTS );

					if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

						// Bind columns 1, 2, and 3  
						retcode = SQLBindCol( hstmt, 1, SQL_UNICODE_CHAR, szName, NAME_LEN, &cbName);			// 컬럼 하나하나를 바인드해서 넣어주라
						retcode = SQLBindCol( hstmt, 2, SQL_C_LONG, &nID, 10, &cbID);
						retcode = SQLBindCol( hstmt, 3, SQL_C_LONG, &nLEVEL, 10, &cbLEVEL);

						// Fetch and print each row of data. On an error, display a message and exit.  
						for ( int i = 0; ; i++ ) {
							retcode = SQLFetch( hstmt );
							if ( retcode == SQL_ERROR )
								show_error();
							if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
								wprintf( L"%d: %d %lS %d\n", i + 1, nID, szName, nLEVEL);
							else
								break;
						}
					}

					else
					{
						HandleDiagnosticRecord( hstmt, SQL_HANDLE_DESC, retcode );
					}

					// Process data  
					if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
						SQLCancel( hstmt );
						SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
					}

					SQLDisconnect( hdbc );
				}

				SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
			}
		}
		SQLFreeHandle( SQL_HANDLE_ENV, henv );
	}
}