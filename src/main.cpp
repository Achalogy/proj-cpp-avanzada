#include <iostream>
#include <cstring>
#include "acha/main.h"

using namespace std;

struct SPokemon
{
  char name[20];
  short level;
  short attack;
  short live;
  short speed;
  SPokemon *previous;
  SPokemon *next;
  bool mainPlayer;
  int game;
};

struct SPlayer
{
  char name[20];
  bool mainPlayer;
  int game;
  SPokemon *team;
};

int main(void)
{
  string *opciones = new string[3];

  *(opciones) = "Crear nueva partida";
  *(opciones + 1) = "Cargar partida";
  *(opciones + 2) = "Salir";

  cout << "+-----------------------------------------+" << endl;
  cout << "|                 POKÉMON                 |" << endl;
  cout << "|          Edición Programación           |" << endl;
  cout << "|                                         |" << endl;
  cout << "|               PRESS START               |" << endl;
  cout << "|             © 2024 Acha inc             |" << endl;
  cout << "+-----------------------------------------+" << endl
       << endl;

  while (true)
  {
    int opt = crearMenuApuntadores(3, opciones);
    switch (opt)
    {
    case 1:
      break;
    case 2:
      break;
    case 3:
      return 0;
      break;

    default:
      break;
    }
  }

  return 0;
}