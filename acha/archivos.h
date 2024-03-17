#pragma once

#include <bits/stdc++.h>
#include <fstream>
#include "utils.h"

using namespace std;

void printFileContent(string path)
{
  ifstream file;
  string line;

  file.open(path);

  while (!file.eof())
  {
    getline(file, line);
    cout << line << endl;
  }

  file.close();
}

void readFile(string path)
{
  ifstream file;

  cout << "Intentando leer " << path << endl;

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede leer el archivo" << endl;
    return;
  }
  cout << "Contenido del archivo " << path << ": " << endl
       << endl
       << endl;

  cout << "############################ Inicio del archivo ###########################" << endl;

  printFileContent(path);

  cout << "############################# Fin del archivo #############################" << endl
       << endl;

  file.close();
  enviarParaContinuar();
}

void writeFile(string path)
{
  ofstream file;
  string line;
  bool firstLine = true;

  if (!preguntaContinuar("¿Desea sobreescribir el archivo " + path + "?"))
  {
    return;
  }

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede escribir el archivo" << endl;
    return;
  }

  clearConsole();

  cout
      << "Escribiendo " << path << endl;
  cout << "Escribe !q para finalizar" << endl
       << endl
       << endl;

  cin.ignore();

  while (true)
  {
    getline(cin, line);
    if (line == "!q")
      break;
    file << (firstLine ? "" : "\n")
         << line;
    firstLine = false;
  }

  clearConsole();

  file.close();
}

void appendContent(string path)
{
  ofstream file;
  string line;
  bool firstLine = true;

  file.open(path, ios::app);

  if (!file.is_open())
  {
    cout << "No se puede escribir el archivo" << endl;
    return;
  }

  clearConsole();

  cout
      << "Escribiendo " << path << endl;
  cout << "Escribe !q para finalizar" << endl
       << endl
       << endl;

  printFileContent(path);

  cin.ignore();

  while (true)
  {
    getline(cin, line);
    if (line == "!q")
      break;
    file << (firstLine ? "" : "\n")
         << line;
    firstLine = false;
  }

  clearConsole();

  file.close();
}

int numberOfLines(string path)
{
  ifstream file;
  int cantLineas = 0;
  string line;

  cout << "Intentando leer " << path << endl;

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede leer el archivo" << endl;
    return -1;
  }

  while (!file.eof())
  {
    getline(file, line);
    cantLineas++;
  }

  // file.seekg(0, ios::end);
  // cout << "Cantidad de lineas: " << cantLineas << endl;

  file.close();

  return cantLineas;
}

int fileSize(string path)
{
  ifstream file;
  string line;

  cout << "Intentando leer " << path << endl;

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede leer el archivo" << endl;
    return -1;
  }

  file.seekg(0, ios::end);
  cout << "Tamaño del archivo: " << file.tellg() << endl;

  file.close();

  return file.tellg();
}

void readWord(string path)
{
  ifstream file;
  int i = 0;
  string word;

  cout << "Intentando leer " << path << endl;

  file.open(path);

  if (!file.is_open())
  {
    cout << "No se puede leer el archivo" << endl;
  }

  cout << endl;

  while (!file.eof())
  {
    file >> word;
    cout << i << ". " << word << endl;
    i++;
  }

  cout << endl;

  enviarParaContinuar();

  file.close();
}