#pragma once
#define tamanoBufferEntrada 99

#define entradasEntra 5 //-i
#define tamanoColasExternas 6 //-ie
#define entradasSalida 10 //-oe
#define B 100 //-b
#define D 100 //-d
#define S 100 //-s
#define tamanoColasInternas 6 //-q
//int tamanoBufferEntrada = 99;
//public solo se puede dentro de una clase
struct elemento {
  int elemento;
};

struct Buffer {
  int entra;
  int sale;
  int cantidad;
  int tamano;
  struct elemento buffer[tamanoBufferEntrada];
};

struct BandejasEntrada{
  int posicionesEntra[tamanoColasExternas];
};

struct BandejasInternas{
  int tamanoInternas[tamanoColasInternas];
};

struct BandejaSalida{
  int tamanoSalida[tamanoColasExternas];
};