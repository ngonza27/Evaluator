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
  int entradasEntra; //-i numero entradas
  int tamanoColasExternas; //-ie numero posiciones de cada cola
  int entradasSalida; //-oe n numero entradas cola de salida
  int tamanoColasInternas ; //-q
} Header;

struct BandejasEntrada{
  int entra;
  int sale;
  int cantidad;
  int tamano;
  int colasEntra[0];
} BandejasEntrada;

struct muestra {
  int i;
  char k;
  int cantidad;
} muestra;

struct BandejaSalida{
  int entra;
  int sale;
  int cantidad;
  int tamano;
  //int tamanoSalida[tamanoColasExternas];
} BandejaSalida;