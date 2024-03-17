#pragma once

#include <bits/stdc++.h>

using namespace std;

bool preguntaContinuar(string msg = "¿Desea continuar?")
{
  // Se encarga de preguntar al usuario si continuar y retorna esta decision en forma de bool

  char eleccion;

  do
  {
    cout << endl
         << msg << " (Y/n) ";
    cin >> eleccion;

    // El ciclo se ejecutará hasta que se envie un char aceptable
  } while (eleccion != 'y' && eleccion != 'Y' && eleccion != 'n' && eleccion != 'N');

  return eleccion == 'y' || eleccion == 'Y';
}

void enviarParaContinuar()
{
  string _;
  cout << "Envia cualquier cosa para continuar... ";
  cin >> _;
}

int crearMenu(int cantOpciones, string opciones[])
{
  // Retorna el entero proporcionado por el usuario que identifica la opción elegida.

  int opcionInput = -1;

  cout << "===========================================" << endl;
  do
  {
    for (int i = 0; i < cantOpciones; i++)
      cout << i + 1 << ". " << opciones[i] << endl;
    cout << "===========================================" << endl;
    cout << "> ";

    cin >> opcionInput;
    cout << endl;
    // Volver a enviar el menu en caso de no recibir una opción valida
  } while (opcionInput <= 0 || opcionInput > cantOpciones);

  return opcionInput;
}

void clearConsole()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}