#pragma once
#define tamanoBufferEntrada 99


struct Tipo{
  int b;
  int d;
  int s;
};
 
struct BandejasEntrada{
  int entradasEntra; //-i numero entradas
  int tamanoColasExternas; //-ie numero posiciones de cada cola
  int entradasSalida; //-oe n numero entradas cola de salida
  int tamanoColasInternas ; //-q
  int colasEntra[0];
};

struct Muestra{
  int id;
  char k;
  int cantidad;
};

struct BandejaSalida{
  int entra;
  int sale;
  int cantidad;
  int tamano;
};

struct Buffer {
  int entra;
  int sale;
  int cantidad;
  int tamano;
  struct Tipo tipo;
  struct BandejasEntrada bandejaEntrada;
  struct BandejaSalida bandejaSalida[tamanoBufferEntrada];
  struct Muestra muestra[tamanoBufferEntrada];
} buffer;