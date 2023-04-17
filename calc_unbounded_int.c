#include "calc_unbounded_int.h"

// Déclaration des fonctions auxilliaires statiques :

// static void affiche_kv(key_value*);
static void programme(FILE *, FILE *);
static void erreur(FILE *, FILE *, char *, key_value *);
static void skip_sep(FILE *, char);
static void print(FILE *, FILE *, key_value *);
static void binaire(FILE *src, FILE *dst, key_value *kv);
static void decimale(FILE *src, FILE *dst, key_value *kv);
static void absolue(FILE *src, FILE *dst, key_value *kv);
static void liberer_string();
static void liberer_kv();
static int contient(char, char *);
static int is_int(FILE *);
static int exist_key(key_value *, string *);
static int string_cmp_string(string *, string *);
static int string_cmp_char(string *s, char *c);
static int is_space_new_ligne(char c);
static char is_op(char);
static char *stringtochar(string *);
static unbounded_int operation(char, unbounded_int, unbounded_int);
static unbounded_int get_value(key_value *, string *);
static unbounded_int read_value(FILE *, key_value *);
static key_value *create_kv(string *, unbounded_int);
static key_value *add_kv(key_value *, string *, unbounded_int);
static string *add_string(char, string *);
static string *next_word(FILE *, char *);

// Fonctions principales  :
int main(int argc, char *argv[])
{
    FILE *src;
    FILE *dst;
    switch (argc)
    {
    case 5: // on écrit et on lit dans des fichiers
        src = fopen(argv[2], "r");
        dst = fopen(argv[4], "w");
        break;
    case 3: // on lit ou on écrit dans un fichier, l'autre est le terminal
        if (strcmp(argv[1], "-i") == 0)
        {
            src = fopen(argv[2], "r");
            dst = stdout;
        }
        if (strcmp(argv[1], "-o") == 0)
        {
            src = stdin;
            dst = fopen(argv[2], "w");
        }
        break;
    case 1: // on écrit et on lit dans le terminal
        src = stdin;
        dst = stdout;
        break;
    default:
        printf("Il n'y a pas le bon nombre d'arguments");
        break;
    }
    if (src != NULL && dst != NULL)
    {
        programme(src, dst);
        fclose(src);
        fclose(dst);
    }
    return 0;
}

// Effectue toutes les opérations de lecture et d'écriture :
static void programme(FILE *src, FILE *dst)
{
    key_value *kv = NULL; // crée une liste vide
    while (!feof(src))    // tant qu'on n'est pas à la fin du fichier
    {
        string *first = next_word(src, " ="); // recupère le nom de la variable
        // si on print
        if (string_cmp_char(first, "print"))
            print(src, dst, kv);
        else
        {
            if (string_cmp_char(first, "binaire"))
                binaire(src, dst, kv);
            else
            {
                if (string_cmp_char(first, "decimale"))
                    decimale(src, dst, kv);
                else
                {
                    if (string_cmp_char(first, "abs"))
                        absolue(src, dst, kv);
                    else
                    {
                        skip_sep(src, ' ');
                        // si c'est un = => affectation de variable ou calcule
                        if (fgetc(src) == '=')
                        {
                            unbounded_int v = read_value(src, kv); // recupère la valeur
                            skip_sep(src, ' ');
                            int i = fgetc(src);
                            // fin de ligne ou fin de fichier => affectation de variable
                            if (i == '\n' || i == EOF)
                                kv = add_kv(kv, first, v); // ajoute à la liste la variable
                            else
                            { // On a une opération
                                int op = is_op(i);
                                if (op != 0)
                                {
                                    unbounded_int v2 = read_value(src, kv);     // recupère la deuxième valeur
                                    unbounded_int final = operation(op, v, v2); // calcule l'opèration
                                    kv = add_kv(kv, first, final);              // ajoute à la liste le resultat final
                                }
                                else
                                    erreur(src, dst, "Erreur : pas le bon format de fichier", kv);
                            }
                        }
                        else
                            erreur(src, dst, "Erreur : pas le bon format de fichier", kv);
                    }
                }
            }
        }
        int i = fgetc(src); // lecture du caractère
        while (is_space_new_ligne(i))
        { // Passe tous les espaces et sauts de lignes
            skip_sep(src, ' ');
            skip_sep(src, '\n');
            i = fgetc(src);
        }
        ungetc(i, src);
    }
    liberer_kv(kv);
}

// Fonctions auxilliaires :

// Renvoie une erreur :
static void erreur(FILE *src, FILE *dst, char *str, key_value *kv)
{
    // Libérer la mémoire :
    liberer_kv(kv);
    // Fermeture des fichiers :
    if (src != stdin)
        fclose(src);
    if (dst != stdout)
        fclose(dst);
    // Message d'erreur :
    fprintf(stderr, "%s", str);
    // Fermeture du programme :
    exit(EXIT_FAILURE);
}

// vérifie si la chaine str contient le caractère c :
static int contient(char c, char *str)
{
    while (*str != '\0')
    {
        if (*str == c)
            return 1;
        str++;
    }
    return 0;
}

// renvoie l'opération si il y en a une :
static char is_op(char i)
{
    if (i == '+' || i == '-' || i == '*' || i == '/' || i == '%')
        return i;
    return 0;
}

// Prend l'opération en paramètre et effectue l'opération ( donnée par le char c )
static unbounded_int operation(char c, unbounded_int a, unbounded_int b)
{
    switch (c)
    {
    case '+':
        return unbounded_int_somme(a, b);
    case '-':
        return unbounded_int_difference(a, b);
    case '*':
        return unbounded_int_produit(a, b);
    case '/':
        return unbounded_int_quotient(a, b);
    case '%':
        return unbounded_int_modulo(a, b);
    default:
        printf("ce n'est pas une operation que nous pouvons traiter. \n");
        return init_unbounded();
    }
}

// Fonctions de lecture :

// avance dans la lecture du fichier jusqu'au prochain caractère différent de sep
static void skip_sep(FILE *fd, char sep)
{
    int i = 0;
    while ((i = fgetc(fd)) == sep)
        continue;
    ungetc(i, fd);
}

// renvoie 1 si la prochaine chose qu'on lit est un int :
static int is_int(FILE *src)
{
    skip_sep(src, ' ');
    int i = fgetc(src);
    ungetc(i, src);
    if (i == '+' || i == '-' || isDigit(i))
        return 1;
    return 0;
}

// renvoie 1 si la chaine est un espace ou un retour à la ligne
static int is_space_new_ligne(char c)
{
    return c == ' ' || c == '\n';
}

// renvoie la chaine de caractère lue avant le sep
static string *next_word(FILE *src, char *sep)
{
    int i = fgetc(src); // lecture du caractère
    while (is_space_new_ligne(i))
    { // Passe tous les espaces et sauts de lignes
        skip_sep(src, ' ');
        skip_sep(src, '\n');
        i = fgetc(src);
    }
    ungetc(i, src);
    string *name = NULL;

    while ((contient((i = fgetc(src)), sep)) == 0 && i != EOF)
    { // calcule la taille du mot
        name = add_string(i, name);
    }
    add_string('\0', name);
    ungetc(i, src); // renvoie le séparateur dans le flux
    return name;
}

// Recupère la valeur dans le fichier
static unbounded_int read_value(FILE *src, key_value *kv)
{
    skip_sep(src, ' ');
    unbounded_int v = init_unbounded(); // initalise l'unbounded_int
    if (is_int(src))
    {
        string *value = next_word(src, "\n ");
        char *val = stringtochar(value);
        v = string2unbounded_int(val);
        free(val);
        liberer_string(value);
    }
    else
    {
        string *name = next_word(src, "\n ");
        if (exist_key(kv, name))
            v = get_value(kv, name);
        else
        {
            chiffre *zero = malloc(sizeof(chiffre));
            zero->c = '0';
            zero->precedent = NULL;
            zero->suivant = NULL;
            v.signe = '+';
            v.len = 1;
            v.premier = zero;
            v.dernier = zero;
        }
        liberer_string(name);
    }
    return v;
}

// Fonctions d'écriture :

static void print(FILE *src, FILE *dst, key_value *kv)
{
    skip_sep(src, ' ');
    string *name = next_word(src, "\n ");
    if (fputs(stringtochar(name), dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (fputs(" = ", dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (!exist_key(kv, name))
        fputs("0", dst);
    else
    {
        unbounded_int value = get_value(kv, name);
        if (fputs(unbounded_int2string(value), dst) == EOF)
            erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    }
    if (fputc('\n', dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    skip_sep(src, ' ');
    skip_sep(src, '\n');
    liberer_string(name);
}

static void binaire(FILE *src, FILE *dst, key_value *kv)
{
    skip_sep(src, ' ');
    string *name = next_word(src, "\n ");
    if (fputs(stringtochar(name), dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (fputs(" (en binaire) = ", dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (!exist_key(kv, name))
        fputs("0", dst);
    else
    {
        unbounded_int value = get_value(kv, name);
        if (fputs(unbounded_int2string(TenToTwo(value)), dst) == EOF)
            erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    }
    if (fputc('\n', dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    skip_sep(src, ' ');
    skip_sep(src, '\n');
    liberer_string(name);
}

static void decimale(FILE *src, FILE *dst, key_value *kv)
{
    skip_sep(src, ' ');
    string *name = next_word(src, "\n ");
    if (fputs(stringtochar(name), dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (fputs(" (en decimale) = ", dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (!exist_key(kv, name))
        fputs("0", dst);
    else
    {
        unbounded_int value = get_value(kv, name);
        if (fputs(unbounded_int2string(TwoToTen(value)), dst) == EOF)
            erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    }
    if (fputc('\n', dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    skip_sep(src, ' ');
    skip_sep(src, '\n');
    liberer_string(name);
}

static void absolue(FILE *src, FILE *dst, key_value *kv)
{
    skip_sep(src, ' ');
    string *name = next_word(src, "\n ");
    if (fputs("La valeur absolue de ", dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (fputs(stringtochar(name), dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (fputs(" = ", dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    if (!exist_key(kv, name))
        fputs("0", dst);
    else
    {
        unbounded_int value = get_value(kv, name);
        if (fputs(unbounded_int2string(u_abs(value)), dst) == EOF)
            erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    }
    if (fputc('\n', dst) == EOF)
        erreur(src, dst, "Erreur: Ecriture dans le fichier de destination", kv);
    skip_sep(src, ' ');
    skip_sep(src, '\n');
    liberer_string(name);
}

// Fonctions qui gère la liste d'élements :

// Crée une liste vide
static key_value *create_kv(string *key, unbounded_int value)
{
    key_value *kv = malloc(sizeof(key_value));
    kv->key = key;
    kv->value = value;
    kv->next = NULL;

    return kv;
}

// Ajout des clés à la liste
static key_value *add_kv(key_value *kv, string *key, unbounded_int value)
{
    key_value *first = kv;
    if (kv == NULL)
        return create_kv(key, value);
    if (string_cmp_string(kv->key, key))
        kv->value = value;
    else
        kv->next = add_kv(kv->next, key, value);
    return first;
}

// Teste si la clé k existe dans la liste
static int exist_key(key_value *kv, string *key)
{
    if (kv == NULL)
        return 0;
    if (string_cmp_string(kv->key, key))
        return 1;
    return exist_key(kv->next, key);
}

/* //Affiche la liste
static void affiche_kv(key_value *kv)
{
    while (kv != NULL)
    {
        printf("key : %s  value : ", stringtochar(kv->key));
        affiche(kv->value);
        printf("\n");
        kv = kv->next;
    }
}*/

// renvoie la valeur de k si k existe dans la liste kv
static unbounded_int get_value(key_value *kv, string *key)
{
    while (kv != NULL)
    {
        if (string_cmp_string((kv->key), (key)))
            return kv->value;
        kv = kv->next;
    }
    return init_unbounded();
}

// Ajoute un caractere à un string
static string *add_string(char c, string *s)
{
    string *res = s;
    if (s == NULL)
    {
        res = malloc(sizeof(string));
        res->c = c;
        res->next = NULL;
        return res;
    }
    res->next = add_string(c, res->next);
    return s;
}

// renvoie 1 si  deux string sont égaux, 0 sinon
static int string_cmp_string(string *s, string *st)
{
    while (s != NULL && st != NULL)
    {
        if (s->c != st->c)
        {
            return 0;
        }
        s = s->next;
        st = st->next;
    }
    if (s == NULL && st == NULL)
        return 1;
    return 0;
}

// Convertie un string en char*
static char *stringtochar(string *s)
{
    string *tmp = s;
    int i = 0;
    while (tmp != NULL)
    {
        i++;
        tmp = tmp->next;
    }
    char *res = malloc(i);
    tmp = s;
    i = 0;
    while (tmp != NULL)
    {
        res[i] = tmp->c;
        tmp = tmp->next;
        i++;
    }
    return res;
}

// Compare un string et un char *
static int string_cmp_char(string *s, char *c)
{
    while (s != NULL && *c != '\0')
    {
        if (s->c != *c)
        {
            return 0;
        }
        s = s->next;
        c++;
    }
    if (s->c == '\0' && *c == '\0')
        return 1;
    return 0;
}

// Fonctions de libération de mémoire :

// Libère la memoire du nom d'une variable :
static void liberer_string(string *s)
{
    while (s != NULL)
    {
        string *tmp = s->next;
        free(s);
        s = tmp;
    }
}

// Libère la memoire de la liste kb :
static void liberer_kv(key_value *kv)
{
    while (kv != NULL)
    {
        key_value *tmp = kv->next;
        liberer_string(kv->key);
        liberer(kv->value);
        free(kv);
        kv = tmp;
    }
}