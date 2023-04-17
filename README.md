# LANGUAGE C 2022

## Sommaire

1. [Introduction et informations](README.md#introduction-et-informations)
2. [Compilation et Exection du projet](README.md#compilation-et-Execution-du-projet)
3. [Fonctionalite](README.md#fonctionalites)

## Introduction et informations

**Informations généraux**

- Le sujet du projet : [projet.pdf](projet.pdf)
- Numero du groupe : **114**

**Identifiants et membres du groupe**

1. Asma, MOKEDDES, 21967552, MI1

2. Tara, AGGOUN, 21961069, MI1

## Compilation et Execution du projet

**Compilation**

Pour compiler le projet, il suffit d'entrer la commande suivante :

```
make
```

Pour supprimer les fichiers .o et les exécutables :

```
make clean
```

**Execution**

Pour exécuter les tests des fonctions de unbounded_int.c :

```
./test_unbounded
```

Pour exécuter l'interpreteur :

```
./calc_unbounded_int [args]
```

**Les argument**

calc_unbounded_int peut prendre au maximum deux arguments :

1. -i [*nom du fichier*] :

   - Le fichier doit obligatoirement exister.
   - si l'argument n'est pas passer, il sera remplacer par l'entrée standard (stdin) automatiquement.
     - Pour arreter de lire dans l'entrée standard il faut faire:
     ```
     Ctrl D
     ```
   - Dans le repertoire [Tests](Tests/) vous trouverez quelques fichiers de tests ( différents scénarions que nous avons imaginé pour tester la deuxième partie du projet).

1. -o [*nom du fichier*] :
   - Le fichier peut ne pas exister, il sera créé automatiquement, sinon il sera ecrasé par la sortie du programme.
   - si l'argument n'est pas passé, il sera remplacé par la sortie standard (stdout) automatiquement.

## Fonctionnalités :

**Quelques règles que doit respecter le fichier source**

1. Chaque ligne contient au plus une instruction.

2. Il est possible d’avoir des lignes vides (sans instruction).

3. Il n’y a pas de parenthèses.

4. Il y a des espaces (au moins un de chaque côté) autour de chaque opération.

5. Les entiers positifs peuvent être écrits soit comme 23432 soit comme +23432 (avec + au début suivi immédiatement d’un chiffre).

6. Les nombres négatifs s’écrivent comme -657867654 (avec le signe - suivi immédiatement d’un chiffre).

7. Des espaces à gauche ou à droite du = sont autorisés mais pas obligatoires.

8. On peut avoir zéro ou plusieurs espaces au début de chaque ligne.

**Les instructions**

Il y a 6 types d'instructions possibles :

1. L'affectation : _nom de variable_ = _valeur_

2. L'opération : _nom de variable_ = _valeur_ _operateur_ _valeur_

3. Affichage: print _nom de variable_

4. La convertion en binaire : binaire _nom de variable_

5. La convertion de binaire en décimale : decimale _nom de variable_

6. Valeur absolue d'un chiffre : abs _nom de variable_

- Il n'y a aucune condition sur le _nom de variable_

  - si il est appellé alors qu'il n'a jamais été initailisé, il vaudra 0.

- _valeur_ est soit un nombre, soit un _nom de variable_

- _operateur_ peut être soit :

  1. '+' , l'opération sera alors l'addition.
  2. '-' , l'opération sera alors la soustraction.
  3. '\*' , l'opération sera alors le produit.
  4. '/' , l'opération sera alors la division.
  5. '%' , l'opeéation sera alors le modulo.
