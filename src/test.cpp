#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

struct SPokemon
{
  char name[20];
  short level = 0;
  short attack = 0;
  short live = 0;
  short maxLive = 0;
  short speed = 0;
  SPokemon *previous;
  SPokemon *next;
  bool mainPlayer = false;
  int game = 0;
};

struct SPlayer
{
  char name[20];
  bool mainPlayer = false;
  int game = 0;
  SPokemon *team;
};

void leerPokemonesBin();

SPlayer *jugador = new SPlayer;
SPlayer *enemigo = new SPlayer;
int main()
{
  ifstream jugadores;

  jugadores.open("partidas.bin", ios::in | ios::out | ios::binary);

  if (!jugadores.is_open())
    cout << "Error abriendo el archivo";

  jugadores.seekg(0, ios::end);
  cout << "Cantidad partidas guardadas: " << (int)jugadores.tellg() / (sizeof(SPlayer) * 2) << endl;
  jugadores.seekg(0, ios::beg);

  jugadores.read((char *)jugador, sizeof(SPlayer));
  jugadores.read((char *)enemigo, sizeof(SPlayer));

  // cout << (*jugador).name << " " << (*enemigo).name;

  // Leer pokemones

  leerPokemonesBin();
}

void leerPokemonesBin()
{
  ifstream pokemones;
  pokemones.open("pokemones.bin", ios::binary);
  SPokemon *primero = NULL;
  SPokemon *anterior = NULL;

  pokemones.seekg((jugador->game - 1) * sizeof(SPokemon) * 8);

  for (int i = 1; i <= 8; i++)
  {
    SPokemon *pokemon = new SPokemon;
    pokemones.read((char *)pokemon, sizeof(SPokemon));

    pokemon->next = NULL;
    pokemon->previous = NULL;
    pokemon->game = jugador->game;
    pokemon->mainPlayer = i <= 4 ? true : false;

    if (!primero)
      primero = pokemon;
    else
    {
      anterior->next = pokemon;
      pokemon->previous = anterior;
    }
    pokemon->next = primero;
    anterior = pokemon;
    primero->previous = pokemon;

    if (i == 4)
    {
      jugador->team = primero;
      primero = NULL;
      anterior = NULL;
    }
    else if (i == 8)
    {
      enemigo->team = primero;
    }
  }
}