#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

const int ALTO = 7121;
const int LARGO = 10681;

// Función que convierte un símbolo a un número, considerando que "*" se convierte a 300
int simbolo_a_numero(string &simbolo) {
  if (simbolo == "*") {
    return 300;
  }

  return stoi(simbolo);
}

int main() {
  // Abrir archivos de texto para lectura
  ifstream fileP("promedio.txt");
  ifstream fileA("alfa.txt");
  ifstream fileB("azul.txt");
  ifstream fileR("rojo.txt");
  ifstream fileG("verde.txt");

  // Vectores para almacenar los datos leídos desde los archivos
  vector<uint32_t> v_rojo(LARGO * ALTO);
  vector<uint32_t> v_verde(LARGO * ALTO);
  vector<uint32_t> v_azul(LARGO * ALTO);
  vector<uint32_t> v_alfa(LARGO * ALTO);
  vector<uint32_t> v_promedio(LARGO * ALTO);

  uint32_t rojo, verde, azul;

  // Puntero para almacenar los píxeles de la imagen final
  uint32_t *pixels = new uint32_t[ALTO * LARGO];

  cout << "Poblando arreglos" << endl;

  // Leer datos desde los archivos y almacenarlos en los vectores
  for (int i = 0; i < ALTO * LARGO; i++) {
    string aux;
    fileA >> aux;
    v_alfa[i] = simbolo_a_numero(aux);
    fileR >> aux;
    v_rojo[i] = simbolo_a_numero(aux);
    fileG >> aux;
    v_verde[i] = simbolo_a_numero(aux);
    fileB >> aux;
    v_azul[i] = simbolo_a_numero(aux);
    fileP >> aux;
    v_promedio[i] = simbolo_a_numero(aux);
  }

  // Cerrar los archivos de texto
  fileP.close();
  fileA.close();
  fileB.close();
  fileR.close();
  fileG.close();

  cout << "Generando pixeles" << endl;

  // Procesar los datos y generar los píxeles de la imagen final
#pragma omp parallel for
  for (int i = 0; i < ALTO * LARGO; i++) {

    // Ajustar el valor de rojo si es mayor que 255
    if (v_rojo[i] > 255) {
      rojo = v_promedio[i] * 0.3;
    } else {
      rojo = v_rojo[i];
    }

    // Ajustar el valor de verde si es mayor que 255
    if (v_verde[i] > 255) {
      verde = v_promedio[i] * 0.59;
    } else {
      verde = v_verde[i];
    }

    // Ajustar el valor de azul si es mayor que 255
    if (v_azul[i] > 255) {
      azul = v_promedio[i] * 0.11;
    } else {
      azul = v_azul[i];
    }

    // Combinar los canales alfa, rojo, verde y azul para formar un píxel
    pixels[i] = (v_alfa[i] << 24 | azul << 16 | verde << 8 | rojo);
  }

  cout << "Escribiendo archivo" << endl;
  // Escribir la imagen en formato JPG
  stbi_write_jpg("salida.jpg", LARGO, ALTO, 4, pixels, 10);

  return 0;
}
