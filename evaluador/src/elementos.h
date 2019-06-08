#pragma once
#define tamanoBufferEntrada 99

<<<<<<< HEAD
struct Tipo{
  int b;
  int d;
  int s;
=======
enum test_type{B, S, D};
enum test_result{p, n, r};

struct exam{
  int id;
  test_type type;
  test_result res;
  int quantity;
  int queue;
  int timeProcessing;
  exam() {}
  exam(int id, test_type tipo, int q, int queueIn);
};

struct elemento {
  int elemento;
>>>>>>> d8a4f6e446db2bee097bd916ee01d7d461c3449d
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