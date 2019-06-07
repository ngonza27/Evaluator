#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <semaphore.h>
#include <cerrno>
#include "elementos.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sstream>
#include <fstream>

using namespace std;

string nombreMemoriaCompartida = "evaluator";
string memComp = "/"+nombreMemoriaCompartida;

exam::exam(int id, test_type type, int q, int queueIn){
  this->id = id;
  this->type = type;
  this->quantity = q;
  this->queue = queueIn;
}

static void usage (const char* progname){
  cerr << "Usage: " << progname
       << " init [-i <integer>] [-ie <integer>] [-oe <integer>]\n\t\t\t[-n <string>] [-b <integer>] [-d <integer>]\n\t\t\t[-ee <integer>] [-s <integer>] [-q <integer>]"
       << "\n\n\t\t| reg [-n <string>] {{<filename>} ... | -}"
       << "\n\t\t| ctrl [-s <string>]"
       << "\n\t\t| rep [-s <string>] {-i <integer> | -m <integer>}"
       << endl;
       exit(EXIT_FAILURE);
}

void ArgMap(int argc, char const *argv[]){
	if(argc > 1){
		string cmd = argv[1];
		//keep remaining arguments
		string *args = new string[argc-2];
		for(int i=2; i<argc; i++){
			args[i-2] = argv[i];
		}
		argc -= 2;
		if(cmd == "init"){
			Initialize(argc, args);
			return;
		} else if(cmd == "reg"){
			Register(argc, args);
			return;
		} else if(cmd == "ctrl"){
			Control(argc, args);
			return;
		} else if(cmd == "rep"){
			Report(argc, args);
			return;
		}
	} else{
        cout << "Please use one of the following commands:"<< endl;
        usage(argv[0]);
    }
	return;
}


void Initialize(int argc, string argv[]){
}
void Register(int argc, string argv[]){
}
void Control(int argc, string argv[]){
}
void Report(int argc, string argv[]){
}


int main(int argc , char* argv[]){
    ArgMap(argc, argv);
    return 0;
}