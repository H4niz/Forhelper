/*
          *****************
          *Author: h4niz  *
          *****************
*/

#ifndef BUILTINLIB_H_INCLUDED
#define BUILTINLIB_H_INCLUDED
#define _GNU_SOURCE //using getline() getdelim()

#include <windows.h>
#include <Lmcons.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "time.h"
#include <cstdlib>
#include <iostream>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "tinyxml2.h"



#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
/* Macros for prettier code. */
#ifndef MAX_PATH
#   define MAX_PATH _MAX_PATH
#endif

#define DIR "Data"
#define registryDIR "Data/Registry"
#define processDIR "Data/Processes"
#define serviceDIR "Data/Services"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define ARRAY_SIZE 1024

#define RUN ("REG QUERY \"HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\" /s >> Data/Registry/registry.txt")
#define WINLOGON ("REG QUERY \"HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\" /s >> Data/Registry/registry.txt")
#define WINDOWS ("REG QUERY \"HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\" /s >> Data/Registry/registry.txt")
#define DRIVER32 "REG QUERY \"HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32\" /s >> Data/Registry/registry.txt"
#define URUN ("REG QUERY \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\" /s >> Data/Registry/registry.txt")
#define URUNONCE ("REG QUERY \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce\" /s >> Data/Registry/registry.txt")


/*================== DEFINE SUB-FUNCTIONS ===================   */

/*Handle path Exception like: %temp%, %...*/
void HandlePath(char * path, char * ex)    {
    char *buf, temp[2048], t[2048], sig[1048], newpath[2048];
    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    strcpy( temp, path);
    strcpy( t, path);

    buf = strtok(temp, "%");

    while( buf != NULL) {
        strcpy(sig, temp);
        buf = strtok(NULL, "%");
    }

////    std::cout << "Done %" << "\n";
//    std::cout << "Sig: " << sig << "\n";
    memset(newpath, 0, strlen(newpath));
    if( strcmp( strlwr(sig), "%windir") == 0 || strcmp( strlwr(sig), "%systemroot") == 0 )   {
        strcpy( newpath, "C:\\Windows\\");
    }
    else if ( strcmp( strlwr(sig), "%programfiles" ) == 0) {
        strcpy( newpath, "C:\\Program Files\\");
    }
    else if(strcmp( strlwr(sig), "%localappdata") == 0 )   {
        GetUserName(username, &username_len);
        strcpy(newpath, "C:\\Users\\");
//        std::cout << "UserName: " << username << "\n";
        strcat(newpath, username);
        strcat(newpath, "\\AppData\\Local\\");
    }
    else if (strncmp(t, "\\", 1) == 0 )    {
        strcpy(newpath, "C:");
    }
    else {
        strncpy(newpath, t, 2);
    }

    buf = strchr(t,92);
    strcat(newpath, buf);
    strcpy(ex, newpath);
//    std::cout<< buf << "\n" << newpath << "\n";
//    getchar();

}

/*DumpBinary function-- This dump file binary from path*/
void dumpBinary(char *oldpath, char *newpath, char *func )   {
    char command[1024];

//    system(command);
    FILE *fCopy;
    fCopy = fopen(".\\Data\\copylog.txt", "a+");
    DWORD RT;
    char *ppch, path[2048];

    strcpy(path, oldpath);
    ppch = strtok(oldpath, "\"");
//    std::cout << path << std::endl;

    if(ppch != NULL )   {
        //ppch = strstr(name, "C:");
        ppch = strtok(ppch, "\"");
        strcpy( path, ppch );
        if (ppch != NULL)   {
            strcpy( path, ppch );
        }

        ppch = strtok(ppch, "\"");
        if (ppch != NULL)   {
                strcpy( path, ppch );
        }
    }
//    std::cout << path << std::endl;
    strcpy(command, path);
    memset(path, 0, strlen(path));
    HandlePath( command, path );
//    std::cout << "->  " << path << "\n";
//    getchar();
///*Handle*/
//    strcpy(command, "xcopy /F /Y /R ");
//    strcat(command, oldpath);
//    strcat(command, " ");
//    strcat(command, newpath);
//    strcat(command, " >> Data\\copylog.txt");

    /*Copy Options*/
    if ( CopyFile(path, newpath, FALSE) == 0 ) {
            RT = GetLastError();
            if ( RT == ERROR_ACCESS_DENIED) {
                fprintf( fCopy, "\n[x] %s - File at %s access was denied!\n", func, path);
//
//                fprintf( fCopy, "    [+]Trying... ");
//                system( command );
//                fprintf( fCopy, "\n" );
            }
            else if ( RT == ERROR_FILE_NOT_FOUND)   {
                fprintf( fCopy, "\n[x] %s - File at %s was not found!\n", func, path);
//
//                fprintf( fCopy, "    [+]Trying... ");
//                system( command );
//                fprintf( fCopy, "\n" );
            }
            else {
                fprintf(fCopy, "\n[x] %s - Cannot copy file: %s - Undetermind the reason\n", func, path);
//
//                fprintf( fCopy, "    [+]Trying... ");
//                system( command );
//                fprintf( fCopy, "\n" );
            }
    }
    fclose( fCopy );
}

/*Copy all file in determind folder*/
void CopyFolder(char *fPath, char *de)   {
    HANDLE fHandle;
    WIN32_FIND_DATA FileDetails;
    char name[1024] = {0}, filepath[2048] = {0}, des[2048] = {0}, folderpath[2048] = {0};

    FILE *fLog;
    fLog = fopen(".\\Data\\copylog.txt", "a+");
//    std::cout << "Preparing for copy" << "\n";
    strcpy( folderpath, fPath );
    strcat( folderpath, "*");
//    getchar();


    fHandle = FindFirstFileA(folderpath, &FileDetails);
//    std::cout << "File name: " << FileDetails.cFileName << "\n";

    if( !fHandle ) fprintf(fLog, "FolderCopy - Folder not found!\n\t[+]Path: %s", folderpath);
    FindNextFile(fHandle, &FileDetails);

    while( FileDetails.cFileName )    {
        FindNextFile(fHandle, &FileDetails);
//        std::cout << "Next file: " << FileDetails.cFileName << "\n";
        if(strcmp(FileDetails.cFileName, name) == 0) {
            break;
        }

        strcpy(name, FileDetails.cFileName);

        /*Handle to copy*/
        strcpy(filepath, fPath);
        strcat( filepath, name);
        strcpy(des, de);
        strcat( des, name);
        /*-----*/

//        std::cout << "FilePath: " << filepath << "\n";
//        fprintf(x, "File path: %s\n", filepath);
        dumpBinary(filepath, des, "CopyFolder");
    }

    fclose(fLog);
}

// Show time use for writeLog()
char *showTime()   {
    time_t t;
    time(&t);
    return ctime(&t);
}

//Write log with parameter
void writeLog(char *info)   {
    FILE *log;
    log = fopen("Data\\log.txt", "a+");
    fprintf(log, "[+] Time %s\t\t\t |->", showTime());
    fprintf( log, "\tAction: %s\n", info);
    fclose(log);
}
// Search each process in the snapshot for id.
BOOL FindProcessID(HANDLE snap, DWORD id, LPPROCESSENTRY32 ppe)
{
    BOOL fOk;
    ppe->dwSize = sizeof(PROCESSENTRY32);
    for (fOk = Process32First( snap, ppe ); fOk; fOk = Process32Next( snap, ppe ))
        if (ppe->th32ProcessID == id)
            break;
    return fOk;
}

// Obtain the process and thread identifiers of the parent process.
BOOL ParentProcess(LPPROCESS_INFORMATION ppi, DWORD id)
{
    HANDLE hSnap;
    PROCESSENTRY32 pe;
    THREADENTRY32   te;
//    DWORD id = GetCurrentProcessId();
    BOOL fOk;

    hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS|TH32CS_SNAPTHREAD, id );

    if (hSnap == INVALID_HANDLE_VALUE)
        return FALSE;

    FindProcessID( hSnap, id, &pe );
    if (!FindProcessID( hSnap, pe.th32ParentProcessID, &pe ))
    {
        CloseHandle( hSnap );
        return FALSE;
    }

    te.dwSize = sizeof(te);
    for (fOk = Thread32First( hSnap, &te ); fOk; fOk = Thread32Next( hSnap, &te ))
        if (te.th32OwnerProcessID == pe.th32ProcessID)
            break;

    CloseHandle( hSnap );

    ppi->dwProcessId = pe.th32ProcessID;
    ppi->dwThreadId = te.th32ThreadID;

    return fOk;
}
/*====================  END  ====================*/


/*==================== CORE FUNCTIONS ======================*/
/*List ScheduleTask*/
void listScheduleTask() {
    system("schtasks /query /xml > Data\\Processes\\raw");
    writeLog( "List Schedule tasks" );
}

/*List connection into file using commandline*/
void connection()   {
    writeLog( "List Connections" );
    system("netstat -aon > Data\\Processes\\connections.txt");
}

void connectionEx()   {
    writeLog("List Connection with option -abon");
    system("netstat -abon > Data\\Processes\\connectionsEx.txt");
}

/*Enumerate Drivers*/
void enumDriver()   {
   LPVOID drivers[ARRAY_SIZE];
   DWORD cbNeeded;
   char *pch, name[1024], path[5120], des[1024];
   wchar_t log[5120];
   int cDrivers, i;
   FILE *fdriver;

   fdriver = fopen("Data\\Services\\drivers.txt", "a+");

   writeLog( "Enumerate drivers" );


   if( EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))   {
      TCHAR szDriver[ARRAY_SIZE];

      cDrivers = cbNeeded / sizeof(drivers[0]);
      /*Total Driver*/
      fprintf( fdriver, "Total: %d\n\n", cDrivers);

      for (i=0; i < cDrivers; i++ )      {
         if(GetDeviceDriverFileName(drivers[i], szDriver, sizeof(szDriver)/sizeof(szDriver[0])))         {
            fprintf( fdriver, TEXT("%s\n"), szDriver);

            /*This snipet try to convert path*/
              strcpy( name, szDriver );
              pch = strstr (name, "C:");
              if ( pch != NULL )    {
                strcpy( path, pch );
              }
              else {
                strcpy( name, szDriver );
                pch = strtok (name, "\\");


                  if (pch != NULL ) {
                      strcpy(path, "C:\\Windows");
                      pch = strtok (NULL, "\\");
                      while (pch != NULL)  {
                        strcat(path, "\\");
                        strcat(path, pch);
                        strcpy(name, pch);
                        pch = strtok (NULL, "\\");
                      }
                  }
              }

              /*---*/
         }
         strcpy( des, ".\\Data\\Services\\drivers\\");
         strcat( des, name );
         fprintf( fdriver, TEXT("   -> Des: %s\n   -> Path: %s\n"), des, path);
         dumpBinary(path, des, "Drivers" );
      }
   }
//   fprintf( fdriver, "%d", GetLastError());
    fclose(fdriver);
}

int listService() {
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    LPQUERY_SERVICE_CONFIG lpsc;
    LPSERVICE_DESCRIPTION lpsd;
    DWORD dwBytesNeeded, cbBufSize, dwError;
    void* buf = NULL;
    DWORD bufSize = 0;
    DWORD moreBytesNeeded, serviceCount;
    FILE *fSvc, *fPath, *fSvcNonDes, *fErr;
    int svCount=0, NonSvCount=0;
    char path[100000], des[4048], name[1000000], p[1024];
    char *pch, x[1024], *ppch, s[1024], *temp, padding[1024];


    writeLog( "List Services" );

    fSvc = fopen( "Data\\Services\\servicedetails.txt", "a+");
    fPath = fopen( "Data\\Services\\path.txt", "a+");
    fErr = fopen( "Data\\Services\\error.txt", "a+");

    // Get a handle to the SCM database.

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database
        SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)    {
        fprintf( fErr, "OpenSCManager failed (%d): NULL == schSCManager\n", GetLastError());
        return 0;
    }

    // Get a handle to the service.
      for (;;) {
        if (EnumServicesStatusEx(
            schSCManager,
            SC_ENUM_PROCESS_INFO,
            SERVICE_WIN32,
            SERVICE_STATE_ALL,
            (LPBYTE)buf,
            bufSize,
            &moreBytesNeeded,
            &serviceCount,
            NULL,
            NULL)) {
          ENUM_SERVICE_STATUS_PROCESS* services = (ENUM_SERVICE_STATUS_PROCESS*)buf;


          for (DWORD i = 0; i < serviceCount; ++i) {
            if(svCount <= serviceCount) {
                              /*Total service*/
                fprintf( fSvc, "Total: %d\n\n", serviceCount);
                svCount = serviceCount;
                }

            fprintf( fSvc, "%s\n", services[i].lpServiceName);

                    schService = OpenService(
                    schSCManager,          // SCM database
                    services[i].lpServiceName,             // name of service
                    SERVICE_QUERY_CONFIG); // need query config access

                if (schService == NULL)    {
                    fprintf( fErr, "OpenService failed (%d): schService == NULL\n", GetLastError());
                    CloseServiceHandle(schSCManager);
                    return 0;
                }

                // Get the configuration information.

                if( !QueryServiceConfig(
                    schService,
                    NULL,
                    0,
                    &dwBytesNeeded))  {
                    dwError = GetLastError();
                    if( ERROR_INSUFFICIENT_BUFFER == dwError )   {
                        cbBufSize = dwBytesNeeded;
                        lpsc = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LMEM_FIXED, cbBufSize);
                    }else   {
                        fprintf( fErr, "QueryServiceConfig failed (%d): ERROR_INSUFFICIENT_BUFFER == dwError\n", dwError);
                        CloseServiceHandle(schService);
                        CloseServiceHandle(schSCManager);
                        return 0; //4/4
                    }
                }

                if( !QueryServiceConfig(
                    schService,
                    lpsc,
                    cbBufSize,
                    &dwBytesNeeded) )    {
                    fprintf( fErr, "QueryServiceConfig failed (%d): QueryServiceConfig(schService,lpsc,cbBufSize,&dwBytesNeeded)", GetLastError());
                    CloseServiceHandle(schService);
                    CloseServiceHandle(schSCManager);
                    return 0; //4/4
                }

                if( !QueryServiceConfig2(
                    schService,
                    SERVICE_CONFIG_DESCRIPTION,
                    NULL,
                    0,
                    &dwBytesNeeded))  {
                    dwError = GetLastError();
                    if( ERROR_INSUFFICIENT_BUFFER == dwError )        {
                        cbBufSize = dwBytesNeeded;
                        lpsd = (LPSERVICE_DESCRIPTION) LocalAlloc(LMEM_FIXED, cbBufSize);
                    }else   {
                        fprintf( fErr, "QueryServiceConfig2 failed (%d)", dwError);
                        CloseServiceHandle(schService);
                        CloseServiceHandle(schSCManager);
                        return 0; //4/4
                    }
                }

                if (! QueryServiceConfig2(
                    schService,
                    SERVICE_CONFIG_DESCRIPTION,
                    (LPBYTE) lpsd,
                    cbBufSize,
                    &dwBytesNeeded) )    {
                    fprintf( fErr, "QueryServiceConfig2 failed (%d)", GetLastError());
                    CloseServiceHandle(schService);
                    CloseServiceHandle(schSCManager);
                    return 0; //4/4
                }

                // Print the configuration information.
                fprintf( fSvc, TEXT("[%d]Service name: %s \n"), i, services[i].lpServiceName);
                fprintf( fSvc, TEXT("  Type: 0x%x\n"), lpsc->dwServiceType);
                fprintf( fSvc, TEXT("  Start Type: 0x%x\n"), lpsc->dwStartType);
                fprintf( fSvc, TEXT("  Error Control: 0x%x\n"), lpsc->dwErrorControl);
                fprintf( fSvc, TEXT("  Command: %s\n"), lpsc->lpBinaryPathName);
                fprintf( fSvc, TEXT("  Account: %s\n"), lpsc->lpServiceStartName);
                svCount++;

                if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
                    fprintf( fSvc, TEXT("  Description: %s\n"), lpsd->lpDescription);
                else {
                    /*This binary have no description, noting it*/
                    fSvcNonDes = fopen( "Data\\Services\\nonDescripttionSvc.txt", "a+");

                    fprintf( fSvcNonDes, TEXT("[+]Service name: %s \n"), services[i].lpServiceName);
                    fprintf( fSvcNonDes, TEXT("  Type: 0x%x\n"), lpsc->dwServiceType);
                    fprintf( fSvcNonDes, TEXT("  Start Type: 0x%x\n"), lpsc->dwStartType);
                    fprintf( fSvcNonDes, TEXT("  Error Control: 0x%x\n"), lpsc->dwErrorControl);
                    fprintf( fSvcNonDes, TEXT("  Command: %s\n"), lpsc->lpBinaryPathName);
                    fprintf( fSvcNonDes, TEXT("  Account: %s\n"), lpsc->lpServiceStartName);
                    NonSvCount++;

                    if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
                        fprintf( fSvcNonDes, TEXT("  Description: %s\n"), lpsd->lpDescription);
                    if (lpsc->lpLoadOrderGroup != NULL && lstrcmp(lpsc->lpLoadOrderGroup, TEXT("")) != 0)
                        fprintf( fSvcNonDes, TEXT("  Load order group: %s\n"), lpsc->lpLoadOrderGroup);
                    if (lpsc->dwTagId != 0)
                        fprintf( fSvcNonDes, TEXT("  Tag ID: %d\n"), lpsc->dwTagId);
                    if (lpsc->lpDependencies != NULL && lstrcmp(lpsc->lpDependencies, TEXT("")) != 0)
                        fprintf( fSvcNonDes, TEXT("  Dependencies: %s\n"), lpsc->lpDependencies);
                    fprintf( fSvcNonDes, TEXT("\t\t====================\n") );

                    fclose( fSvcNonDes );
                }

                if (lpsc->lpLoadOrderGroup != NULL && lstrcmp(lpsc->lpLoadOrderGroup, TEXT("")) != 0)
                    fprintf( fSvc, TEXT("  Load order group: %s\n"), lpsc->lpLoadOrderGroup);
                if (lpsc->dwTagId != 0)
                    fprintf( fSvc, TEXT("  Tag ID: %d\n"), lpsc->dwTagId);
                if (lpsc->lpDependencies != NULL && lstrcmp(lpsc->lpDependencies, TEXT("")) != 0)
                    fprintf( fSvc, TEXT("  Dependencies: %s\n"), lpsc->lpDependencies);

                /*Handle*/
//                 char *ppch;
                /*This snipet try to convert path*/
                strcpy(path, ""); //NUll
                strcpy(name, lpsc->lpBinaryPathName);
                pch = strtok ( name, ".");
                int x = 0;
                while (pch != NULL)  {
                    if (x >2 ) strcat( path, ".");
                    strcat(path, pch);
                    if (x == 0) strcat( path, ".");
                    pch = strtok (NULL, ".");
                    x++;
                }
                strcpy( name, lpsc->lpBinaryPathName);
                ppch = strstr( name, ".exe");
                pch = strtok (ppch, " ");
                if( pch != NULL) {
                    strcat( path, pch );
                }
                else {
                    strcat(path, ".exe" );
                }
                ppch = strtok(name, "\"");
                if(ppch != NULL )   {
                    //ppch = strstr(name, "C:");
                    ppch = strtok(ppch, "\"");
                    if (ppch != NULL)   {
                        strcpy( path, ppch );
                    }

                     ppch = strtok(ppch, "\"");
                    if (ppch != NULL)   {
                        strcpy( path, ppch );
                    }

                }

                    /*---*/

                fprintf( fPath, " (%d)Handle Path: %s\n     OriginPath: %s\n", i, path, lpsc->lpBinaryPathName);
                strcpy( des, "Data\\Services\\services\\" );
                strcat( des, services[i].lpServiceName);
                strcat( des, ".exe" );

//                Dump Binary
                dumpBinary( path, des, "Services" );

                fprintf( fSvc, TEXT("  CopyPath: %s\n"), path);
                fprintf( fSvc, TEXT("  Destination: %s\n"), des);
                fprintf( fSvc, TEXT("\t\t====================\n") );

                LocalFree(lpsc);
                LocalFree(lpsd);
            CloseServiceHandle(schService);

          }
          free(buf);

          /*Write total Non-description to file*/
          fSvcNonDes = fopen( "Data\\Services\\nonDescripttionSvc.txt", "a+");
          fprintf( fSvcNonDes, "\nTotal: %d", NonSvCount);
          fclose( fSvcNonDes);
          return 0;
        }
        int err = GetLastError();
        if (ERROR_MORE_DATA != err) {
                fprintf( fErr, "Error occur");
          free(buf);
          return err;
        }
        bufSize += moreBytesNeeded;
        free(buf);
        buf = malloc(bufSize);
      }

    fclose( fSvc );
    fclose( fPath );
    fclose( fErr );

    CloseServiceHandle(schSCManager);
}

int openDir()   {
    system("start .\\Data");
    writeLog( "OpenDir" );
}

//Enum single Registry
int enumReg(CHAR* COMMAND)  {
    system(COMMAND);
    return 0;
}

//Enum all registry
void enumAllreg()   {
    writeLog( "Enumerate registry" );
    FILE *reg;

    reg = fopen("Data\\Registry\\registry.txt", "a+");
    fprintf( reg, "\t\t=============* ^RUN^ *=============\n" );
    fclose(reg);

    enumReg(RUN);
    reg = fopen("Data\\Registry\\registry.txt", "a+");
    fprintf( reg, "\t\t=============* ^WINLOGON^ *=============\n" );
    fclose(reg);


    enumReg(WINLOGON);
    reg = fopen("Data\\Registry\\registry.txt", "a+");
    fprintf( reg, "\t\t=============* ^WINDOWS^ *=============\n" );
    fclose(reg);


    enumReg(WINDOWS);
    reg = fopen("Data\\Registry\\registry.txt", "a+");
    fprintf( reg, "\t\t=============* ^DRIVER32^ *=============\n" );
    fclose(reg);


    enumReg(DRIVER32);
    reg = fopen("Data\\Registry\\registry.txt", "a+");
    fprintf( reg, "\t\t=============* ^URUN^ *=============\n" );
    fclose(reg);


    enumReg(URUN);
    reg = fopen("Data\\Registry\\registry.txt", "a+");
    fprintf( reg, "\t\t=============* ^URUNONCE^ *=============\n" );
    fclose(reg);


    enumReg(URUNONCE);
}

//Enum Threads
BOOL ListProcessThreads( FILE *lThreads, DWORD dwOwnerPID, int cProcesses ) {
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
  THREADENTRY32 te32;

  // Take a snapshot of all running threads
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
  if( hThreadSnap == INVALID_HANDLE_VALUE )
    return( FALSE );

  // Fill in the size of the structure before using it.
  te32.dwSize = sizeof(THREADENTRY32 );

  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) )    {
    fprintf( lThreads,("Thread32First") );  // Show cause of failure
    CloseHandle( hThreadSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process
  do    {
    if( te32.th32OwnerProcessID == dwOwnerPID )    {
      fprintf( lThreads, ("  [*]THREAD ID      = %d\n"), te32.th32ThreadID );
      fprintf( lThreads, ("\t[+] Usage  = %d\n"), te32.cntUsage );
      fprintf( lThreads, ("\t[+] Owner PID  = %d\n"), te32.th32OwnerProcessID );
      fprintf( lThreads, ("\t[+] Base priority  = %d\n"), te32.tpBasePri );
      fprintf( lThreads, ("\t[+] Delta priority = %d\n\n\t--------------------\n"), te32.tpDeltaPri );
//      fprintf( lThreads, ("THREAD ID: %d\n"), te32.th32ThreadID );
    }
  } while( Thread32Next(hThreadSnap, &te32 ) );


//  Don't forget to clean up the snapshot object.
  CloseHandle( hThreadSnap );
  return( TRUE );
}

//Enum Drivers
#define ARRAY_SIZE 1024
void printError( TCHAR* msg, CHAR* Directory, TCHAR* att )  {
  DWORD eNum;
  TCHAR sysMsg[256];
  CHAR tcDirectory[30];
  TCHAR* p;
  FILE *er;

  strcpy(tcDirectory, Directory);
  strcat(tcDirectory, "/errors.txt");
  er = fopen(tcDirectory, "a+");
  fprintf(er, "|\t\t\t---ERROR LOG---\n");
  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  fprintf( er, ("\n  WARNING at %s: %s failed with error %d (%s)"), att, msg, eNum, sysMsg );
  fclose(er);
}

// ProcessModule
struct pidInfo {
    DWORD Processid;
    DWORD Parentid;
    char ProcessName[1024];
};

/*The functions just list a single record of each passing process*/
BOOL ListProcessModules( FILE *lProcessModule, DWORD dwPID )  {
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;
  PROCESSENTRY32 pe;
  char cmdl[1024], des[1024];
  unsigned int number=0, n=0;

//  pidInfo pi[2048];

//  Take a snapshot of all modules in the specified process.
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
  if( hModuleSnap == INVALID_HANDLE_VALUE )  {
    printError( TEXT("CreateToolhelp32Snapshot (of modules)"), (processDIR), me32.szModule );
    CloseHandle( hModuleSnap );
    return( FALSE );
  }

//  Set the size of the structure before using it.
  me32.dwSize = sizeof( MODULEENTRY32 );

//  Retrieve information about the first module,
//  and exit if unsuccessful
  if( !Module32First( hModuleSnap, &me32 ) )    {
    printError( TEXT("Module32First"), (processDIR), me32.szModule );  // Show cause of failure
    CloseHandle( hModuleSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

//  Now walk the module list of the process,
//  and display information about each module
  do  {
    if( strncmp("ForHelper4.exe", me32.szModule, 7) )     {
        if( number == 0)  {
                fprintf( lProcessModule, ("[*]PROCESS:     %20s\n"), me32.szExePath );
        }
        fprintf( lProcessModule, ("\n\t  [+]Module name:     %s\n"),            me32.szModule );
        fprintf( lProcessModule, ("\t\t| Executable     = %s\n"),             me32.szExePath );
        fprintf( lProcessModule, ("\t\t| Process ID     = %d\n"),         me32.th32ProcessID );
        fprintf( lProcessModule, ("\t\t| Ref count (g)  =     0x%04X\n"),     me32.GlblcntUsage );
        fprintf( lProcessModule, ("\t\t| Ref count (p)  =     0x%04X\n"),     me32.ProccntUsage );
        fprintf( lProcessModule, ("\t\t| Base address   = 0x%08X\n"),         me32.modBaseAddr );
        fprintf( lProcessModule, ("\t\t| Base size      = %d\n"),             me32.modBaseSize );

        if (number == 0)    {
            strcpy( cmdl, "wmic process where caption=\"");
            strcat( cmdl, me32.szModule);
            strcat( cmdl, "\" get processid, caption, commandline,creationclassname, status /format:list >> Data\\Processes\\commandline.txt");
            system(cmdl);
        }
    }

    number++;
    // Check for dump binary?

            /*Make newfileName then copy*/
    strcpy( des, ".\\Data\\Processes\\binary\\" );
    strcat( des, me32.szModule );
    if( strncmp("ForHelper4.exe", me32.szModule, 7) ) {
        dumpBinary( me32.szExePath, des, "Processes" );
    }

  } while( Module32Next( hModuleSnap, &me32 ) );
    /*Count for Process*/
    n++;

    fprintf( lProcessModule, "\t\t---------------------\n\n");

//  Do not forget to clean up the snapshot object.
  CloseHandle( hModuleSnap );
  return( TRUE );
}


/*Print ProcessTree*/

/*---*/

/*Print ProcessName*/
void PrintProcessName( DWORD processID, char *ProcessName )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.

    strcpy( ProcessName, szProcessName );

    // Release the handle to the process.

    CloseHandle( hProcess );
}
/*--*/
//Print Modules Process
/*List all running process then pass to ListProcessModules() to return */
printProcessModules()   {
    // Get the list of process identifiers.
    writeLog( "List processes modules" );

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    CHAR cmdl[1024]= {0}, ProcessName[1024]={0};
    FILE *lProcessModule, *fPID;
    lProcessModule = fopen(".\\Data\\Processes\\processes.txt", "a+");
    fPID = fopen( ".\\Data\\Processes\\pid.txt", "a+" );

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
        writeLog( "Can not EnumProcesses\n" );
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    fprintf( lProcessModule, ("Total: %d\n\n"), cProcesses);

    for ( i = 0; i < cProcesses; i++ )  {
        if( aProcesses[i] != 0 )    {
            ListProcessModules( lProcessModule, aProcesses[i] );
            PrintProcessName( aProcesses[i], ProcessName );
//              fprintf( lProcessModule, ("pID: %d\n\n"), aProcesses[i]);
        }

        PROCESS_INFORMATION parentInformation;
        if( strncmp( "ForHelper4.exe", ProcessName, 7) ) {
            if(!ParentProcess(&parentInformation, aProcesses[i])) {
                fprintf( fPID, TEXT("[%d]ProcessName: %s - Pid: %d - ParentID: %d\n"), i, ProcessName, aProcesses[i], 0);
    //            pi[i].Processid = aProcesses[i]
    //            pi[i].Parentid = 0;
            }
            else    {
                fprintf( fPID, TEXT("[%d]ProcessName: %s - Pid: %d - ParentID: %d\n"), i, ProcessName, aProcesses[i], parentInformation.dwProcessId);
    //            pi[i].Processid = aProcesses[i]
    //            pi[i].Parentid = parentInformation.dwProcessId;
            }
        }
    }

//    for(int s = 0; s < cProcesses; s++) {
//            for(int j = 0; j < cProcesses; j++ )    {
//                fprintf( fPID, "%d  |   ", aProcesses[s]);
//                if( pi[j].Parentid == aProcesses[s] )   {
//                    printf( fPID, "...%s\n", pi[j].ProcessName);
//                }
//            }
//    }

  fclose(lProcessModule);
  fclose( fPID );
}

/*List all thread then pass to ListProcessThreads() to return*/
printThreadModules()   {
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    FILE *lThreads;

    lThreads = fopen(processDIR"/threads.txt", "a+");

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
        writeLog( "Can not EnumProcesses to list Thread\n" );
    }
    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    fprintf( lThreads, ("Total: %d\n\n"), cProcesses);

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
            fprintf( lThreads, ("\n\n[+] PROCESS ID: %d\n"), aProcesses[i] );
            ListProcessThreads( lThreads, aProcesses[i], cProcesses );
        }
    }

    fclose(lThreads);
    writeLog( "List Threads" );
    return 1;
}


/*Show Machine information*/
showMachineInfo()   {
    system( "systeminfo > .//Data//sysInfo.txt" );
}


void QueryKey(char *rootkey, HKEY hKey) {
	TCHAR    achKey[255];   // buffer for subkey name
	DWORD    cbName;                   // size of name string
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
	DWORD    cchClassName = MAX_PATH;  // size of class string
	DWORD    cSubKeys=0;               // number of subkeys
	DWORD    cbMaxSubKey;              // longest subkey size
	DWORD    cchMaxClass;              // longest class string
	DWORD    cValues;              // number of values for key
	DWORD    cchMaxValue;          // longest value name
	DWORD    cbMaxValueData;       // longest value data
	DWORD    cbSecurityDescriptor; // size of security descriptor
	FILETIME ftLastWriteTime;      // last write time
    FILE *fRegData, *fRegErr;


    fRegData = fopen( ".\\Data\\Registry\\sumary.txt", "a+" );
    fRegErr = fopen( ".\\Data\\Registry\\error.txt", "a+" );

	DWORD i, retCode;
	char des[2058], path[2048], name[2048], *temp, pading[1024];

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count.
	retCode = RegQueryInfoKey(
		hKey,                    // key handle
		achClass,                // buffer for class name
		&cchClassName,           // size of class string
		NULL,                    // reserved
		&cSubKeys,               // number of subkeys
		&cbMaxSubKey,            // longest subkey size
		&cchMaxClass,            // longest class string
		&cValues,                // number of values for this key
		&cchMaxValue,            // longest value name
		&cbMaxValueData,         // longest value data
		&cbSecurityDescriptor,   // security descriptor
		&ftLastWriteTime);       // last write time

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)   {
		fprintf( fRegData, "\nNumber of subkeys: %d - %s\n", cSubKeys, rootkey);

		for (i=0; i<cSubKeys; i++)	{
			cbName = 255;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)	{
				fprintf( fRegErr, TEXT("(%d) %s\n"), i+1, achKey);
			}
        }
    }

	// Enumerate the key values.
	//LPBYTE buffer[cbMaxValueData];
	//buffer = (LPBYTE*)malloc( cbMaxValueData );

	char* buffer = new char[cbMaxValueData];
	ZeroMemory(buffer, cbMaxValueData);

	if (cValues)    {
		fprintf( fRegData, "\nNumber of values: %d\n", cValues);

		for (i=0, retCode = ERROR_SUCCESS; i<cValues; i++)  {
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';
			retCode = RegEnumValue(hKey, i,
				achValue,
				&cchValue,
				NULL,
				NULL,
				NULL,
				NULL);

			if (retCode == ERROR_SUCCESS )  {

				DWORD lpData = cbMaxValueData;
				buffer[0] = '\0';
				LONG dwRes = RegQueryValueEx(hKey, achValue, 0, NULL, (LPBYTE)buffer, &lpData);
				fprintf( fRegData, TEXT("(%d) %s : %s\n"), i+1, achValue, buffer);
            }
            /*Handle*/
                 char *ppch, ext[1024];
                /*This snipet try to convert path*/

                strcpy(name, buffer);
                ppch = strtok ( buffer, ".");
                if(ppch != NULL)    {
                    strcpy( path, ppch );
                }
                else {
                    fprintf(fRegErr, "Error: Strtok(.)\nCurent Path: %s\n", name);
                }

                strcat( path, "." );
                ppch = strtok( NULL, " ");
                if( ppch != NULL )  {
                    strcat( path, ppch );
                }
                else {
                    fprintf(fRegErr, "Error: strtok([space])\nCurrent Path: %s\n", name);
                }

                ppch = strtok( path, "\"");
                if(ppch != NULL )   {
                    strcpy( ext, ppch );
                }
                else {
                    fprintf(fRegErr, "Error: strtok(\")\nCurrent Path: %s\n", name);
                }

                while (ppch != NULL)  {
                   strcpy(ext, ppch);
                   ppch = strtok (NULL, "\\");
                }

                strcpy( path, ext );
                /*get file name only*/
                strcpy( name, path ); //backup string
                ppch = strtok( name, "\\");
                while (ppch != NULL)  {
                   strcpy(ext, ppch);
                   ppch = strtok (NULL, "\\");
                }

//                strcpy(ppch, "C:");
//                ppch = strstr(path, "\\");
//                temp = strtok(ppch, "\"");
//                strncpy( path, temp, strlen(temp) );

                strcpy( des, ".\\Data\\Registry\\binary\\" );
                strcat( des, ext);
                    /*---*/
                fprintf( fRegData, " Path: %s\nDestination: %s\n", path, des);

                //Dump Binary
                dumpBinary( path, des, "Registry" );

        }
    }

    fclose( fRegData );
    fclose( fRegErr );
	delete [] buffer;
}


void getRegValue()  {
//    HKEY hKey = HKEY_LOCAL_MACHINE;
//    LPCTSTR lpSubKey = TEXT("\\Software\\Microsoft\\Windows\\CurrentVersion\\Winlogon");
//    DWORD ulOptions = 0;
//    REGSAM samDesired = KEY_READ | KEY_WRITE;
//    HKEY phkResult;
//    DWORD dwIndex = 0;
//    TCHAR lpValueName[16383];
//    DWORD lpcchValueName = 16383;
//    LPTSTR lpData= malloc(2048);
//    FILE *fRegData, *fRegErr;
//
//    fRegData = fopen( ".\\Data\\Registry\\sumary.txt", "a+" );
//    fRegErr = fopen( ".\\Data\\Registry\\error.txt", "a+" );
//
//    long RegHandle = RegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, &phkResult);
//    fprintf( fRegData, "phkResult: %s", phkResult );
//    if (RegHandle == ERROR_SUCCESS)    {
//        long sRegHandle = RegEnumValue(phkResult, dwIndex, lpValueName, &lpcchValueName, NULL, NULL,(LPBYTE)lpData, NULL);
//
//
//        if (sRegHandle == ERROR_SUCCESS)   {
//            fprintf( fRegData, "%s: \t%s", lpValueName, lpData );
//            //printf(TEXT("(%d) %s\n"), lpValueName);
//        }
//        else
//            fprintf( fRegErr, "Error: %s", sRegHandle );
//
//    }
//    else if (RegHandle == ERROR_FILE_NOT_FOUND)    {
//        fprintf( fRegErr, "Registry not found!\n" );
//    }
//    else if (RegHandle == ERROR_ACCESS_DENIED)    {
//        fprintf( fRegErr, "Access denied!\n" );
//    }
//    else    {
//        fprintf( fRegErr, "Can not create handle: %s\n", RegHandle );
//
//    }
//
//    free( lpData );
//    fclose( fRegData );
//    fclose( fRegErr );

    HKEY hKey;
    FILE *fRegErr;


    LONG dwRegOPenKey = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"), 0, KEY_READ, &hKey);

    if(dwRegOPenKey == ERROR_SUCCESS){
        //fprintf( fRegErr, "RegOpenKeyEx succeeded, error code %d\n", GetLastError());
        QueryKey( "HKEY_LOCAL_MACHINE", hKey);
    }
    else {
        fRegErr = fopen( ".//Data//Registry//error.txt", "a+" );
        fprintf( fRegErr, "RegOpenKeyEx HKLM failed, error code %d\n", dwRegOPenKey);
        fclose( fRegErr );
    }

        LONG dwRegOPenKey1 = RegOpenKeyEx(HKEY_CURRENT_USER, ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"), 0, KEY_READ, &hKey);

    if(dwRegOPenKey1 == ERROR_SUCCESS){
        //fprintf( fRegErr, "RegOpenKeyEx succeeded, error code %d\n", GetLastError());
        QueryKey( "HKEY_CURRENT_USER", hKey);
    }
    else {
        fRegErr = fopen( ".//Data//Registry//error.txt", "a+" );
        fprintf( fRegErr, "RegOpenKeyEx HKCU failed, error code %d\n", dwRegOPenKey1);
        fclose( fRegErr );
    }


    LONG dwRegOPenKey2 = RegOpenKeyEx(HKEY_CURRENT_USER, ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\"), 0, KEY_READ, &hKey);

    if(dwRegOPenKey2 == ERROR_SUCCESS){
        //fprintf( fRegErr, "RegOpenKeyEx succeeded, error code %d\n", GetLastError());
        QueryKey( "HKEY_CURRENT_USER", hKey);
    }
    else {
        fRegErr = fopen( ".//Data//Registry//error.txt", "a+" );
        fprintf( fRegErr, "RegOpenKeyEx HKCU failed, error code %d\n", dwRegOPenKey2);
        fclose( fRegErr );
    }

    RegCloseKey(hKey);
}

/*Configuaration File handle*/

using namespace tinyxml2;

/*Checking error*/
#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

void CreateConfig() {

    tinyxml2::XMLDocument xmlDoc;
    XMLNode *pConfiguaration = xmlDoc.NewElement("Configuration");
    xmlDoc.InsertFirstChild(pConfiguaration);


    XMLElement *pRegistry = xmlDoc.NewElement("Registry");
    pRegistry->SetText(1);
    pConfiguaration->InsertEndChild(pRegistry);

    XMLElement *pProcess = xmlDoc.NewElement("Process");
    pProcess->SetText(1);
    pConfiguaration->InsertEndChild(pProcess);

    XMLElement *pThread = xmlDoc.NewElement("Thread");
    pThread->SetText(1);
    pConfiguaration->InsertEndChild(pThread);

    XMLElement *pConnection = xmlDoc.NewElement("Connection");
    pConnection->SetText(1);
    pConfiguaration->InsertEndChild(pConnection);

    XMLElement *pConnectionEx = xmlDoc.NewElement("ConnectionEx");
    pConnectionEx->SetText(0);
    pConfiguaration->InsertEndChild(pConnectionEx);

    XMLElement *pService = xmlDoc.NewElement("Service");
    pService->SetText(1);
    pConfiguaration->InsertEndChild(pService);

    XMLElement *pDriver = xmlDoc.NewElement("Driver");
    pDriver->SetText(1);
    pConfiguaration->InsertEndChild(pDriver);

    XMLElement *pSchuduleTask = xmlDoc.NewElement("ScheduleTask");
    pSchuduleTask->SetText(1);
    pConfiguaration->InsertEndChild(pSchuduleTask);

    XMLElement *pStartup = xmlDoc.NewElement("Startup");
    pStartup->SetText(1);
    pConfiguaration->InsertEndChild(pStartup);

    XMLElement *pCustom = xmlDoc.NewElement("CustomPath");
    pCustom->SetText("this is custom path location");
    pConfiguaration->InsertEndChild(pCustom);


    XMLError eResult = xmlDoc.SaveFile("config.conf");
}

void LoadConfig(int *reg, int *pro, int *thre, int *con, int *conex, int *ser, int *dri, int *sche, int *sta) {
        /*Loading file*/

    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile("config.conf");
//    XMLCheckResult(eResult);
    XMLNode * pLoadConfig = xmlDoc.FirstChild();

    XMLElement * pElement = pLoadConfig->FirstChildElement("Registry");
    pElement->QueryIntText(reg);

    pElement = pLoadConfig->FirstChildElement("Process");
    pElement->QueryIntText(pro);

    pElement = pLoadConfig->FirstChildElement("Thread");
    pElement->QueryIntText(thre);

    pElement = pLoadConfig->FirstChildElement("Connection");
    pElement->QueryIntText(con);

    pElement = pLoadConfig->FirstChildElement("ConnectionEx");
    pElement->QueryIntText(conex);

    pElement = pLoadConfig->FirstChildElement("Service");
    pElement->QueryIntText(ser);

    pElement = pLoadConfig->FirstChildElement("Driver");
    pElement->QueryIntText(dri);

    pElement = pLoadConfig->FirstChildElement("ScheduleTask");
    pElement->QueryIntText(sche);

    pElement = pLoadConfig->FirstChildElement("Startup");
    pElement->QueryIntText(sta);
}

void readCustomPath(char *path)   {
    using namespace rapidxml;
    file<> xmlFile(".\\config.conf");

    char *p;
    xml_document<> Doc;
    Doc.parse<0>(xmlFile.data());

    xml_node<> *Node = Doc.first_node();
    xml_node<> *Sub = Node->first_node("CustomPath");

    p = Sub->value();

    if(p != NULL)   {
        strcpy(path, p);
    }
    else {
        strcpy(path, "There is no path");
    }

    std::cout<< "Cus Path: " << path << "\n";
}

void ReadTask() {
    using namespace rapidxml;
    const int parse_full = parse_declaration_node | parse_comment_nodes | parse_doctype_node | parse_pi_nodes | parse_validate_closing_tags;
    /*Read file*/
    file<> xmlFile(".//Data//Processes//raw");
    FILE *fTask;
    fTask = fopen(".//Data//Processes//TaskSchedules.txt", "a+");
    int c = 0;
    char path[2048], des[2048], name[2048];

    xml_document<> Doc;
    Doc.parse<parse_full>(xmlFile.data());
//    fprintf(fTask, "Done ReadXML File\n");
    /*Read Node*/
//    std::cout << "Name of first Node: " << Doc.first_node()->name() << std::endl;

    xml_node<> *Node = Doc.first_node();
    xml_node<> *Sub = Node->first_node("Task");
    xml_node<> *Next;
//    fprintf(fTask, "Done Read Node\n");
//    std::cout << Sub->name() << std::endl;
    while ( Sub != NULL ) {
//            std::cout << "Next Task Element" << "\n";
            char *context = {0};
            if (Sub->first_node("Actions")->first_attribute("Context") != NULL )    {
//                std::cout << "Check Context" << "\n";
                context = Sub->first_node("Actions")->first_attribute("Context")->value();
            }

            if( strncmp("Author", context, 6) == 0 )   {
//                std::cout << "Comparing Author" << "\n";
                if( 1 )     {
                    char p[2048];
//                    std::cout << "IF" << "\n";
                    if (Sub->first_node("Actions") != NULL ) {
                            if( Sub->first_node("Actions")->first_node("Exec") != NULL )    {
                                if(Sub->first_node("Actions")->first_node("Exec")->first_node("Command") != NULL )  {
//                                    std::cout << "Strcpy -> Command" << "\n";
                                    strcpy(p, Sub->first_node("Actions")->first_node("Exec")->first_node()->value() );
                                }
                            }
                    }
//                    std::cout << "Command " << p << std::endl;

                    char *buf, temp[2048];
                    memset(path, 0, strlen(path));
                    if( p != NULL ) strcpy(path, p);

                    buf = strtok(p, "\"");
                    while( buf != NULL )    {
                        strcpy(temp, buf);
                        buf = strtok(NULL, "\"");
                    }
//                    std::cout << temp << "\n";
                    if( temp != NULL )  {
                        strcpy(path, temp);
                    }
                    if( path != NULL ) strcpy(p, path);

                    buf = strtok(p, "\\");
                    while(buf != NULL)  {
                        strcpy(name, buf);
//                        std::cout << "Inside while " << name << "\n";
                        buf = strtok(NULL, "\\");
                    }


//                    strcpy(des, ".\\Data\\Processes\\task binary\\");
//                    if (name != NULL) {
//                            strcat(des, name);
//                    }
//                    else {
//                        strcat(des, "Unknown");
//                    }

                    fprintf(fTask, "Done HandleException\n");
//                    std::cout << "Outside of while " << name << std::endl;

                    if ( strncmp("ForHelper4.exe", name, 7) ) {
                        fprintf(fTask, "[+]Task name: %s\n", name);

                        fprintf(fTask, "Done Taskname\n");

                        if( Sub->first_node("RegistrationInfo")->first_node() != NULL )   {

                            Next = Sub->first_node("RegistrationInfo")->first_node();

                            while ( Next != NULL ) {
                                if( Next->value() != NULL )    {
                                    fprintf(fTask, "\t%s: %s\n", Next->name(), Next->value() );
//                                    std::cout << "Get value inside RegirationInfo: " << Next->value() << "\n";
                                }
                                Next = Next->next_sibling();
//                                std::cout << "Next value inside RegistrationInfo"<< "\n";
                            }

                        }
                        fprintf(fTask, "Done Registration\n");
//                        std::cout << "Done Regis..." << "\n";
                        if( Sub->first_node("Triggers") != NULL )   {
                                if( Sub->first_node("Triggers")->first_node() != NULL ) {
                                    if ( Sub->first_node("Triggers")->first_node()->first_node() != NULL)   {
//                                        std::cout << "Next to Triggers" << "\n";
                                        Next = Sub->first_node("Triggers")->first_node()->first_node();

                                        while( Next != NULL )    {
                                            if( Next->value() != NULL ) {
                                                fprintf(fTask, "\t%s: %s\n", Next->name(), Next->value() );
//                                                std::cout << "Get value inside Triggers: " << Next->value() << "\n";
                                            }
                                            Next = Next->next_sibling();
//                                            std::cout << "Next value inside Triggers" << "\n";
                                        }
                                    }
                                }
                        }

//                        std::cout << "Done Trigger" << "\n";
                        if( Sub->first_node("Actions") != NULL )   {
                                if( Sub->first_node("Actions")->first_node("Exec") != NULL )    {
                                    if( Sub->first_node("Actions")->first_node("Exec")->first_node("Command") != NULL)  {
                                        fprintf(fTask, "\tCommand: %s\n", Sub->first_node("Actions")->first_node("Exec")->first_node("Command")->value());

                                        //DumpFile
                                        memset(des, 0, strlen(des));
                                        strcpy(des, ".\\Data\\Processes\\task binary\\");
//                                        std::cout << "strpy path done!" << "\n";
                                        if(name == NULL) strcpy(name, "Unknown");
                                        strcat(des, name);
                                        fprintf(fTask, "Path: %s\nDes: %s\n", path, des);
//                                        std::cout << "Print path done" << "\n";
                                        dumpBinary(path, des, "TaskSchedule");
//                                        std::cout << "Dump done!" << "\n";
                                    }
                                    if( Sub->first_node("Actions")->first_node("Exec")->first_node("Arguments") != NULL )   {
                                        fprintf(fTask, "\t\tArgument: %s\n", Sub->first_node("Actions")->first_node()->first_node("Arguments")->value() );
                                    }
//                                    std::cout << "Arg.." << "\n";
                                }
                        }
                        fprintf(fTask, "=============================\n\n");
                        c++;
                    }
                }
            }
//            std::cout << "Next Task" << "\n";
            Sub = Sub->next_sibling("Task");
    }
    fprintf(fTask, "Total task: %d\n\n\n", c);
    fclose(fTask);
}

/*GetUserFolder*/
void GetUserFolder(char *path)    {
    HANDLE pHandle;
    WIN32_FIND_DATA FileDetails;
    char name[2048], xpath[2048], aUserFolder[2048], fUsername[2048], DirName[2048], FolderPath[2048];

    strcpy(xpath, path);
    strcat(xpath, "*");

    pHandle = FindFirstFileA(xpath, &FileDetails);
    while( FileDetails.cFileName && pHandle )   {
        FindNextFileA( pHandle, &FileDetails );
        if(strcmp(FileDetails.cFileName, aUserFolder) == 0 )    {
            break;
        }
        if( strcmp( FileDetails.cFileName, ".") != 0 &&  strcmp( FileDetails.cFileName, "..") != 0 && FileDetails.cFileName != NULL && strcmp( fUsername, FileDetails.cFileName) != 0 ) {
            strcpy(aUserFolder, FileDetails.cFileName );
            if( aUserFolder != 0 & strcmp( aUserFolder, "desktop.ini") != 0)   {
                strcpy( DirName, ".\\Data\\Startup\\" );
                strcat( DirName, aUserFolder );

                CreateDirectory(DirName, NULL);
//                std::cout << "Created Directory named: " << DirName << "\n";
                strcat( DirName, "\\");
                if( strcmp(aUserFolder, "All Users") == 0 ) {
                    strcpy( FolderPath, " C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\");
//                    std::cout << "Path: " << FolderPath << "\n";
                    CopyFolder( FolderPath, DirName );
                }
                else if ( strcmp(aUserFolder, "Public") != 0 && strncmp(aUserFolder, "Defa", 4) != 0) {
                    strcpy( FolderPath, "C:\\Users\\");
                    strcat( FolderPath, aUserFolder);
                    strcat( FolderPath, "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\" );
//                    std::cout << "Path: " << FolderPath << "\n";

                    CopyFolder( FolderPath, DirName );
                }
            }
        }
    }
}


void DumpStartup(char *custom)  {
     GetUserFolder("C:\\Users\\");

    if(strcmp(custom, "C:\\dir1\\dir2\\DestinationDir\\") != 0 )  {
        CreateDirectory(".\\Data\\Startup\\CustomFolder\\", NULL);
        CopyFolder(custom, ".\\Data\\Startup\\CustomFolder\\");
    }
}
/*==================== END CORE FUNCTIONS ======================*/
#endif // BUILTINLIB_H_INCLUDED


