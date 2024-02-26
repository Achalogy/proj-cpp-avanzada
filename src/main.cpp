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

int crearMenuApuntadores(int cantOpciones, string *opciones);
void crearPartida();
void pause();
void clearConsole();
void renderMessage(string str);
void menuPrincipal();
void mostrarPokemones(SPlayer *jugador, int h1, int h2);
string padEnd(string str, int size);
void cargarPokemones(string path, SPlayer *jugador, bool mainPlayer);
int contarPokemones(SPlayer *p);
SPokemon *encontrarDirPokemon(SPlayer *p, int pos);
void intercambiarPokemones();
int getSizeWithoutAccents(string str);

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
    clearConsole();
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
      clearConsole();
      return 0;
      break;

    default:
      break;
    }
  }

  return 0;
}

void cargarPokemones(string path, SPlayer *jugador, bool mainPlayer)
{
  ifstream file;
  string line;
  int a = 0;

  SPokemon *primero = NULL;
  SPokemon *anterior = NULL;

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede escribir el archivo" << endl;
  }

  while (!file.eof())
  {
    SPokemon *pokemon = new SPokemon;
    getline(file, line);
    string prop = "";
    string value = "";
    bool flag = false;

    for (int i = 0; i < line.size(); i++)
    {
      char c = line[i];

      if (c == '-' || (i + 1) == line.size())
      {
        if (i + 1 == line.size())
          value += c;

        // Asignar valores a la estructura
        if (prop == "nombre")
          strcpy(pokemon->name, value.c_str());
        if (prop == "ataque")
          pokemon->attack = (short)stoi(value);
        if (prop == "vida")
          pokemon->live = (short)stoi(value);
        if (prop == "velocidad")
          pokemon->speed = (short)stoi(value);
        if (prop == "nivel")
          pokemon->level = (short)stoi(value);

        prop = "";
        value = "";
        flag = false;

        if ((i + 1) == line.size())
        {
          pokemon->next = NULL;
          pokemon->previous = NULL;
          pokemon->game = partidasGuardadas;
          pokemon->mainPlayer = mainPlayer;

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
        }
      }
      else if (c == ':')
        flag = true;
      else if (flag)
        value += c;
      else
        prop += c;
    }
  }

  jugador->team = primero;

  file.close();
}

void mostrarPokemones(SPlayer *p, int h1 = -1, int h2 = -1)
{
  SPokemon *actual = p->team;
  int i = 0;

  do
  {
    // cout << "Dirección: " << actual << endl;
    cout << "+--------------------------------+" << endl;
    cout << (i == h1 || i == h2 ? "  : " : "| ") << padEnd("Nombre: " + (string)actual->name, 30) << " |" << endl;
    cout << (i == h1 || i == h2 ? "  : " : "| ") << padEnd("Ataque:    " + to_string(actual->attack), 30) << " |" << endl;
    cout << (i == h1 || i == h2 ? "  : " : "| ") << padEnd("Vida:      " + to_string(actual->live), 30) << " |" << endl;
    cout << (i == h1 || i == h2 ? "  : " : "| ") << padEnd("Velocidad: " + to_string(actual->speed), 30) << " |" << endl;
    cout << (i == h1 || i == h2 ? "  : " : "| ") << padEnd("Nivel:     " + to_string(actual->level), 30) << " |" << endl;
    // cout << "Siguiente: " << actual->next << endl;
    // cout << "Anterior:  " << actual->previous << endl
    actual = actual->next;
    i++;
  } while (actual != p->team);
  cout << "+--------------------------------+" << endl;
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

  partidasGuardadas++;

  cin >> nombre;
  strcpy(jugador->name, nombre.c_str());
  jugador->mainPlayer = true;
  jugador->team = NULL;
  jugador->game = partidasGuardadas;

  cin.ignore();

  clearConsole();
  renderMessage("¿Así que te llamas " + nombre + "?");
  pause();

  strcpy(enemigo->name, "Alain");
  enemigo->team = NULL;
  enemigo->game = partidasGuardadas;

  cargarPokemones("main.txt", jugador, true);
  cargarPokemones("enemy.txt", enemigo, false);

  renderMessage("De acuerdo, " + nombre + ". Estás a punto de dar tus primeros pasos en la región de Jave.");
  pause();
  renderMessage("¡Vivirás emocionantes aventuras junto a humanos y Pokémon!");
  pause();

  menuPrincipal();
}

int contarPokemones(SPlayer *p)
{
  SPokemon *actual = p->team;
  int i = 0;

  do
  {
    actual = actual->next;
    i++;
  } while (actual != p->team);

  return i;
}

SPokemon *encontrarDirPokemon(SPlayer *p, int pos)
{
  SPokemon *actual = p->team;
  int i = 0;

  do
  {
    if (i == pos)
      return actual;

    i++;
    actual = actual->next;
  } while (actual != p->team);

  return p->team; // en caso de error retornar la primer pos
}

void intercambiarPokemones()
{
  int pos1 = 0;
  int pos2 = 0;
  int max = contarPokemones(jugador);
  int confirmation = -1;

  do
  {
    clearConsole();
    mostrarPokemones(jugador);
    renderMessage("¿Que posición inicial desea intercambiar? (0 para cancelar)");
    cout << " > ";
    cin >> pos1;
    pos1--;

    if (pos1 == -1)
      return;
  } while (pos1 <= 0 && pos1 > max);
  do
  {
    clearConsole();
    mostrarPokemones(jugador, pos1);
    renderMessage("¿A que posición desea intercambiar? (0 para cancelar)");
    cout << " > ";
    cin >> pos2;
    pos2--;

    if (pos2 == -1)
      return;
  } while (pos2 <= 0 && pos2 > max);
  mostrarPokemones(jugador, pos1, pos2);

  cin.ignore();

  renderMessage("Confirmas que deseas intercambiar las posiciones " + to_string(pos1 + 1) + " y " + to_string(pos2 + 1) + "? (y/n)");

  do
  {
    char i;
    cout << "> ";
    cin >> i;

    confirmation = (i == 'y' || i == 'Y') ? 1 : (i == 'n' || i == 'N') ? 0
                                                                       : -1;
  } while (confirmation == -1);

  if (confirmation == 0)
    return;

  SPokemon *dir1 = encontrarDirPokemon(jugador, pos1);
  SPokemon *dir2 = encontrarDirPokemon(jugador, pos2);

  SPokemon aux = *dir1;

  dir1->attack = dir2->attack;
  dir1->level = dir2->level;
  dir1->live = dir2->live;
  strcpy(dir1->name, dir2->name);
  dir1->speed = dir2->speed;

  dir2->attack = aux.attack;
  dir2->level = aux.level;
  dir2->live = aux.live;
  strcpy(dir2->name, aux.name);
  dir2->speed = aux.speed;

  cin.ignore();

  mostrarPokemones(jugador);
  renderMessage("Posiciones intercambiadas con exito.");
}

void menuPrincipal()
{
  string *opciones = new string[4];

  *(opciones) = "Mostrar pokemones";
  *(opciones + 1) = "Reordenar";
  *(opciones + 2) = "Luchar";
  *(opciones + 3) = "Guardar y Salir";

  while (true)
  {
    int opt = crearMenuApuntadores(4, opciones);
    cin.ignore(); // Ya que se envia un número

    switch (opt)
    {
    case 1:
      mostrarPokemones(jugador);
      break;
    case 2:
      intercambiarPokemones();
      break;
    case 3:
      break;
    case 4:
      return;
      break;
    }

    pause();
    clearConsole();
  }
}

int getSizeWithoutAccents(string str)
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

  if (str.back() == '\r')
  {
    // Fix: Al intentar agregar espacios a un string, si termina con \r se reemplazaran los primeros char
    str.pop_back();
  }

  int strSize = getSizeWithoutAccents(str);

  for (int i = 0; i < size - strSize; i++)
  {
    str = str + " ";
  }

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

  do
  {
    cout << "+--------------------------------------------------------------+" << endl;
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