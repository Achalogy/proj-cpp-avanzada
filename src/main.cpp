#include <iostream>
#include <cstring>

using namespace std;
int crearMenuApuntadores(int cantOpciones, string *opciones);
void crearPartida();
void pause();
void clearConsole();
void renderMessage(string str);

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

  cout << "+--------------------------------------------------------------+" << endl;
  cout << "|                           POKÉMON                            |" << endl;
  cout << "|                    Edición Programación                      |" << endl;
  cout << "|                                                              |" << endl;
  cout << "|                         PRESS START                          |" << endl;
  cout << "|                       © 2024 Acha inc                        |" << endl;
  cout << "+--------------------------------------------------------------+" << endl
       << endl;

  pause();

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
         << "Presiona enter para continuar... ";
  } while (cin.get() != '\n');
  clearConsole();
}

void crearPartida()
{
  string nombre;

  clearConsole();
  renderMessage("¡Te doy la bienvenida al mundo de los Pokémon!");
  pause();
  renderMessage("Me llamo Acha y soy el Profesor Pokémon de la región de Jave.");
  pause();
  renderMessage("¡Este mundo está lleno de unas criaturitas desdichadas (Álgebra) llamadas Pokémon!");
  pause();
  renderMessage("Y ahora, háblame un poco de ti. Veamos... ¿Cómo te llamas?");
  cout << endl
       << " > ";

  cin >> nombre;
  strcpy(jugador->name, nombre.c_str());
  jugador->mainPlayer = true;
  jugador->team = NULL;
  jugador->game = partidasGuardadas + 1;

  strcpy(enemigo->name, "Alain");
  enemigo->team = NULL;
  enemigo->game = partidasGuardadas + 1;

  cin.ignore();
  pause();
}

int getSizeWithoutAccents(const string &str)
{
  int size = 0;
  int accents = 0;

  for (int i = 0; i < str.size(); i++)
  {
    char c = str[i];
    size++;
    if (!(c >= 32 && c <= 122))
      accents++;
  }

  return size - (accents / 2);
}

string padEnd(string str, int size)
{
  // Agrega espacios al final

  int strSize = getSizeWithoutAccents(str);

  for (int i = 0; i < size - strSize; i++)
    str += " ";

  return str;
}

void renderMessage(string str)
{
  int size = str.size();
  int line = 1;
  int i = 0;
  int a = 0;
  string word = "";
  int prevSize = 0;

  cout << "+--------------------------------------------------------------+" << endl;

  for (line; line <= 3; line++)
  {
    string renderLine = "";
    prevSize = word.size();

    for (a; a < (60 * line) - prevSize && i <= size + 1; a++)
    {
      if (str[i] == ' ' || str[i] == '\0')
      {
        renderLine += word + " ";
        word = "";
      }
      else
        word += str[i];

      i++;
    }
    a = 60 * line;
    cout
        << "| ";
    cout << padEnd(renderLine, 60);
    cout << " |" << endl;
    renderLine = "";
  }

  cout << "+--------------------------------------------------------------+" << endl;
}

int crearMenuApuntadores(int cantOpciones, string *opciones)
{
  // Retorna el entero proporcionado por el usuario que identifica la opción elegida.

  int opcionInput = -1;

  cout << "+--------------------------------------------------------------+" << endl;
  do
  {
    for (int i = 0; i < cantOpciones; i++)
      cout << "| " << padEnd(to_string(i + 1) + ". " + *(opciones + i), 60) << " |" << endl;
    cout << "+--------------------------------------------------------------+" << endl
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