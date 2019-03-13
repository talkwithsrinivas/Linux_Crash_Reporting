Sample poc showing integration with breakpad on linux.

Directory structure:
===================
test.cpp is the driver program. It crashes every few minutes and randomly generates either a sigabort or a floating point exception.
monitor.sh is the script to monitor test and restart test if it is not running.
breakpad is the google breakpad source directory. This has been setup using https://chromium.googlesource.com/breakpad/breakpad


Compilation steps for sample program:
====================================

~/Programs/breakpad_example$ g++ -g -I breakpad/src/src  -o test test.cpp breakpad/src/src/client/linux/libbreakpad_client.a -std=c++11 -lpthread breakpad/src/src/common/linux/http_upload.cc  -ldl -lcurl


Symbol Generation:
==================

./breakpad/src/src/tools/linux/dump_syms/dump_syms ./test > test.sym

head -n1 test.sym
MODULE Linux x86_64 8A3387456B385ED4F14C07E9A7A1259F0 test
mkdir -p symbols/test/8A3387456B385ED4F14C07E9A7A1259F0
mv test.sym symbols/test/8A3387456B385ED4F14C07E9A7A1259F0/
google-breakpad/src/processor/minidump_stackwalk minidump.dmp ./symbols

or we can generate full core 
./breakpad/src/src/tools/linux/md2core/minidump-2-core -o fullcore d34a335e-dbba-4623-ee9116a3-07e6c846.dmp
    $ gdb ./test fullcore
(gdb) bt

Sentry Related
=============
create the org and project in sentry and login. This will give you the url to upload the minidump file.
Download the sentry cli
curl -sL https://sentry.io/get-cli/ | bash
create a auth token in sentry and provide the
 the --auth-token command line parameter whenever you invoke sentry-cli or add it to your .sentryclirc config file.

sentry-cli upload-dif -t breakpad -o srinivas-7r -p linux-crash symbols/ (Org is srinivas-7r and project is linux-crash. symbopls is the directory of extracted breakpad symbols above.)
 Found 1 debug information file
> Prepared debug information file for upload
> Uploaded 1 missing debug information file
> File processing complete:




