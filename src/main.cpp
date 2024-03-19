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

int crearMenuApuntadores(int cantOpciones, string *opciones);
void crearPartida();
void pause();
void clearConsole();
void renderMessage(string str);
void menuPrincipal();
void mostrarPokemones(SPlayer *jugador, int h1, int h2);
string padEnd(string str, int size);
string padStart(string str, int size);
void cargarPokemones(string path, SPlayer *jugador);
int contarPokemones(SPlayer *p);
SPokemon *encontrarDirPokemon(SPlayer *p, int pos);
void intercambiarPokemones();
int getSizeWithoutAccents(string str);
int pokemonesVivos(SPlayer *p);
string barraPokemones(int vivos, int muertos);
string PSBar(int c, int max);
void renderBattle(SPokemon *poke1, SPokemon *poke2);
SPokemon *siguienteVivo(SPokemon *actual);
void realizarAtaque(SPokemon **poke1, SPokemon **poke2);
void batallaPokemon();
void cambiarPokemon(SPokemon **poke1, SPokemon **poke2);
void leerPokemonesBin();
int contarPartidasGuardadas();
void guardarPartida();
void cargarPartida();

SPlayer *jugador = new SPlayer;
SPlayer *enemigo = new SPlayer;
int partidasGuardadas = 0;

int main(void)
{
  string *opciones = new string[3];
  partidasGuardadas = contarPartidasGuardadas();

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
      cargarPartida();
      break;
    case 3:
      // Borrar memoria
      delete jugador;
      delete enemigo;
      clearConsole();
      return 0;

    default:
      break;
    }
  }

  return 0;
}

int contarPartidasGuardadas()
{
  ifstream jugadores;
  jugadores.open("partidas.bin", ios::in | ios::out | ios::binary);

  if (!jugadores.is_open())
  {
    cout << "Error abriendo el archivo de partidas guardadas, crearlo si no existe" << endl;
    pause();
    return 0;
  }

  jugadores.seekg(0, ios::end);
  int r = (int)jugadores.tellg() / (sizeof(SPlayer) * 2);
  jugadores.close();
  return r;
}

void cargarPartida()
{
  clearConsole();
  if (partidasGuardadas < 1)
  {
    renderMessage("No hay partidas guardadas");
    pause();
    return;
  };

  ifstream jugadores;
  string *opciones = new string[partidasGuardadas];
  jugadores.open("partidas.bin", ios::in | ios::out | ios::binary);

  if (!jugadores.is_open())
  {
    cout << "Error abriendo el archivo de partidas guardadas, crearlo si no existe" << endl;
    pause();
    return;
  }

  for (int i = 1; i <= partidasGuardadas; i++)
    *(opciones + i - 1) = "Partida " + to_string(i);
  int opt = crearMenuApuntadores(partidasGuardadas, opciones);

  jugadores.seekg((opt - 1) * sizeof(SPlayer) * 2);

  jugadores.read((char *)jugador, sizeof(SPlayer));
  jugadores.read((char *)enemigo, sizeof(SPlayer));

  leerPokemonesBin();

  renderMessage("Se ha cargado la partida con exito");

  // Borrar memoria
  jugadores.close();
  delete[] opciones;

  pause();
  menuPrincipal();
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

  pokemones.close();
}

void guardarPartida()
{
  ofstream jugadores;

  jugadores.open("partidas.bin", ios::in | ios::out | ios::binary);

  if (!jugadores.is_open())
  {
    cout << "Error abriendo el archivo partidas.bin" << endl;
    pause();
    return;
  }

  jugadores.seekp((jugador->game - 1) * sizeof(SPlayer) * 2);

  jugadores.write((char *)jugador, sizeof(SPlayer));
  jugadores.write((char *)enemigo, sizeof(SPlayer));

  ofstream pokemones;

  pokemones.open("pokemones.bin", ios::in | ios::out | ios::binary);

  if (!pokemones.is_open())
  {
    cout << "Error abriendo el archivo pokemones.bin" << endl;
    pause();
    return;
  }

  pokemones.seekp((jugador->game - 1) * sizeof(SPokemon) * 8);

  SPokemon *actual = jugador->team;

  do
  {
    pokemones.write((char *)actual, sizeof(SPokemon));
    actual = actual->next;
  } while (actual != jugador->team);

  actual = enemigo->team;

  do
  {
    pokemones.write((char *)actual, sizeof(SPokemon));
    actual = actual->next;
  } while (actual != enemigo->team);

  // Borrar memoria

  actual = jugador->team;

  do
  {
    actual = actual->next;
    delete actual->previous;
  } while (actual != jugador->team);

  actual = enemigo->team;

  do
  {
    actual = actual->next;
    delete actual->previous;
  } while (actual != enemigo->team);

  jugadores.close();
  pokemones.close();

  renderMessage("La partida número " + to_string(jugador->game) + " ha sido guardada con exito");
  pause();
}

void cargarPokemones(string path, SPlayer *jugador)
{
  ifstream file;
  char line[500];

  SPokemon *primero = NULL;
  SPokemon *anterior = NULL;

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede leer el archivo" << endl;
    return;
  }

  while (!file.eof())
  {
    SPokemon *pokemon = new SPokemon;

    file >> line;

    char *token;
    token = strtok(line, "-");

    while (token != NULL)
    {
      char *valor = strstr(token, ":") + 1;
      char *atrib = token;
      int atribSize = strlen(token) - strlen(valor) - 1;

      strncpy(atrib, token, atribSize);
      atrib[atribSize] = '\0';

      if (strcmp(atrib, (char *)"nombre") == 0)
        strcpy(pokemon->name, valor);
      if (strcmp(atrib, (char *)"velocidad") == 0)
        pokemon->speed = (short)stoi(valor);
      if (strcmp(atrib, (char *)"vida") == 0)
      {
        pokemon->live = (short)stoi(valor);
        pokemon->maxLive = (short)stoi(valor);
      }
      if (strcmp(atrib, (char *)"nivel") == 0)
        pokemon->level = (short)stoi(valor);
      if (strcmp(atrib, (char *)"ataque") == 0)
        pokemon->attack = (short)stoi(valor);

      token = strtok(NULL, "-");
    }

    // te amo <3

    pokemon->next = NULL;
    pokemon->previous = NULL;
    pokemon->game = partidasGuardadas;
    pokemon->mainPlayer = jugador->mainPlayer;

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

  jugador->team = primero;

  file.close();
}

void mostrarPokemones(SPlayer *p, int h1 = -1, int h2 = -1)
{
  SPokemon *actual = p->team;
  int i = 0;

  clearConsole();

  do
  {
    // cout << "Dirección: " << actual << endl;
    cout << "+--------------------------------+" << endl;
    cout << (i == h1 || i == h2 ? "  : " : "| ") << padEnd("Nombre:    " + (string)actual->name, 30) << " |" << endl;
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
  char nombre[50];

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
  strcpy(jugador->name, nombre);
  jugador->mainPlayer = true;
  jugador->team = NULL;
  jugador->game = partidasGuardadas;

  cin.ignore();

  clearConsole();
  renderMessage("¿Así que te llamas " + (string)nombre + "?");
  pause();

  strcpy(enemigo->name, "Alain");
  enemigo->team = NULL;
  enemigo->game = partidasGuardadas;

  cargarPokemones("main.txt", jugador);
  cargarPokemones("enemy.txt", enemigo);

  renderMessage("De acuerdo, " + (string)nombre + ". Estás a punto de dar tus primeros pasos en la región de Jave.");
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
  cin.ignore();

  clearConsole();
  mostrarPokemones(jugador, pos1, pos2);
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
  dir1->maxLive = dir2->maxLive;
  strcpy(dir1->name, dir2->name);
  dir1->speed = dir2->speed;

  dir2->attack = aux.attack;
  dir2->level = aux.level;
  dir2->live = aux.live;
  dir2->maxLive = aux.maxLive;
  strcpy(dir2->name, aux.name);
  dir2->speed = aux.speed;

  cin.ignore();

  mostrarPokemones(jugador);
  renderMessage("Posiciones intercambiadas con exito.");
}

int pokemonesVivos(SPlayer *p)
{
  SPokemon *actual = p->team;
  int i = 0;

  do
  {
    if (actual->live > 0)
      i++;
    actual = actual->next;
  } while (actual != p->team);

  return i;
}

string barraPokemones(int vivos, int muertos)
{
  string barra = "";

  while (vivos--)
    barra += "◒ ";
  while (muertos--)
    barra += "○ ";

  return barra;
}

string PSBar(int c, int max)
{
  int psPoint = max / 12;

  string bar = "";

  for (int i = 0; i < c / psPoint; i++)
    bar += "#";

  for (int i = 0; i < 12 - c / psPoint; i++)
    bar += "-";

  return bar;
}

void renderBattle(SPokemon *poke1, SPokemon *poke2)
{
  int mainPkmCant = contarPokemones(jugador);
  int enemigoPkmCant = contarPokemones(enemigo);

  int mainPkmVivos = pokemonesVivos(jugador);
  int enemigoPkmVivos = pokemonesVivos(enemigo);

  cout << " " << barraPokemones(enemigoPkmVivos, enemigoPkmCant - enemigoPkmVivos) << endl;
  cout << " " << padEnd(poke2->name, 25) << "Nv" << poke2->level << endl;
  cout << padStart("PS" + PSBar(poke2->live, poke2->maxLive) + " " + to_string(poke2->live) + "/" + to_string(poke2->maxLive), 30) << endl;

  cout << endl
       << endl
       << endl
       << endl
       << endl
       << endl
       << endl;

  cout << padEnd("", 34) << barraPokemones(mainPkmVivos, mainPkmCant - mainPkmVivos) << endl;
  cout << padStart(padEnd(poke1->name, 25) + "Nv" + to_string(poke1->level), 63) << endl;
  cout << padStart("PS" + PSBar(poke1->live, poke1->maxLive) + " " + to_string(poke1->live) + "/" + to_string(poke1->maxLive), 63) << endl;
}

SPokemon *siguienteVivo(SPokemon *actual)
{
  SPokemon *retorno = actual;

  while (retorno->live <= 0)
  {
    retorno = retorno->next;
    if (retorno->name == actual->name)
      break;
  }

  return retorno;
}

void realizarAtaque(SPokemon **poke1, SPokemon **poke2)
{
  SPokemon **primero = (*poke1)->speed > (*poke2)->speed ? poke1 : poke2;
  SPokemon **segundo = (*poke1)->speed > (*poke2)->speed ? poke2 : poke1;

  (*segundo)->live -= (*primero)->attack;
  if ((*segundo)->live <= 0)
    (*segundo)->live = 0;

  clearConsole();
  renderBattle(*poke1, *poke2);
  renderMessage((string)(*primero)->name + " ha realizado un ataque.");
  pause();

  if ((*segundo)->live > 0)
  {
    (*primero)->live -= (*segundo)->attack;
    if ((*primero)->live <= 0)
      (*primero)->live = 0;

    clearConsole();
    renderBattle(*poke1, *poke2);
    renderMessage((string)(*segundo)->name + " ha realizado un ataque.");
    pause();
  }
  else
  {
    clearConsole();
    renderBattle(*poke1, *poke2);
    renderMessage((string)(*segundo)->name + " se ha debilitado.");
    pause();

    if (*segundo == siguienteVivo(*segundo))
      return;
    (*segundo) = siguienteVivo(*segundo);
    clearConsole();
    renderBattle(*poke1, *poke2);

    if ((*segundo)->mainPlayer)
    {
      renderMessage("¡Adelante, " + (string)(*segundo)->name + "!");
    }
    else
      renderMessage("¡El campeón " + (string)enemigo->name + " ha enviado a " + (string)(*segundo)->name + "!");

    pause();
  }

  if ((*primero)->live <= 0)
  {
    clearConsole();
    renderBattle(*poke1, *poke2);
    renderMessage((string)(*primero)->name + " se ha debilitado.");
    pause();

    if (*primero == siguienteVivo(*primero))
      return;
    (*primero) = siguienteVivo(*primero);
    clearConsole();
    renderBattle(*poke1, *poke2);

    if ((*primero)->mainPlayer)
    {
      renderMessage("¡Adelante, " + (string)(*primero)->name + "!");
    }
    else
      renderMessage("¡El campeón " + (string)enemigo->name + " ha enviado a " + (string)(*primero)->name + "!");

    pause();
  }
}

void batallaPokemon()
{
  SPokemon *poke1 = jugador->team;
  SPokemon *poke2 = enemigo->team;

  if (
      pokemonesVivos(jugador) == 0 || pokemonesVivos(enemigo) == 0)
    return; // Evitar problemas

  clearConsole();
  renderMessage("Bienvenido. Me presentaré: soy " + (string)enemigo->name + ", el Campeon.");
  pause();
  renderMessage("Entonces, ¿tú eres de quien tanto hablaba el profesor Acha?");
  pause();
  renderMessage("Venga, disfrutemos de un intenso combate.");
  pause();

  clearConsole();
  renderBattle(poke1, poke2);
  renderMessage("¡Adelante, " + (string)poke1->name + "!");
  pause();

  clearConsole();
  renderBattle(poke1, poke2);
  renderMessage("¡El campeon " + (string)enemigo->name + " saca a " + poke2->name + "!");
  pause();

  while (true)
  {
    if (
        pokemonesVivos(jugador) == 0 || pokemonesVivos(enemigo) == 0)
    {
      clearConsole();
      renderBattle(poke1, poke2);
      if (pokemonesVivos(jugador) == 0)
      {
        renderMessage("Tus pokemones han dado todo de si. Pero haz perdido en contra de mi, " + (string)enemigo->name + " el campeón.");
        renderMessage("Has perdido.");
      }
      else
        renderMessage("¡El campeón " + (string)enemigo->name + " ha perdido!");
      return;
    }

    clearConsole();
    renderBattle(poke1, poke2);
    renderMessage("¿Que deberia hacer " + (string)poke1->name + "?");
    cout << endl;

    string *opcionesLucha = new string[3];
    *(opcionesLucha) = "Jugar Turno";
    *(opcionesLucha + 1) = "Cambiar pokemon";
    *(opcionesLucha + 2) = "Volver al menu principal";

    int opt = crearMenuApuntadores(3, opcionesLucha);

    cin.ignore();

    switch (opt)
    {
    case 1:
      realizarAtaque(&poke1, &poke2);
      break;
    case 2:
      cambiarPokemon(&poke1, &poke2);
      break;
    case 3:
      delete[] opcionesLucha;
      return;

    default:
      break;
    }
  }
}

void cambiarPokemon(SPokemon **poke1, SPokemon **poke2)
{
  SPokemon *siguiente = (*poke1)->next;
  SPokemon *anterior = (*poke1)->previous;

  // renderMessage("Quieres cambiar a (1) " + (string)siguiente->name + " o quieres enviar a (2) " + (string)anterior->name);

  clearConsole();
  renderBattle(*poke1, *poke2);

  string *opciones = new string[3];
  *(opciones) = "Cambiar a (1) " + (string)siguiente->name;
  *(opciones + 1) = "Cambiar a (2) " + (string)anterior->name;
  *(opciones + 2) = "Cancelar";

  int opt = crearMenuApuntadores(3, opciones);
  cin.ignore();

  clearConsole();
  renderBattle(*poke1, *poke2);
  switch (opt)
  {
  case 1:
    renderMessage("Ok, " + (string)(*poke1)->name + " diste una buela pelea, adelante " + (string)siguiente->name);
    *poke1 = siguiente;
    break;
  case 2:
    renderMessage("Ok, " + (string)(*poke1)->name + " diste una buela pelea, adelante " + (string)anterior->name);
    *poke1 = anterior;
    break;
  default:
    delete[] opciones;
    return;
  }
  pause();
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
      batallaPokemon();
      break;
    case 4:
      guardarPartida();
      delete[] opciones;
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

  int strSize = getSizeWithoutAccents(str);

  for (int i = 0; i < size - strSize; i++)
  {
    str = str + " ";
  }

  return str;
}

string padStart(string str, int size)
{
  // Agrega espacios al inicio

  int strSize = getSizeWithoutAccents(str);

  for (int i = 0; i < size - strSize; i++)
  {
    str = " " + str;
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