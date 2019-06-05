#pragma once
#define tamanoBufferEntrada 99

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

struct Header {
  int entradasEntra = 5; //-i
  int tamanoColasExternas = 6; //-ie
  int entradasSalida = 10; //-oe
  //int B = 100; //-b
  //int D = 100; //-d
  //int S = 100; //-s
  int tamanoColasInternas = 6; //-q
};

struct BandejasEntrada{
  int entra;
  int sale;
  int cantidad;
  int tamano;
  //int posicionesEntra[tamanoColasExternas];
};

struct BandejaSalida{
  int entra;
  int sale;
  int cantidad;
  int tamano;
  //int tamanoSalida[tamanoColasExternas];
};