#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <semaphore.h>
#include <cerrno>
#include "elementos.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int
initMemoriaCompartidaEntrada(void) {
  cout << "lol" << endl;
  sem_t *vacios = sem_open("vacios", O_CREAT | O_EXCL, 0660, tamanoBufferEntrada);
  sem_t *llenos = sem_open("llenos", O_CREAT | O_EXCL, 0660, 0);
  sem_t *mutex  = sem_open("mutex", O_CREAT | O_EXCL, 0660, 1);

  int fd = shm_open("buffer", O_RDWR | O_CREAT | O_EXCL, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  if (ftruncate(fd, sizeof(struct Buffer)) != 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  if ((dir = mmap(NULL, sizeof(struct elemento), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  struct Buffer *pBuffer = (struct Buffer *) dir;
  pBuffer->entra = 5;
  pBuffer->sale = 10;
  pBuffer->cantidad = 0;
  pBuffer->tamano = tamanoBufferEntrada;

  close(fd);

  return EXIT_SUCCESS;
}


int
crearSemaforoProductor(void) {

  sem_t *vacios, *llenos, *mutex;

  vacios = sem_open("vacios", 0);
  llenos = sem_open("llenos", 0);
  mutex  = sem_open("mutex", 0);

  int fd = shm_open("/buffer", O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  if ((dir = mmap(NULL, sizeof(struct elemento), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  struct Buffer *pBuffer = (struct Buffer *) dir;
  int item = 0;

  for(;;) {
    item++;

    sem_wait(vacios);
    sem_wait(mutex);

    pBuffer->buffer[pBuffer->entra].elemento = item;
    pBuffer->entra = (pBuffer->entra + 1) % pBuffer->tamano;
    pBuffer->cantidad++;

    sem_post(mutex);
    sem_post(llenos);
    cout << "Item producido y ubicado" << endl;
    sleep(2);
  }

  return EXIT_SUCCESS;
}


int
crearSemaforoConsumidor(void) {

  sem_t *vacios, *llenos, *mutex;

  vacios = sem_open("vacios", 0);
  llenos = sem_open("llenos", 0);
  mutex  = sem_open("mutex", 0);

  int fd = shm_open("/buffer", O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  if ((dir = mmap(NULL, sizeof(struct elemento), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  struct Buffer *pBuffer = (struct Buffer *) dir;
  int item = 0;

  for(;;) {

    sem_wait(llenos);
    sem_wait(mutex);

    item = pBuffer->buffer[pBuffer->sale].elemento;
    pBuffer->sale = (pBuffer->sale + 1) % pBuffer->tamano;
    pBuffer->cantidad--;

    sem_post(mutex);
    sem_post(vacios);
    cout << item << endl;
  }

  return EXIT_SUCCESS;
}

int
deleteMemoriaCOmpartida(void) {

  sem_unlink("vacios");
  sem_unlink("llenos");
  sem_unlink("mutex");
  shm_unlink("/buffer");

  return EXIT_SUCCESS;
}

//init
int iniciarSistema(){
  //-i
  int numeroEntradas = 5;
}

//reg
int registrarExamenes(){}

//ctrl
int revisarSistema(){
  //string listMode;
  //modo interactivo:
  // while (el usuario ingrese list o update, espere)
  // >list [ processing | waiting | reported | reactive | all]
  // >update { B|D|S } <integer>
  //cin >> listMode; 
}

//rep
int reportarResultados(){}

static void usage (const char* progname){
  cerr << "Usage: " << progname
       << " init [-i <integer>] [-ie <integer>] [-oe <integer>]\n\t\t\t[-n <string>] [-b <integer>] [-d <integer>]\n\t\t\t[-ee <integer>] [-s <integer>] [-q <integer>]"
       << "\n\n\t\t| reg [-n <string>] {{<filename>} ... | -}"
       << "\n\t\t| ctrl [-s <string>]"
       << "\n\t\t| rep [-s <string>] {-i <integer> | -m <integer>}"
       << endl;
       exit(EXIT_FAILURE);
}

int
main(int argc , char* argv[]){

    if(argc < 2){
      cout << "Please use one of the following commands:"<< endl;
      usage(argv[0]);
    }

    std::string arg1(argv[1]);
    if (arg1 == "init"){
        //initMemoriaCompartidaSem();
    }
    if (arg1 == "reg"){ 
        //deleteMemoriaCOmpartida();
    }
    if (arg1 == "ctrl"){
        //revisarSistema();
        string command;
          cout << "> ";
          cin >> command;
          if(command[0] == 'l'){
            cout << "list mode " << command[0] << endl;
          } else {
            cout << "update mode " << command[0] << endl;  
          }
    }
    if (arg1 == "rep"){
      if(argc != 6){ 
          cout << "rep - Number of arguments not met" << endl;
          usage(argv[0]);
      }
      //reportarResultados();
    }
    return EXIT_SUCCESS;
}