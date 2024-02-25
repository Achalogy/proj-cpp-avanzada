#include <iostream>
#include <cstring>

using namespace std;
int crearMenuApuntadores(int cantOpciones, string *opciones);
void crearPartida();
void pause();
void clearConsole();

struct SPokemon
{
  char name[20];
  short level = 0;
  short attack = 0;
  short live = 0;
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

SPlayer *jugador = new SPlayer;
SPlayer *enemigo = new SPlayer;
int partidasGuardadas = 0;

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
    cin.ignore(); // Ya que se envia un número

    switch (opt)
    {
    case 1:
      crearPartida();
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

void pause()
{
  do
  {
    cout << '\n'
         << "Presiona una tecla para continuar... ";
  } while (cin.get() != '\n');
  clearConsole();
}

void crearPartida()
{
  string nombre;

  clearConsole();
  cout << "¡Te doy la bienvenida al mundo de los Pokémon!" << endl;
  pause();
  cout << "Me llamo Acha y soy el Profesor Pokémon de la región de Jave." << endl;
  pause();
  cout << "¡Este mundo está lleno de unas criaturitas adorables llamadas Pokémon!" << endl;
  pause();
  cout << "Y ahora, háblame un poco de ti. Veamos..." << endl;
  cout << "¿Cómo te llamas?" << endl
       << endl
       << " > ";

  cin >> nombre;
  strcpy(jugador->name, nombre.c_str());
  jugador->mainPlayer = true;
  jugador->team = NULL;
  jugador->game = partidasGuardadas + 1;

  strcpy(enemigo->name, "Alain");
  enemigo->team = NULL;
  enemigo->game = partidasGuardadas + 1;
}

int crearMenuApuntadores(int cantOpciones, string *opciones)
{
  // Retorna el entero proporcionado por el usuario que identifica la opción elegida.

  int opcionInput = -1;

  cout << "===========================================" << endl;
  do
  {
    for (int i = 0; i < cantOpciones; i++)
      cout << i + 1 << ". " << *(opciones + i) << endl;
    cout << "===========================================" << endl
         << endl;
    cout << " > ";

    cin >> opcionInput;
    cout << endl;
    // Volver a enviar el menu en caso de no recibir una opción valida34
  } while (opcionInput <= 0 || opcionInput > cantOpciones);

  return opcionInput;
}

void clearConsole()
{
  // Limpia la consola, usa un if para saber si es linux o windows

#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}