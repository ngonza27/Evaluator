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

string nombreMemoriaCompartida = "evaluator"; //-n
string memComp = "/"+nombreMemoriaCompartida;

int
initMemoriaCompartidaEntrada(int i, int ie, int oe) {
  sem_t *vacios = sem_open("vacios", O_CREAT | O_EXCL, 0660, tamanoBufferEntrada);
  sem_t *llenos = sem_open("llenos", O_CREAT | O_EXCL, 0660, 0);
  sem_t *mutex  = sem_open("mutex", O_CREAT | O_EXCL, 0660, 1);

  int fd = shm_open(memComp.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  int tamanoTotal = (sizeof(struct BandejasEntrada) + 
                     sizeof(struct Header) + 
                     sizeof(struct BandejaSalida)
                    );

  if (ftruncate(fd, tamanoTotal) != 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  //

  void *dir;

  if ((dir = mmap(NULL, sizeof(struct elemento), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  //aca se hace el mapeo de las bandejas
  //Mapeo COLAS ENTRADA
  if ((dir = mmap(NULL, (sizeof(struct BandejasEntrada)*Header.entradasEntra*Header.tamanoColasExternas), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida ENTRADA: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  //Mapeo HEADER
  //if ((dir = mmap(NULL, (sizeof(struct Header)), PROT_READ | PROT_WRITE, MAP_SHARED,
  //		  fd, 0)) == MAP_FAILED) {
  //  cerr << "Error mapeando la memoria compartida HEADER: "
	// << errno << strerror(errno) << endl;
  //  exit(1);
  //}

  //Mapeo COLAS SALIDA
  //if ((dir = mmap(NULL, (sizeof(struct BandejasEntrada)*Header.entradasSalida), PROT_READ | PROT_WRITE, MAP_SHARED,
	//	  fd, 0)) == MAP_FAILED) {
  //  cerr << "Error mapeando la memoria compartida SALIDA: "
	// << errno << strerror(errno) << endl;
  //  exit(1);
  //}

  struct Header *pHeader = (struct Header *)dir;
  pHeader->entradasEntra = 5; //-i
  pHeader->tamanoColasExternas = 6; //-ie 
  pHeader->entradasSalida = 10; //-oe
  pHeader->tamanoColasInternas = 6; //-q

  struct BandejasEntrada *pEntra = (struct BandejasEntrada *)dir;
  pEntra->entra = 0;
  pEntra->sale = 0;
  pEntra->cantidad = 0;
  pEntra->tamano = tamanoBufferEntrada;

  struct BandejaSalida   *pSale  = (struct BandejaSalida * )dir;
  pSale->entra = 0;
  pSale->sale = 0;
  pSale->cantidad = 0;
  pSale->tamano = tamanoBufferEntrada;

  //Header *h = mmap()..;
  //Particionando bandeja entrada
  //int i;
  //BandejaEntrada **ban = new BandejaEntrada[];
  //ban[0] = (BandejaEntrada)(( (char *) n)+ sizeof(header));
  //for(int nb=1; nb<i; ++i){
  //  ban[nb]=(ban[nb-1])+sizeof(BandejaEntrada)* header ->ie;
  //}


  //Crear multiples semaforos:
  //cantidad = numero bandejas de entrada: entradasEntra
  
  //int cantidad = 0;
  //std::string semname = "sem";
  //sem_t **arraySem = new sem_t *[cantidad];

  //for(int j=0; j<cantidad; j++){
  //  std::ostringstream name(semname);
  //  name  << semname << j;
  //  std::string realName(name.str());

  //  arraySem[j]= sem_open(realName.c_str(),
  //                        O_CREAT | O_EXCL, 0660, 1);

  //  if(arraySem[j] == SEM_FAILED){
  //    cerr << "error openening semaphore" << endl;
  //  }
  //}

  close(fd);

  return EXIT_SUCCESS;
}

void *rutinaAejecutar2(void * hola){
  printf("hilo creado");
}

void *rutinaAejecutar(void * hola){
  printf("hilo creado");
}

void hilosMultiples(int n){
 pthread_t threads[n];
   int rc;
   int i;
   
   for( i = 0; i < n; i++ ) {
      cout << "main() : creating thread, " << i << endl;
      rc = pthread_create(&threads[i], NULL, *rutinaAejecutar, NULL);
      pthread_join(threads[i], NULL);//Espera a que el hilo creado termine de ejecutar.
      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}

void hilosInternos (){
  pthread_t interno1, interno2, interno3;
  pthread_create(&interno1, NULL, *rutinaAejecutar2,NULL);
  pthread_create(&interno2, NULL, *rutinaAejecutar2,NULL);
  pthread_create(&interno3, NULL, *rutinaAejecutar2,NULL);
  
  pthread_join(interno1, NULL);
  pthread_join(interno2, NULL);
  pthread_join(interno3, NULL);
}

void evaluar(int tamanoColasInternas){ 
  //struct muestra B[tamanoColasInternas];
  //struct muestra D[tamanoColasInternas];
  //struct muestra S[tamanoColasInternas];
}

//reg
int registrarExamenes(){}

//ctrl
int revisarSistema(){
  //string listMod
  //modo interactivo:
  // while (el usuario ingrese list o update, espere)
  // >list [ processing | waiting | reported | reactive | all]
  // >update { B|D|S } <integer>
  //cin >> listMode; 
}

//rep
int reportarResultados(){
  //liberar el reporte
  //shm ó sem unlink?
}

int
deleteMemoriaCOmpartida(void) {
  sem_unlink("vacios");
  sem_unlink("llenos");
  sem_unlink("mutex");
  shm_unlink(memComp.c_str());
  return EXIT_SUCCESS;
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

int
main(int argc , char* argv[]){
    if(argc < 2){
      cout << "Please use one of the following commands:"<< endl;
      usage(argv[0]);
    }

    std::string arg1(argv[1]);
    if (arg1 == "init"){
      int i = 0;
      int ie = 0;
      int oe = 0;
      int b = 0;
      int d = 0;
      int s = 0;
      int q = 0;
        for(int i=1; i < argc; ++i){
          if(std::string (argv[i]) == "-n"){ 
            nombreMemoriaCompartida = argv[i+1];
            memComp = "/"+nombreMemoriaCompartida;
          }
          //TODO: poner bien los valores de cada variable
          if(std::string (argv[i]) == "-i"){ int i = 0; }
          if(std::string (argv[i]) == "-ie"){ int ie = 0;  }
          if(std::string (argv[i]) == "-oe"){ int oe = 0;  }
          if(std::string (argv[i]) == "-b"){ int b = 0;  }
          if(std::string (argv[i]) == "-d"){ int d = 0;  }
          if(std::string (argv[i]) == "-s"){ int s = 0;  }
          if(std::string (argv[i]) == "-q"){ int q = 0;  }
        }
        //cout << nombreMemoriaCompartida << endl;
        //cout <<"memcomop NUEVO: " <<memComp << endl;
        initMemoriaCompartidaEntrada(i,ie,oe);
    }
    if(arg1 == "reg"){ //registrar examenes
        int idFicheros;
        string contenido[100];
        if (argv[2] == NULL)
            cout << "ERRRRRRRROR PS" << endl;
        string filename = argv[2];
        //cout << filename << endl;
        //MODO INTERACTIVO:
        if(std::string (argv[4]) == "-"){
          int id=0;
          string linea;
          string arr[3];
          string cola,tipo,cantidad;
          int i = 0; //CON ESTE i PUEDO GENERAR EL IDENTIFICADOR
          while(getline(cin,linea)){
            stringstream ssin(linea);
            while (ssin.good() && i < 3){
                ssin >> arr[i];
                ++i;
            }
            cola = arr[0];
            tipo = arr[1];
            if(std::atoi(arr[2].c_str()) > 5){
              cerr << "Cantidad no valida" << endl;
            }
            cantidad = arr[2];
            ++id;
            cout << "id:" << id << endl;
            i = 0;
          }
        } else {
          ifstream infile(filename.c_str());
          ofstream outfile;
          int j=0;

          //TODO: Falta obtener varios ficheros
          outfile.open("out.spl"); //falta obtener el nombre de cada fichero
          while (!infile.eof()){
              infile >> contenido[j];
              ++j;
              if (j%3 == 0){
                idFicheros = j;
                outfile << "id:" << idFicheros/3 << '\n';
                ++idFicheros;
              }
          }
                            outfile.close();

        }
    }
    
    if (arg1 == "ctrl"){ //mostrar estado del sistema, agregar reactivos
        //-n nombre memoria compartida
        //revisarSistema();
        string command[2];
        string linea;
        int i=0;
        cout << "> " << endl;
          while(getline(cin,linea)){
            stringstream ssin(linea);
            while (ssin.good()){
                ssin >> command[i];
                ++i;
                cout << i << endl;
            }
            //TODO: quitar los mostrar y hacer los cout de lo que se necesita
            if(command[0] == "list"){
              if(command[1] == "all"){
                cout << "mostrando all" << endl;
                i=0;
              }
              if(command[1] == "reactive"){
                cout << "mostrando reactive" << endl;
                i=0;
              }
              if(command[1] == "reported"){
                cout << "mostrando reported" << endl;
                i=0;
              }
              if(command[1] == "waiting"){
                cout << "mostrando waiting" << endl;
                i=0;
              }
              if(command[1] == "processing"){
                cout << "mostrando processing" << endl;
                i=0;
              }
            } else {
              //TODO:hay que agregar cosas a esto
              cout << "update mode " << command[0] << endl;  
            }
          }
    }
    if (arg1 == "rep"){
      if(argc != 6){ 
          cout << "rep - Number of arguments not met" << endl;
          usage(argv[0]);
      }
      //reportarResultados();
    }
    if (arg1 == "stop"){

      //terminate called after throwing an instance of 'std::logic_error'
      //what():  basic_string::_M_construct null not valid
      //Aborted (core dumped)

      
       //if(std::string (argv[2]) == "-n"){
       //    nombreMemoriaCompartida = argv[3];
       //    memComp = "/"+nombreMemoriaCompartida;
       //}
      //deleteMemoriaCOmpartida();
    }
    return EXIT_SUCCESS;
}