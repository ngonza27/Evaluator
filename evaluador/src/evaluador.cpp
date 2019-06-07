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


//TODO: PROBAR EN EL PRODUCTOR CONSUMIDOR YA DECLARANDO LOS VALORES DEL BUFFER, INCREMENTAR EL VALOR DESDE OTRA FUNCION A VER SI FUNCIONA
//cout << Header.entradasEntra <<endl;
//cout << Header.entradasEntra + 7 <<endl;


//HACER UN FICHERO DE PRUEBA PARA LA RECEPCION DE VARIOS FICHEROS

using namespace std;

string nombreMemoriaCompartida = "evaluator"; //-n
string memComp = "/"+nombreMemoriaCompartida;

int
initMemoriaCompartidaEntrada(int iInit, int ieInit, int oeInit, int qInit) {
  cout << "i:" << iInit << endl;
  cout << "ie:" << ieInit << endl;
  cout << "oe:" << oeInit << endl;
  //sem_t *vacios = sem_open("vacios", O_CREAT | O_EXCL, 0660, tamanoBufferEntrada);
  //sem_t *llenos = sem_open("llenos", O_CREAT | O_EXCL, 0660, 0);
  //sem_t *mutex  = sem_open("mutex", O_CREAT | O_EXCL, 0660, 1);

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

  //--------------------------------------------------------------------------------------------------------------------//
  //aca se hace el mapeo de las bandejas
  //Mapeo COLAS ENTRADA

  void *dir;

  if ((dir = mmap(NULL, (sizeof(struct BandejasEntrada)*iInit*ieInit), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida ENTRADA: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  //Mapeo HEADER
  if ((dir = mmap(NULL, (sizeof(struct Header)), PROT_READ | PROT_WRITE, MAP_SHARED,
  		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida HEADER: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  //Mapeo COLAS SALIDA
  if ((dir = mmap(NULL, (sizeof(struct BandejasEntrada)*oeInit), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida SALIDA: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }
  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  struct Header *pHeader = (struct Header *)dir;
  pHeader->entradasEntra=8; //-i
  pHeader->tamanoColasExternas; //-ie 
  pHeader->entradasSalida; //-oe
  pHeader->tamanoColasInternas; //-q

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
  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  //Header *h = mmap()..;
  //Particionando bandeja entrada
  //int i;
  //BandejaEntrada **ban = new BandejaEntrada[];
  //ban[0] = (BandejaEntrada)(( (char *) n)+ sizeof(header));
  //for(int nb=1; nb<i; ++i){
  //  ban[nb]=(ban[nb-1])+sizeof(BandejaEntrada)* header ->ie;
  //}
  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  //Crear multiples semaforos:
  //cantidad = numero bandejas de entrada: entradasEntra
  
  //int cantidad = iInit;
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
  //--------------------------------------------------------------------------------------------------------------------//

  //evaluar(qInit);

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

}

//rep
int reportarResultados(){
  //liberar el reporte
  //shm ó sem unlink?
}

int
deleteMemoriaCOmpartida(void) {
  //sem_unlink("vacios");
  //sem_unlink("llenos");
  //sem_unlink("mutex");
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
  //cout << Header.entradasEntra <<endl;
  //cout << Header.entradasEntra + 7 <<endl;
    if(argc < 2){
      cout << "Please use one of the following commands:"<< endl;
      usage(argv[0]);
    }

    std::string arg1(argv[1]);
    if (arg1 == "init"){
      int ii = 5;
      int ie = 6;
      int q = 6;
      int oe = 10;
      int d = 100;
      int s = 100;
      int b = 100;
        for(int i=1; i < argc; ++i){
          if(std::string (argv[i]) == "-n"){ 
            nombreMemoriaCompartida = argv[i+1];
            memComp = "/"+nombreMemoriaCompartida;
          }
          //TODO: poner bien los valores de cada variable
          //int i = (int)argv[i];
          if(std::string (argv[i]) == "-i"){ ii = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-ie"){ ie = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-oe"){ oe = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-b"){ b = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-d"){ d = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-s"){ s = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-q"){ q = atoi(argv[i+1]);  }
        }
        cout << "i:" << ii << endl;
        //initMemoriaCompartidaEntrada(ii,ie,oe,q);
    }

    if(arg1 == "reg"){ //registrar examenes
    //TODO: hay que agregar el condicional para el -n
        if (argv[2] == NULL)
            cout << "ERRRRRRRROR PS" << endl;
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
          int idFicheros;
          string ficheroSalida;
          string contenido[100];
          string files[argc-4];

          //----------------------------------------------------------------------------------//
          //guardo todas las direcciones de los ficheros que me pasan mediante el comando reg
          for(int p=0; p<argc-4; p++){
            files[p]=argv[p+4];
            //cout << files[p] << endl;
          }
          //----------------------------------------------------------------------------------//

          //----------------------------------------------------------------------------------//
          //obtengo nombre para hacer el fichero .spl
          string fileN = files[0];
          const size_t last_slash_idx = fileN.find_last_of("\\/");
          if (std::string::npos != last_slash_idx){
              fileN.erase(0, last_slash_idx + 1);
          }
          const size_t period_idx = fileN.rfind('.');
          if (std::string::npos != period_idx){
              fileN.erase(period_idx);
          }
          //----------------------------------------------------------------------------------//
         
          //----------------------------------------------------------------------------------//
          //leo el contenido de los ficheros y genero el spl con los identificadores unicos 
          ficheroSalida = fileN+".spl";
          ofstream outfile;
          outfile.open(ficheroSalida); 
          int j=0;
          for(int y=0; y<argc-4; ++y){
              ifstream infile(files[y].c_str());
              while (!infile.eof()){
                  infile >> contenido[j];
                  ++j;
                  if (j%3 == 0){
                    idFicheros = j;
                    outfile << "id:" << idFicheros/3 << '\n';
                    ++idFicheros;
                  }
              }
            }
            outfile.close();
            //----------------------------------------------------------------------------------//
        }
    }
    
    if (arg1 == "ctrl"){ //mostrar estado del sistema, agregar reactivos
        //TODO: falta agregar la condicion del -n
        //-n nombre memoria compartida
        //revisarSistema(); 
        string command[3];
        string linea;
        int i=0;
        cout << "> " << endl;
          while(getline(cin,linea)){
            stringstream ssin(linea);
            while (ssin.good() && i <2){
                ssin >> command[i];
                ++i;
                //cout << i << endl;
            }
            //TODO: quitar los mostrar y hacer los cout de lo que se necesita
            if(command[0] == "list"){
              //if(command[1] == "all"){
              //  cout << " all" << endl;
              //  i=0;
              //}
              if(command[1] == "reactive"){
                cout << "{B D S}  reactive" << endl;
                i=0;
              }
              if(command[1] == "reported"){
                cout << "[id i k r] reported" << endl;
                i=0;
              }
              if(command[1] == "waiting"){
                cout << "[id i k q] waiting" << endl;
                i=0;
              }
              if(command[1] == "processing"){
                cout << "[id i k q p] processing" << endl;
                i=0;
              }
            } 
            if(command[0] == "update") {
              string valor;
              string tipo;
              string line;
              string updateValues[3];
              int count;
              //TODO:hay que agregar cosas a esto
              cout << "update mode " << command[0] << endl;
              while(getline(cin,line)){
                stringstream ssin(line);
                while (ssin.good()){  
                 ssin >> updateValues[count];
                  ++count;
                }
                tipo = updateValues[1];
                valor = updateValues[2];
                cout << "tipo:" << tipo << endl;
                cout << "valor:" << valor << endl;
                count =0;
              }
            }
          }
    }
    if (arg1 == "rep"){
        string linea;
        int tiempo;
        if(std::string (argv[2]) == "-n"){
          nombreMemoriaCompartida = argv[3];
          memComp = "/"+nombreMemoriaCompartida;
        }
        if(std::string (argv[4]) == "-i"){
            while(getline(cin,linea)){
              stringstream ssin(linea);
              while(ssin.good()){
                ssin >> tiempo;
              }
              sleep(tiempo);
              cout << "tiempo:" << tiempo << endl;  
            }
            cout << "-i [id i k r]" << endl;
        } else {
          //-m 10 espero recibir el reporte de 10 muestras
          //saca el reporte de la cola de salida
          cout << "-m [id i k r]" << endl;
        }
      }
      //reportarResultados();
    
    if (arg1 == "stop"){
      if( (argc>2) && (std::string (argv[2]) == "-n")){
          nombreMemoriaCompartida = argv[3];
          memComp = "/"+nombreMemoriaCompartida;
      }     
       //deleteMemoriaCOmpartida();
    }
    return EXIT_SUCCESS;
}