#include <iostream>
#include <string>
#include "client/linux/handler/exception_handler.h"
#include "common/linux/http_upload.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 


#define CORE_GEN_TIME_IN_MIN    1 /* Generate core every X minutes */
#define CORE_PATH               "/home/srinivas/Programs/breakpad_example/"

using namespace google_breakpad;
typedef std::vector<std::string> stringvec;
 

bool  dumpCallback (const google_breakpad::MinidumpDescriptor &desc,
                         void* context,
                         bool succeeded)
{
    std::cerr << "Minidump file created: " << desc.path();
    return succeeded;
}

void genFloatingException()
{
    int a = 2/0;
}

void genAbortException()
{
    abort();
}

bool UploadMinidump(std::string &path)
{
  // Add additional arguments for Sentry
  std::map<string, string> parameters;

  std::map<string, string> files;
  files["upload_file_minidump"] = path;

  return HTTPUpload::SendRequest(
    "https://sentry.io/api/1410953/minidump/?sentry_key=041baeafc0104f3184a52892d62d8d47",
    parameters,
    files,
    /* proxy */ "",
    /* proxy password */ "",
    /* certificate */ "",
    /* response body */ nullptr,
    /* response code */ nullptr,
    /* error */ nullptr
  );
}

 
void read_directory(const std::string& name, stringvec& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
	string file_name =  dp->d_name;
	if(file_name.find(".dmp") != std::string::npos) {
            v.push_back(file_name);
	}
    }
    closedir(dirp);
}

void CheckForMiniDumpFiles(stringvec& v)
{
    read_directory(CORE_PATH, v);
    for(auto i : v) {
    	std::cerr << "filenmae " << i <<   " "; // this will print all the contents of *features*
	UploadMinidump(i);
	remove(i.c_str());
    } 

}

int main(int argc, char* argv[])
{
    google_breakpad::MinidumpDescriptor descriptor(CORE_PATH);
    google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);
    stringvec v;	
    CheckForMiniDumpFiles(v);   

    sleep(CORE_GEN_TIME_IN_MIN * 60);
    srand(time(0));
    if (rand() %2) { 		
        genFloatingException();
    } else {
	genAbortException();
    }


    return 0;
}
