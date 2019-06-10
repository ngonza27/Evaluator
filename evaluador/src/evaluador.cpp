#include "elementos.h"

using namespace std;

string nombreMemoriaCompartida = "evaluator"; //-n
string memComp = "/"+nombreMemoriaCompartida;

int
initMemoriaCompartidaEntrada(int iInit, int ieInit, int oeInit, int bInit, int dInit, int sInit, int qInit) {


  sem_t *vacios = sem_open("vacios", O_CREAT | O_EXCL, 0660, tamanoBufferEntrada);
  sem_t *llenos = sem_open("llenos", O_CREAT | O_EXCL, 0660, 0);
  sem_t *mutex  = sem_open("mutex", O_CREAT | O_EXCL, 0660, 1);

  int fd = shm_open(memComp.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  //sem_init(sem, 1, 1);

  int tamanoTotal = (sizeof(struct BandejasEntrada)*iInit*ieInit +  
                     sizeof(struct BandejaSalida)*qInit +
                     sizeof(struct Tipo) +
                     sizeof(sem_t)*iInit*3 +
                     sizeof(sem_t)*3
                    );

  if (ftruncate(fd, tamanoTotal) != 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  //--------------------------------------------------------------------------------------------------------------------//
  //Mapeo
  void *dir;

   if ((dir = mmap(NULL, (sizeof(struct Buffer)), PROT_READ | PROT_WRITE, MAP_SHARED,
	 	  fd, 0)) == MAP_FAILED) {
     cerr << "Error mapeando la memoria compartida ENTRADA: "
	  << errno << strerror(errno) << endl;
     exit(1);
   }

  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  struct Buffer *pBuffer = (struct Buffer *)dir;
  for(int i=0; i<iInit; ++i){
    pBuffer->bandejaEntrada.cola[i].simulador.entra=0; //-i
    pBuffer->bandejaEntrada.cola[i].simulador.sale=0; //-i
    pBuffer->bandejaEntrada.cola[i].simulador.cantidad=0; //-i
    pBuffer->bandejaSalida.cola[i].simulador.entra=0; 
    pBuffer->bandejaSalida.cola[i].simulador.sale=0;
    pBuffer->bandejaSalida.cola[i].simulador.cantidad=0;
  }
  pBuffer->cant=iInit;
  pBuffer->tipo.b=bInit;
  pBuffer->tipo.d=dInit;
  pBuffer->tipo.s=sInit;
  // //--------------------------------------------------------------------------------------------------------------------//

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
  
  //hilosMultiples();
  //hilosInternos();
  close(fd);
  for(;;){}
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

void mostrar(string queMuestro, string memoria){
  char res;
  int ran;
  ran = rand() % ((51-14)+1) + 1;
  if(res >= 50){
    res = 'P';
  } else if(res <= 15){
     res = '?';
  } else{
    res = 'N';
  }
  int fd = shm_open(memComp.c_str(), O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  //Mapeo TIPOS
  if ((dir = mmap(NULL, (sizeof(struct Tipo)), PROT_READ | PROT_WRITE, MAP_SHARED,
  		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida HEADER: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  
  struct Buffer *pBuffer = (struct Buffer *)dir;
  int num = pBuffer->cant;

  if(queMuestro == "reactive"){
    cout << "B: " << pBuffer->tipo.b
         << "\nD: " << pBuffer->tipo.d
         << "\nS: " << pBuffer->tipo.s << endl;
  }
  for(int i=0; i <num-1 ;++i){
  if(queMuestro == "processing"){
    //[id i k q p NL]
  }
  if(queMuestro == "waiting"){
    //[id i k q NL]
    cout << "id: "  <<  pBuffer->bandejaEntrada.cola[i].muestra[0].idCola
         << "\ni (cola entrada); " << pBuffer->bandejaEntrada.cola[i].muestra[1].id
         << "\nk (tipo muestra); " << pBuffer->bandejaEntrada.cola[i].muestra[2].k
         << "\nq (cantidad de muestra)" << pBuffer->bandejaEntrada.cola[i].muestra[3].cantidad
         << endl;
  }
  if(queMuestro == "reported"){
    //[id i k r NL]
    cout << "id: "   << pBuffer->bandejaSalida.cola[i].muestra[0].idCola
         << "\ni (cola entrada); " << pBuffer->bandejaSalida.cola[i].muestra[2].id
         << "\nk (tipo muestra); " << pBuffer->bandejaSalida.cola[i].muestra[3].k
         << "\nr (resultado  de la muestra P,N,?)" << res
         << endl;
  }
  }
}

void actualizar(string tipo, string valor, string memComp){
  int fd = shm_open(memComp.c_str(), O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  if ((dir = mmap(NULL, (sizeof(struct Tipo)), PROT_READ | PROT_WRITE, MAP_SHARED,
  		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida HEADER: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  struct Buffer *pBuffer = (struct Buffer *)dir;

  if(tipo == "B"){
    pBuffer->tipo.b=pBuffer->tipo.b+stoi(valor);
    cout << "B:" << pBuffer->tipo.b << endl;
  } else if (tipo == "D"){
    pBuffer->tipo.d=pBuffer->tipo.d+stoi(valor);
    cout << "D:" << pBuffer->tipo.d << endl;
  } else {
    pBuffer->tipo.s=pBuffer->tipo.s+stoi(valor);
    cout << "S:" << pBuffer->tipo.s << endl;
  }
}

int procesarExamenes(string tipo,string memoria){
  sem_t *sem = sem_open(semName, 0);
  int res;
  if(tipo == "B"){
    res = rand() % ((7-1) +1) + 1;
  }
  if(tipo == "D"){
    res = rand() % ((20-5) +1) + 1;
  }
  if(tipo == "S"){
    res = rand() % ((25-8) +1) + 1;
  }
  return res;
}

char resultadosExamenes(string tipo, string memoria){
  char res;
  int ran;
  ran = rand() % ((51-14)+1) + 1;
  if(res >= 50){
    res = 'P';
  } else if(res <= 15){
     res = '?';
  } else{
    res = 'N';
  }
  return res;
}

//waiting
void agregarColaEntrada(int id, string cola, string tipo, string valor, string memComp){
  int fd = shm_open(memComp.c_str(), O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  //Mapeo TIPOS
  if ((dir = mmap(NULL, (sizeof(struct Tipo)), PROT_READ | PROT_WRITE, MAP_SHARED,
  		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida HEADER: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }
  char c;
  strcpy(&c,tipo.c_str());
  struct Buffer *pBuffer = (struct Buffer *)dir;

  if(tipo == "B"){
    pBuffer->bandejaEntrada.cola[id-1].muestra[0].idCola=id;
    pBuffer->bandejaEntrada.cola[id-1].muestra[1].id=stoi(cola); 
    pBuffer->bandejaEntrada.cola[id-1].muestra[2].k=c;
    pBuffer->bandejaEntrada.cola[id-1].muestra[3].cantidad=stoi(valor); 
  } else if (tipo == "D"){
    pBuffer->bandejaEntrada.cola[id-1].muestra[0].idCola=id;
    pBuffer->bandejaEntrada.cola[id-1].muestra[1].id=stoi(cola); 
    pBuffer->bandejaEntrada.cola[id-1].muestra[2].k=c; 
    pBuffer->bandejaEntrada.cola[id-1].muestra[3].cantidad=stoi(valor); 
  } else {
    pBuffer->bandejaEntrada.cola[id-1].muestra[0].idCola=id;
    pBuffer->bandejaEntrada.cola[id-1].muestra[1].id=stoi(cola); 
    pBuffer->bandejaEntrada.cola[id-1].muestra[2].k=c; 
    pBuffer->bandejaEntrada.cola[id-1].muestra[3].cantidad=stoi(valor); 
  }
}

//reported
void agregarColaSalida(int id,string cola, string tipo, string valor, string memComp){
  int fd = shm_open(memComp.c_str(), O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	       << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  //Mapeo TIPOS
  if ((dir = mmap(NULL, (sizeof(struct Tipo)), PROT_READ | PROT_WRITE, MAP_SHARED,
  		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida HEADER: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  char c;
  strcpy(&c,tipo.c_str());
  struct Buffer *pBuffer = (struct Buffer *)dir; 

  if(tipo == "B"){
    pBuffer->bandejaSalida.cola[id-1].muestra[0].idCola=id;
    pBuffer->bandejaSalida.cola[id-1].muestra[1].id=stoi(cola); 
    pBuffer->bandejaSalida.cola[id-1].muestra[2].k=c; 
    pBuffer->bandejaSalida.cola[id-1].muestra[3].cantidad=stoi(valor); 
  } else if (tipo == "D"){
    pBuffer->bandejaSalida.cola[id-1].muestra[0].idCola=id;
    pBuffer->bandejaSalida.cola[id-1].muestra[1].id=stoi(cola); 
    pBuffer->bandejaSalida.cola[id-1].muestra[2].k=c; 
    pBuffer->bandejaSalida.cola[id-1].muestra[3].cantidad=stoi(valor); 
  } else {
    pBuffer->bandejaSalida.cola[id-1].muestra[0].idCola=id;
    pBuffer->bandejaSalida.cola[id-1].muestra[1].id=stoi(cola); 
    pBuffer->bandejaSalida.cola[id-1].muestra[2].k=c; 
    pBuffer->bandejaSalida.cola[id-1].muestra[3].cantidad=stoi(valor); 
  }

}

int
deleteMemoriaCOmpartida(void) {
  cout << "Borrando la memoria compartida: " << memComp << endl;
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
          if((ii || ie || q || oe || d || s || b) < 0){
            cerr << "Porfavor ingresar valores positivos" << endl;
          }

          if(std::string (argv[i]) == "-i"){ ii = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-ie"){ ie = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-oe"){ oe = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-b"){ b = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-d"){ d = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-s"){ s = atoi(argv[i+1]);  }
          if(std::string (argv[i]) == "-q"){ q = atoi(argv[i+1]);  }
        }
        cout << "Numero de entradas:" << ii << endl;
        cout << "Posiciones cola entrada:" << ie << endl;
        cout << "Entradas colas de salida:" << oe << endl;
        cout << "Nombre memoria compartida:" << memComp << endl;
        cout << "Blood:" << b << endl;
        cout << "Ditritos:" << d << endl;
        cout << "Skin:" << s << endl;
        cout << "Tamano colas internas:" << s << endl;
        initMemoriaCompartidaEntrada(ii,ie,oe,b,d,s,q);
    }

    if(arg1 == "reg"){ //registrar examenes
        if (argv[2] == NULL)
            cout << "ERRRRRRRROR PS" << endl;
        if( (argc>2) && (std::string (argv[2]) == "-n")){
          nombreMemoriaCompartida = argv[3];
          memComp = "/"+nombreMemoriaCompartida;
        }     
        //MODO INTERACTIVO:
        if(std::string (argv[4]) == "-"){
          int id=0;
          string linea;
          string arr[3];
          string cola,tipo,cantidad;
          int i = 0; //CON ESTE i PUEDO GENERAR EL IDENTIFICADOR
          cout << "> " << endl;
          while(getline(cin,linea)){
            cout << "> " << endl;
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
            agregarColaEntrada(id,cola,tipo,cantidad,memComp);
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
          string colaFichero;
          string tipoFichero;
          string cantidadFichero;
          int counter=0;
          int aux;
          for(int y=0; y<argc-4; ++y){
              ifstream infile(files[y].c_str());
              while (!infile.eof()){
                  infile >> contenido[j];
                  //cout << contenido[j] << endl;
                  ++counter;
                  if (counter%3 == 0){
                    idFicheros = counter;
                    outfile << "id:" << idFicheros/3 << '\n';
                    ++idFicheros;
                    aux=idFicheros/3;
                  }
                  j++;
                  if(j == 3){
                    colaFichero=contenido[0];
                    tipoFichero=contenido[1];
                    cantidadFichero=contenido[2];
                    cout << memComp << endl;
                    agregarColaEntrada(aux,colaFichero,tipoFichero,cantidadFichero,memComp);        
                    j=0;
                  }
              }            }
            outfile.close();
            //----------------------------------------------------------------------------------//
        }
    }
    
    if (arg1 == "ctrl"){ 
        string command[3];
        string linea;
        int i=0;
        int num=0;
        if( (argc>2) && (std::string (argv[2]) == "-n")){
          nombreMemoriaCompartida = argv[3];
          memComp = "/"+nombreMemoriaCompartida;
        }     
        cout << "> " << endl;
          while(getline(cin,linea)){
            cout << "> " << endl;
            stringstream ssin(linea);
            command[0]="0";
            command[1]="0";
            command[2]="0";
            while (ssin.good()){
                ssin >> command[i];
                ++i;   
            }
          
            if(command[0] == "list"){
              if(command[1] == "reactive"){
                mostrar("reactive", memComp);
                i=0;
              }
              if(command[1] == "reported"){
                mostrar("reported", memComp);
                i=0;
              }
              if(command[1] == "waiting"){
                mostrar("waiting", memComp);
                i=0;
              }
              if(command[1] == "processing"){
                mostrar("processing", memComp);
                i=0;
              }
            } 
            if(command[0] == "update") {
              string valor;
              string tipo;
              tipo = command[1];
              valor = command[2];
              //cout << "tipo:" << tipo << endl;
              //cout << "valor:" << valor << endl;
              actualizar(tipo, valor, memComp);
              i=0;
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
          cout << "Insert integer: " << endl;
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
          cout << "-m [id i k r]" << endl;
        }
        //reportarResultados();
      }
    
    if (arg1 == "stop"){
      if( (argc>2) && (std::string (argv[2]) == "-n")){
          nombreMemoriaCompartida = argv[3];
          memComp = "/"+nombreMemoriaCompartida;
      }     
      deleteMemoriaCOmpartida();
    }
    return EXIT_SUCCESS;
}