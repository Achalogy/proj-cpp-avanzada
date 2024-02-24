#pragma once

#include <bits/stdc++.h>

using namespace std;

int crearMenuApuntadores(int cantOpciones, string *opciones)
{
  // Retorna el entero proporcionado por el usuario que identifica la opción elegida.

  int opcionInput = -1;

  cout << "===========================================" << endl;
  do
  {
    for (int i = 0; i < cantOpciones; i++)
      cout << i + 1 << ". " << *(opciones + i) << endl;
    cout << "===========================================" << endl;
    cout << "> ";

    cin >> opcionInput;
    cout << endl;
    // Volver a enviar el menu en caso de no recibir una opción valida34
  } while (opcionInput <= 0 || opcionInput > cantOpciones);

  return opcionInput;
}