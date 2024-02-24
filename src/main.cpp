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
  return 0;
}