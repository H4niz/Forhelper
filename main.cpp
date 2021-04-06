#include "builtinLibrary.h"

#define MAX_THREADS 3
#define BUF_SIZE 255

int genXXX(char *xxx)   {
    char *s1 = "falkjJLlkjalkfjJHWI304pjljjfjLJ==JLADSJljldjfaljajfljas-ldfjaljfoisafjlasjfjzcxvzxclkejjldsjcJKD-JJLIEf";
    char *s2 = "-IUUJOJjflasdljLKJLJLJlfaLJLJLFDJFLKJLJLKJlafjdfjalfjajflajdLJLKJFDJ-jalfjalfj00-50234325JLJLJLjfldaflajlf";
    char *s3 = "-DFJDLJLNMLJlfjallj35u23045u0234u5u0asjflkajdJLJDFLJDLFJDLXXXX";
    char *s4 = "HPT_";
    char x[100000];

    strcpy(x, s4);
    strcat(x, s1 );
    strcat(x, s3 );
    strcat(x, s2 );

    strncat(x, "_END_", 5);

    strcpy(xxx, x);
    return 0;
}

int check(char *x1, char *x2, int *res)   {
    if( !strncmp( x1, x2, 200) )  {
        *res = 1;
    }
    else {
        *res = 0;
    }
    return 0;
}
int main()  {
    CreateDirectory("Data", NULL);

    FILE *f, *fo;
    LPWORD lpThreadID;
    char buf, pass[100000], license[100000];
//    config = fopen("config.cf", "a+");
    int all = 0, reg = 0, pro = 0, thre = 0, con = 0, conex = 0, ser = 0, dri = 0, sche = 0, sta = 0;
    int result = 0;
    char custom[2048] = {'0'};

    /*CheckLicense*/
    fo = fopen(".//HPT.lic", "r");
    genXXX(pass);
    //fprintf( fo, "%s", pass); //Create HPT LIC
    if( fo == 0 ) {
        fclose(fo);
        exit(0);
    }
    if( fgets(license, 210, fo ) )   {
        check(pass, license, &result);
        if( result == 0 ) {
                fclose(fo);
                exit(0);
        }
    }
    fclose(fo);
    /*-------*/


    f = fopen(".//config.conf", "r");

    if (f == 0) {
        CreateConfig();
    }
    else    {
        LoadConfig(&reg, &pro, &thre, &con, &conex, &ser, &dri, &sche, &sta);
    }

    fclose( f );

    printf("\t\t\t=== OPTION ===\n\t\t\tResitry: %d\n\t\t\tProcess: %d\n\t\t\tThread: %d\n\t\t\tConnection: %d\n\t\t\t  ConnectionEx: %d\n\t\t\tService: %d\n\t\t\tDriver: %d\n\t\t\tScheduleTask: %d\n\t\t\tStartup: %d\n", reg, pro, thre, con, conex, ser, dri, sche, sta );
//
//    fclose(config);

    DWORD   dwThreadIdArray[MAX_THREADS];
    HANDLE  hThreadArray[MAX_THREADS];

    CreateDirectory("Data\\Processes", NULL);
    //CreateDirectory("Data\\Processes\\xmlTask", NULL);
    CreateDirectory("Data\\Processes\\binary", NULL);


    if(all == 1)    {
            //         Create the thread to begin execution on its own.
//       hThreadArray[0] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      enumDriver,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      &dwThreadIdArray[0]);   // returns the thread identifier
//        Sleep(5000);
//        hThreadArray[1] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      printProcessModules,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      &dwThreadIdArray[1]);   // returns the thread identifi
//        Sleep(1000);
//        hThreadArray[2] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      connection,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      &dwThreadIdArray[2]);   // returns the thread identifier
//        Sleep(1000);
//        hThreadArray[3] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      listScheduleTask,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      &dwThreadIdArray[3]);   // returns the thread identifier
//        Sleep(1000);
//        hThreadArray[4] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      printThreadModules,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      &dwThreadIdArray[4]);   // returns the thread identifier
//        Sleep(1000);
//        hThreadArray[5] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      enumAllreg,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      &dwThreadIdArray[5]);   // returns the thread identifier
//        Sleep(1000);
//        hThreadArray[6] = CreateThread(
//                                      NULL,                   // default security attributes
//                                      0,                      // use default stack size
//                                      listService,       // thread function name
//                                      0,          // argument to thread function
//                                      0,                      // use default creation flags
//                                      dwThreadIdArray[6]);   // returns the thread identifier
//
//        WaitForMultipleObjects(6, hThreadArray, TRUE, INFINITE);
//        // Close all thread handles and free memory allocations.
//        for(int i=0; i<7; i++)
//        {
//            CloseHandle(hThreadArray[i]);
//        }
    }
    else {
        if ( ser )  {
            CreateDirectory("Data\\Services", NULL);
            CreateDirectory("Data\\Services\\services", NULL);
            CreateDirectory("Data\\Services\\drivers", NULL);

            listService();
        }
        if ( reg ) {
            CreateDirectory("Data\\Registry", NULL);
            CreateDirectory( "Data\\Registry\\binary", NULL );

            enumAllreg();
            getRegValue();
        }
        if ( thre )    {
            printThreadModules();
        }
        if ( pro ) {
            printProcessModules();      // thread function name
        }
        if ( con ) {
            connection();
        }
        if ( conex ) {
            connectionEx();
        }
        if ( dri )   {
            enumDriver();
        }
        if ( sche ) {
            CreateDirectory("Data\\Processes\\task binary", NULL);
            listScheduleTask();
            //ShellExecute(NULL, "open", ".\\TaskScheduler2.exe", NULL, NULL, 0);
        }
        if (sta)    {
            CreateDirectory("Data\\Startup", NULL);
            readCustomPath(custom);
            DumpStartup(custom);
        }
    }

    Sleep(100);
    showMachineInfo();
    if (sche)   ReadTask();

//    dumpBinary("C:\\Program Files (x86)\\Google\\Update\\GoogleUpdate.exe", ".\\Data\\Processes\\task binary", "CopyTask");
//    EnumTaskSchedule();


    openDir();

    return 0;
}



