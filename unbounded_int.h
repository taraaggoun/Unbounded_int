#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

// Bibliothèques utilisées :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures :
typedef struct chiffre
{
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct
{
    char signe;       /* soit ’+’ soit ’-’ */
    size_t len;       /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/
} unbounded_int;

// Fonctions principales:

// prend en argument l’adresse d’une chaîne de caractères représentant un entier et renvoyant une valeur unbounded_int représentant cet entier.
// si la chaîne pointée par e ne représente pas un entier la fonction retournera un unbounded_int avec le champ signe == ’*’.
unbounded_int string2unbounded_int(const char *);

// prend en argument un nombre long long et retourne la structure unbounded_int représentant ce nombre.
unbounded_int ll2unbounded_int(long long ll);

// prend en argument une structure unbounded_int représentant un entier et retourne cet entier sous forme d’une chaîne de caractères.
char *unbounded_int2string(unbounded_int);

// retourne une des valeurs −1, 0, 1 si, respectivement, a < b, a == b, a > b.
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

// retourne une des valeurs −1, 0, 1 si, respectivement, a < b, a == b, a > b avec b un long long.
int unbounded_int_cmp_ll(unbounded_int a, long long b);

// retourne la représentation de la somme de deux entiers représentés par a et b.
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

// retourne la représentation de leur différence.
unbounded_int unbounded_int_difference(unbounded_int, unbounded_int);

// retourne la représentation de leur produit.
unbounded_int unbounded_int_produit(unbounded_int, unbounded_int);

// retourne la representation leur quotient.
unbounded_int unbounded_int_quotient(unbounded_int, unbounded_int);

// retourne la representation leur modulo.
unbounded_int unbounded_int_modulo(unbounded_int, unbounded_int);

// convertie un unbounded_int representé en 'base 2' en 'base 10'.
unbounded_int TwoToTen(unbounded_int a);

// convertie un unbounded_int representé en 'base 10' en 'base 2'.
unbounded_int TenToTwo(unbounded_int a);

// retourne la valeur absolue d'un unbounded_int.
unbounded_int u_abs(unbounded_int);

// Fonctions auxiliaires:

// affiche un unbounded_int :
void affiche(unbounded_int);

// libère l'unbounded_int :
void liberer(unbounded_int);

// retourne un unbounded_int qui a pour signe '*'
unbounded_int init_unbounded();

// retourne 1 si le caractère e est un chiffre, 0 sinon.
int isDigit(char e);

#endif