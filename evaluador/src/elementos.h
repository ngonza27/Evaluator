#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <semaphore.h>
#include <cerrno>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sstream>
#include <fstream>

#pragma once
#define tamanoBufferEntrada 5
#define tamanoBufferSalida 10
#define semName "sem"

sem_t semagoros;
sem_t arrSemaforos[tamanoBufferEntrada];

struct Tipo{
  int b;
  int d;
  int s;
};

struct Simulador{
  int entra;
  int sale;
  int cantidad;
  int tamano; 
};
 
struct Muestra{
  int idCola;
  int id;
  std::string k;
  int cantidad;
};

struct Cola{
  struct Simulador simulador;
  struct Muestra muestra[tamanoBufferEntrada];
};

struct BandejasEntrada{
  int id;
  int entradasEntra; //-i numero entradas
  int tamanoColasExternas; //-ie numero posiciones de cada cola
  int entradasSalida; //-oe n numero entradas cola de salida
  int tamanoColasInternas ; //-q
  struct Cola cola[tamanoBufferEntrada];
};

struct BandejaSalida{
  int entra;
  int sale;
  int cantidad;
  int tamano;
  struct Cola cola[tamanoBufferEntrada];
};

struct Buffer {
  struct Simulador simulador;
  struct Tipo tipo;
  struct BandejasEntrada bandejaEntrada;
  struct BandejaSalida bandejaSalida;
} buffer;