#ifndef CALC_UNBOUNDED_H
#define CALC_UNBOUNDED_H

// Bibliothêques utilisées :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "unbounded_int.h"

// Structure qui stocke les différents noms de variables qu'on a initialisées :
typedef struct string
{
    char c;
    struct string *next;
} string;

// Structure qui stocke les noms des variable avec leurs valeurs :
typedef struct key_value
{
    string *key;
    unbounded_int value;
    struct key_value *next;
} key_value;

#endif