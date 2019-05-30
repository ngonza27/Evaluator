#pragma once

//-ie
#define tamanoBufferEntrada 6

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
