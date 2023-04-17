#include "unbounded_int.h"

// Déclaration des fonctions auxilliaires statiques :

static int StringIsInt(const char *);
static int is_empty(unbounded_int);
static int lenll(long long);
static int cmp_val(unbounded_int, unbounded_int);
static int cmp_val_ll(unbounded_int, long long, int);
static long long puissance(int, int);
static int char2int(char);
static char int2char(int);
static char getchiffre(unbounded_int, int);
static unbounded_int add_chiffre(unbounded_int, char, int);
static unbounded_int addition(unbounded_int, unbounded_int);
static unbounded_int soustraction(unbounded_int, unbounded_int, int);
static unbounded_int setchiffre(unbounded_int, int, char);
static unbounded_int simplification(unbounded_int);
static unbounded_int division(unbounded_int, unbounded_int, int);

// Fonctions principales  :

// Fonction 1 :
unbounded_int string2unbounded_int(const char *c)
{
    unbounded_int res = init_unbounded();
    if (!StringIsInt(c)) // erreur si ce n'est pas un unbounded_int ou si malloc échoue
    {
        return init_unbounded();
    }
    res.signe = (*c == '-') ? '-' : '+'; // ajoute le signe à res
    c = isDigit(*c) ? c : (c + 1);       // regarde si il y a un chiffre en 1 ère position, sinon on l'enlève
    for (; *c; c++)                      // ajoute les chiffres à res
        res = add_chiffre(res, *c, 1);
    return res;
}

// Fonction 2 :
unbounded_int ll2unbounded_int(long long ll)
{
    unbounded_int res = init_unbounded();
    res.signe = ll < 0 ? '-' : '+'; // ajoute le signe à res
    if (ll < 0)
        ll *= -1; // on enlève le signe devant le longlong en le rendant positif
    while (ll >= 10 || ll <= -10)
    {
        char c = int2char(ll % 10);   // on recupère le dernier caractère
        res = add_chiffre(res, c, 0); // ajoute les chiffres en partant de la fin
        ll /= 10;
    }
    char c = int2char(ll);
    res = add_chiffre(res, c, 0);
    return res;
}

// Fonction 3 :
char *unbounded_int2string(unbounded_int i)
{
    char *res;
    if ((res = malloc(i.len + 2)) == NULL) // erreur si malloc échoue | i.len +2 pour le signe et le caractère de fin de chaine
    {
        fprintf(stderr, "Erreur d'allocation dynamique.\n");
        exit(EXIT_FAILURE);
    }
    res[0] = i.signe; // le 1er caractère représente le signe
    chiffre *element = i.premier;
    for (int j = 1; j <= i.len; j++) // ajoute à res les chiffres de i
    {
        res[j] = element->c;
        element = element->suivant;
    }
    res[i.len + 1] = '\0'; // ajout du caractère de fin de chaine à la fin
    return res;
}

// Fonction 4 :
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b)
{
    if (a.signe == '*' || b.signe == '*') // erreur si a ou b n'est pas un unbounded_int
    {
        fprintf(stderr, "Erreur, les arguments passés en paramètre de la fonction ne peuvent pas être comparés (pas des unbounded_int)\n");
        abort();
    }
    if (a.signe == '+' && b.signe == '-')
        return 1;
    if (a.signe == '-' && b.signe == '+')
        return -1;
    if (a.signe == '+') // si a et b sont positifs
    {
        if (a.len > b.len)
            return 1;
        if (b.len > a.len)
            return -1;
        return cmp_val(a, b);
    }
    // si a et b sont negatifs
    if (a.len > b.len)
        return -1;
    if (b.len > a.len)
        return 1;
    return cmp_val(a, b) * -1;
}

// Fonction 5 :
int unbounded_int_cmp_ll(unbounded_int a, long long b)
{
    if (a.signe == '*')
    {
        fprintf(stderr, "Erreur, les arguments passés en paramètre de la fonction ne peuvent pas être comparés (pas des unbounded_int)\n");
        abort();
    }
    if (a.signe == '+' && b < 0)
        return 1;
    if (a.signe == '-' && b >= 0)
        return -1;
    int lenb = lenll(b);
    if (a.signe == '+') // si a et b sont positifs
    {
        if (a.len > lenb)
            return 1;
        if (lenb > a.len)
            return -1;
        return cmp_val_ll(a, b, lenb);
    }
    // si a et b sont negatifs
    if (a.len > lenb)
        return -1;
    if (lenb > a.len)
        return 1;
    return cmp_val_ll(a, b, lenb) * -1;
}

// Fonction 6 :
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b)
{
    if (a.signe == '+' && b.signe == '+')
        return addition(a, b);
    if (a.signe == '-' && b.signe == '-')
    {
        unbounded_int res = addition(u_abs(a), u_abs(b));
        res.signe = '-';
        return res;
    }
    if (a.signe == '+' && b.signe == '-')
        return soustraction(a, u_abs(b),1);
    if (a.signe == '-' && b.signe == '+')
        return soustraction(b, u_abs(a),1);

    return init_unbounded();
}

// Fonction 7 :
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b)
{
    if (a.signe == '+' && b.signe == '+')
        return soustraction(a, b,1);
    if (a.signe == '-' && b.signe == '-')
        return soustraction(u_abs(b), u_abs(a),1);
    if (a.signe == '+' && b.signe == '-')
        return addition(a, u_abs(b));
    if (a.signe == '-' && b.signe == '+')
    {
        unbounded_int res = addition(b, u_abs(a));
        res.signe = '-';
        return res;
    }
    return init_unbounded();
}

// Fonction 8 :
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b)
{
    if (unbounded_int_cmp_ll(a, 0) == 0 || unbounded_int_cmp_ll(b, 0) == 0)
        return ll2unbounded_int(0);
    unbounded_int res = init_unbounded();
    res.signe = (a.signe != b.signe) ? '-' : '+'; // ajoute le signe à res
    for (int i = 0; i < a.len + b.len; i++)
    { // On initialise tous les chiffres c(i) de résultat c = c(m+n−1) ... c0 avec la valeur 0
        res = add_chiffre(res, '0', 0);
    }
    chiffre *cb = b.dernier;
    for (int j = 0; j < b.len; j++)
    { /* boucle sur les chiffres de b*/
        int r = 0;
        if (cb->c == '0')
        {
            cb = cb->precedent;
            continue;
        }
        chiffre *ca = a.dernier;
        for (int k = 0; k < a.len; k++)
        { /* boucle sur les chiffres de a*/
            int v = char2int(ca->c) * char2int(cb->c);
            v += char2int(getchiffre(res, j + k)) + r;
            r = v / 10;
            res = setchiffre(res, j + k, int2char(v % 10));
            ca = ca->precedent;
        }
        res = setchiffre(res, j + a.len, int2char(r));
        cb = cb->precedent;
    }
    res = simplification(res);
    return res;
}

// Fonction 9:
unbounded_int unbounded_int_quotient(unbounded_int a, unbounded_int b)
{
    unbounded_int res = division(u_abs(a), u_abs(b), 1);
    if (a.signe != b.signe)
        res.signe = '-';
    return res;
}

// Fonction 10:
unbounded_int unbounded_int_modulo(unbounded_int a, unbounded_int b)
{
    if (unbounded_int_cmp_ll(a, 0) == -1 || unbounded_int_cmp_ll(b, 0) == -1)
    {
        fprintf(stderr, "Erreur: le modulo est défini que sur les entiers naturels");
        exit(EXIT_FAILURE);
    }
    unbounded_int res = division(a, b, 0);
    if (a.signe != b.signe)
        res.signe = '-';
    return res;
}

// Fonction 11:
unbounded_int TenToTwo(unbounded_int a)
{
    unbounded_int res = init_unbounded();
    res.signe = ' ';
    long long ll = 1;
    while (unbounded_int_cmp_ll(a, ll) != -1)
    {
        ll *= 2;
    }
    ll/=2;
    unbounded_int ull = init_unbounded();
    unbounded_int supp = init_unbounded();
    while (ll != 0)
    {
        ull = ll2unbounded_int(ll);
        supp = soustraction(a, ull,0);
        if (unbounded_int_cmp_ll(supp, 0) == -1)
        {
            res = add_chiffre(res, '0', 1);
        }

        else
        {
            res = add_chiffre(res, 1 + '0', 1);
            a = u_abs(supp);
        }
        ll /= 2;
    }
    liberer(ull);
    liberer(supp);
    return res;
}

// Fonction 12:
unbounded_int TwoToTen(unbounded_int a)
{
    char tab[a.len];
    chiffre *c = a.dernier;
    for (int i = 0; i < a.len; i++)
    { // crée un tableau qui contient les valeurs de a
        tab[i] = c->c;
        c = c->precedent;
    }
    long long somme = 0;
    for (int i = 0; i < a.len; i++)
    {
        if (tab[i] == int2char(1))
        {
            if (i == 0)
                somme++;
            else
            {
                somme += puissance(2, i);
            }
        }
    }
    return ll2unbounded_int(somme);
}

// Fontion 13:
unbounded_int u_abs(unbounded_int a)
{
    unbounded_int res = a;
    res.signe = '+';
    return res;
}

// Fonctions auxilliaires :

// renvoie un unbounded_int avec le signe = '*' :
unbounded_int init_unbounded()
{
    return (unbounded_int){.signe = '*', .len = 0, .premier = NULL, .dernier = NULL};
}

// affiche un unbounded_int :
void affiche(unbounded_int u)
{
    if (u.signe == '*') // renvoie une erreur si u n'est pas un unbounded_int
    {
        fprintf(stderr, "Erreur ce n'est pas un unbounded_int.\n");
        exit(EXIT_FAILURE);
    }
    printf("> signe : %c \n", u.signe);
    printf("> len : %ld\n", u.len);
    chiffre *c = u.premier;
    while (c != NULL)
    {
        printf("%c", c->c);
        c = c->suivant;
    }
    printf("\n");
}

// libère l'unbounded_int :
void liberer(unbounded_int u)
{
    if (!is_empty(u))
    {
        while (u.premier != NULL)
        {
            chiffre *tmp = u.premier;
            u.premier = u.premier->suivant;
            free(tmp);
        }
    }
}

// vérifie que la chaine de caractère passée en paramètre représente bien un entier
// renvoie 0 si faux et 1 si vrai :
static int StringIsInt(const char *e)
{
    // Teste si le 1er caractère est un chiffre ou un signe ('+' ou  '-')
    if (!(*e == '+' || *e == '-' || isDigit(*e)))
        return 0;
    e++;
    while (*e != '\0') // vérifie qu'il y a que des int dans e
    {
        if (!isDigit(*e))
            return 0;
        e++;
    }
    return 1;
}

// vérifie que le caractère passé en argument représente un chiffre
// return 1 si vrai et 0 si faux :
int isDigit(char e)
{
    return e >= '0' && e <= '9'; // return 1 si e est un chiffre
}

// ajoute un chiffre à u :
// Si b est vrai à la fin
// Si b est faux au debut
static unbounded_int add_chiffre(unbounded_int u, char i, int b)
{
    chiffre *element;
    if ((element = malloc(sizeof(chiffre))) == NULL) // renvoie une erreur si malloc échoue
    {
        fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
        if (!is_empty(u))
        {
            while (u.premier != NULL) // libère la memoire des éléments qui étaient malloc avant
            {
                chiffre *tmp = u.premier->suivant;
                free(u.premier);
                u.premier = tmp;
            }
        }
        return init_unbounded();
    }
    element->c = i;
    element->suivant = NULL;
    element->precedent = NULL;
    if (is_empty(u)) // si vide ajoute l'élément en 1er ou dernier
    {
        u.premier = element;
        u.dernier = element;
    }
    else // sinon ajoute en dernière place
    {
        if (b)
        { // ajoute en derniere position
            u.dernier->suivant = element;
            element->precedent = u.dernier;
            u.dernier = element;
        }
        else
        { // ajoute en premiere position
            u.premier->precedent = element;
            element->suivant = u.premier;
            u.premier = element;
        }
    }
    u.len++;
    return u;
}

// vérifie si li est vide :
static int is_empty(unbounded_int li)
{
    if (li.premier == NULL)
        return 1;
    return 0;
}

// calcule le nombre de chiffres dans un long long
static int lenll(long long i)
{
    int len = 0;
    while (i > 10 || i < -10)
    {
        len++;
        i /= 10;
    }
    return len + 1;
}

// change le chiffre à la position id (en partant de la gauche) par c
static unbounded_int setchiffre(unbounded_int a, int id, char c)
{
    chiffre *ch = a.dernier;
    for (int i = 0; i < id; i++)
        ch = ch->precedent;
    ch->c = c;
    return a;
}

// retourne le chiffre à la position id (en partant de la gauche)
static char getchiffre(unbounded_int a, int id)
{
    chiffre *ch = a.dernier;
    for (int i = 0; i < id; i++)
        ch = ch->precedent;
    return ch->c;
}

// convertie un char en int
static int char2int(char c)
{
    return c - '0';
}

// convertie un int en char
static char int2char(int i)
{
    return i + '0';
}

// compare deux unbounded_int positifs de meme taille
static int cmp_val(unbounded_int a, unbounded_int b)
{
    chiffre *ca = a.premier;
    chiffre *cb = b.premier;
    while (ca != NULL)
    {
        if (ca->c > cb->c)
            return 1;
        if (ca->c < cb->c)
            return -1;
        ca = ca->suivant;
        cb = cb->suivant;
    }
    return 0;
}

// compare un unbounded_int et un long long positif de meme taille
static int cmp_val_ll(unbounded_int a, long long b, int lenb)
{
    chiffre *ca = a.premier;
    long long l = b;
    char tab[lenb];
    for (int k = 0; k < lenb; k++)
    {
        tab[lenb - k - 1] = l % 10 + '0';
        l /= 10;
    }
    int i = 0;
    while (ca != NULL)
    {
        if (ca->c > tab[i])
            return 1;
        if (ca->c < tab[i])
            return -1;
        ca = ca->suivant;
        i++;
    }
    return 0;
}

// calcule a puissance n
static long long puissance(int a, int n)
{
    if (a == 0)
        return 0;
    long long res = 1;
    for (int i = 0; i < n; i++)
        res *= a;
    return res;
}

// simplifie un chiffre
static unbounded_int simplification(unbounded_int a)
{
    if(!is_empty(a)){
        while(a.premier != NULL && a.premier->c == '0'){
            chiffre *tmp = a.premier;
            a.premier = a.premier->suivant;
            free(tmp);
            a.len --;
        }
        if (a.premier == NULL)
        {
            liberer(a);
            return ll2unbounded_int(0);
        }
    }
    return a;
}

// Fonctions de calculs:
// additionne deux unbounded_int positifs
static unbounded_int addition(unbounded_int a, unbounded_int b)
{
    unbounded_int c = init_unbounded();
    c.signe = '+'; // le résultat est positif par défaut
    int retenue = 0;
    chiffre *ca = a.dernier;
    chiffre *cb = b.dernier;

    while (ca != NULL && cb != NULL)
    {
        int tmp = char2int(ca->c) + char2int(cb->c) + retenue;
        retenue = tmp / 10;
        c = add_chiffre(c, int2char(tmp % 10), 0);
        ca = ca->precedent;
        cb = cb->precedent;
    }
    while (ca != NULL)
    {
        if (retenue != 0)
        {
            int tmp = char2int(ca->c) + retenue;
            retenue = tmp / 10;
            c = add_chiffre(c, int2char(tmp % 10), 0);
        }
        else
            c = add_chiffre(c, ca->c, 0);
        ca = ca->precedent;
    }
    while (cb != NULL)
    {
        if (retenue != 0)
        {
            int tmp = char2int(cb->c) + retenue;
            retenue = tmp / 10;
            c = add_chiffre(c, int2char(tmp % 10), 0);
        }
        else
            c = add_chiffre(c, cb->c, 0);
        cb = cb->precedent;
    }
    if (retenue != 0 && ca == NULL && ca == NULL)
        c = add_chiffre(c, retenue + '0', 0);
    c = simplification(c);
    return c;
}

// soustrait deux unbounded_int a >= b
static unbounded_int soustraction(unbounded_int a, unbounded_int b, int boo)
{
    int cmp = unbounded_int_cmp_unbounded_int(a, b);
    if (cmp == 0)
    {
        return ll2unbounded_int(0);
    }
    if (cmp == -1)
    {
        unbounded_int res = soustraction(b, a,boo);
        res.signe = (res.signe = '+') ? '-' : '+';
        return res;
    }
    unbounded_int c = init_unbounded();
    c.signe = '+';
    int retenue = 0;
    chiffre *ca = a.dernier;
    chiffre *cb = b.dernier;
    while (ca != NULL && cb != NULL)
    {
        int tmp = char2int(ca->c) - char2int(cb->c) + retenue;
        if (tmp < 0)
        {
            retenue = -1;
            tmp += 10;
        }
        else
            retenue = 0;
        c = add_chiffre(c, int2char(tmp), 0);
        ca = ca->precedent;
        cb = cb->precedent;
    }
    while (ca != NULL)
    {
        if (retenue != 0)
        {
            int tmp = char2int(ca->c) + retenue;
            if (tmp < 0)
            {
                retenue = -1;
                tmp += 10;
            }
            else
                retenue = 0;
            c = add_chiffre(c, int2char(tmp), 0);
        }
        else{
            if(boo){
                c = add_chiffre(c, ca->c, 0);
            }
        }
        ca = ca->precedent;
    }
    if (ca == NULL && retenue != 0)
        c.signe = '-';
    c = simplification(c);
    return c;
}

// fait le division de a par b si b vaut 1 renvoie le quotient sinon le modulo
static unbounded_int division(unbounded_int a, unbounded_int b, int bo)
{
    if (unbounded_int_cmp_ll(b, 0) == 0)
    { // ON ne divise pas par 0
        fprintf(stderr, "Erreur: Division par 0\n");
        return init_unbounded();
    }
    if (unbounded_int_cmp_ll(a, 0) == 0) // si a ==0
        return ll2unbounded_int(0);
    if (unbounded_int_cmp_unbounded_int(a, b) == -1)
    { // si a < b -> 0
        if (bo == 1)
            return ll2unbounded_int(0);
        else
            return a;
    }

    if (unbounded_int_cmp_unbounded_int(u_abs(b), u_abs(a)) == 0)
    {
        return ll2unbounded_int(1);
    }
    // conversion de a et b en binaire
    unbounded_int b_a = TenToTwo(a);
    unbounded_int diviseur = TenToTwo(b);

    unbounded_int dividende = init_unbounded();
    dividende.signe = '+';
    unbounded_int quotient = init_unbounded();
    quotient.signe = '+';
    chiffre *chiffre = b_a.premier;
    dividende = add_chiffre(dividende, chiffre->c, 1); // 1er chiffre du dividende
    chiffre = chiffre->suivant;

    while (chiffre != NULL)
    {
        if (unbounded_int_cmp_unbounded_int(dividende, diviseur) == -1) // si dividende < diviseur
            quotient = add_chiffre(quotient, '0', 1);                   // ajoute 0 au quotient
        else
        {
            dividende = soustraction(dividende, diviseur,0); // dividende - diviseur
            quotient = add_chiffre(quotient, '0' + 1, 1);          // ajoute 1 au quotient
        }
        dividende = add_chiffre(dividende, chiffre->c, 1); // ajoute un chiffre au dividende
        dividende = simplification(dividende);
        chiffre = chiffre->suivant;
    }
    if (unbounded_int_cmp_unbounded_int(dividende, diviseur) == -1)
        quotient = add_chiffre(quotient, '0', 1);
    else
        quotient = add_chiffre(quotient, '0' + 1, 1);

    // Quotient
    quotient = simplification(quotient);
    quotient = TwoToTen(quotient); // convertion en base 10
    liberer(b_a);
    liberer(diviseur);
    liberer(dividende);
    if (!bo)
    { // Modulo
        unbounded_int tmp = unbounded_int_produit(quotient,b);
        return unbounded_int_difference(a,tmp);
    }
    return quotient;
}