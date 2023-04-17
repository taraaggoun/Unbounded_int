#include "unbounded_int.h"

int main()
{
    printf("\n\n---------------------------------------------------------------------\n");
    printf("string2unbounded_int > Conversion d'un string vers un unbounded_int : \n");
    printf("---------------------------------------------------------------------\n");

    const char *stra;
    stra = "32768";
    printf("\n Test 1 : la chaine a vaut %s\n", stra);
    unbounded_int a = string2unbounded_int(stra);
    affiche(a);

    const char *strb;
    strb = "-2147483648";
    printf("\n Test 2 : la chaine b vaut %s\n", strb);
    unbounded_int b = string2unbounded_int(strb);
    affiche(b);

    const char *strc;
    strc = "+9223372036854775808";
    printf("\n Test 3 : la chaine c vaut %s\n", strc);
    unbounded_int c = string2unbounded_int(strc);
    affiche(c);

    // Test erreur ce n'est pas un unbounded
    // en commentaire car le programme s'arrete
    /*
    const char *strerror;
    strerror = "1a";
    printf("\n Test 4 : la chaine error vaut %s\n", strerror);
    unbounded_int error = string2unbounded_int(strerror);
    affiche(error);
    liberer(error);
    */

    printf("\n\n--------------------------------------------------------------------\n");
    printf("ll2unbounded_int > Conversion d'un long long vers un unbounded_int : \n");
    printf("--------------------------------------------------------------------\n");
    long long lld = 6;
    printf("\n Test 1 : le long long d vaut %lld\n", lld);
    unbounded_int d = ll2unbounded_int(lld);
    affiche(d);

    long long lle = -15;
    printf("\n Test 2 : le long long e vaut %lld\n", lle);
    unbounded_int e = ll2unbounded_int(lle);
    affiche(e);

    long long llf = +42;
    printf("\n Test 3 : le long long f vaut %lld\n", llf);
    unbounded_int f = ll2unbounded_int(llf);
    affiche(f);

    printf("\n\n---------------------------------------------------------------------\n");
    printf("unbounded_int2string > Conversion d'un unbounded_int vers un string : : \n");
    printf("---------------------------------------------------------------------\n");

    char *sd = unbounded_int2string(d);
    printf("\n Test 1 :\n");
    affiche(d);
    printf("La chaine de caractère 'sd' vaut alors : %s\n", sd);

    char *se = unbounded_int2string(e);
    printf("\n Test 2 :\n");
    affiche(e);
    printf("La chaine de caractère 'se' vaut alors : %s\n", se);

    char *sf = unbounded_int2string(f);
    printf("\n Test 3 \n");
    affiche(f);
    printf("La chaine de caractere 'sf' vaut alors: %s\n", sf);

    printf("\n\n---------------------------------------------------------------------\n");
    printf("Comparaison de deux unbounded_int {unbounded_int_cmp_unbounded_int} : \n");
    printf("---------------------------------------------------------------------\n");

    printf("\n La régle est simple : \n");
    printf("        ----------------\n");
    printf("        | a < b -> - 1 |\n");
    printf("        | a == b ->  0 |\n");
    printf("        | a > b -> + 1 |\n");
    printf("        ----------------\n\n");

    printf("Test 1 :\n");
    printf("   a = %s et b = %s donne %d\n\n", stra, stra, unbounded_int_cmp_unbounded_int(a, a));
    printf("Test 2 : \n");
    printf("   a = %s et b = %s donne %d\n\n", strb, strc, unbounded_int_cmp_unbounded_int(b, c));
    printf("Test 3 : \n");
    printf("   a = %s et b = %s donne %d\n\n", sd, se, unbounded_int_cmp_unbounded_int(d, e));
    printf("Test 4 : \n");
    printf("   a = %s et b = %s donne %d\n\n", stra, sf, unbounded_int_cmp_unbounded_int(a, f));
    printf("Test 5 : \n");
    printf("   a = %s et b = %s donne %d\n\n", strb, se, unbounded_int_cmp_unbounded_int(b, e));

    printf("\n\n-------------------------------------------------------------------------\n");
    printf("Comparaison d'un unbounded_int avec un long long {unbounded_int_cmp_ll} :  \n");
    printf("-------------------------------------------------------------------------\n");

    printf("\n La régle est simple : \n");
    printf("        ----------------\n");
    printf("        | a < b -> - 1 |\n");
    printf("        | a == b ->  0 |\n");
    printf("        | a > b -> + 1 |\n");
    printf("        ----------------\n\n");

    printf("Test 1 :\n");
    printf("   a = %s et b = %s donne %d\n\n", stra, se, unbounded_int_cmp_ll(a, lle));
    printf("Test 2 : \n");
    printf("   a = %s et b = %s donne %d\n\n", strb, sf, unbounded_int_cmp_ll(b, llf));
    printf("Test 3 : \n");
    printf("   a = %s et b = %s donne %d\n\n", strc, sd, unbounded_int_cmp_ll(c, lld));
    printf("Test 4 : \n");
    printf("   a = %s et b = %s donne %d\n\n", sd, sd, unbounded_int_cmp_ll(d, lld));

    printf("\n\n------------------------------------------------------\n");
    printf("Addition de deux unbounded_int {unbounded_int_somme} : \n");
    printf("------------------------------------------------------\n\n");

    printf("--------\n");
    printf("Test 1 :\n");
    printf("--------\n");

    printf("   a est representé comme suit : \n");
    affiche(a);
    printf("\n   b est representé comme suit : \n");
    affiche(d);
    printf("\n   La somme des deux donne le unbounded_int suivant: \n");
    unbounded_int sad = unbounded_int_somme(a, d);
    affiche(sad);

    printf("\n\n--------\n");
    printf("Test 2 :\n");
    printf("--------\n");

    printf("   a est representé comme suit : \n");
    affiche(b);
    printf("\n   b est representé comme suit : \n");
    affiche(e);
    printf("\n   La somme des deux donne le unbounded_int suivant: \n");
    unbounded_int sbe = unbounded_int_somme(b, e);
    affiche(sbe);

    printf("\n\n--------\n");
    printf("Test 3 :\n");
    printf("--------\n");

    printf("   a est representé comme suit : \n");
    affiche(c);
    printf("\n   b est representé comme suit : \n");
    affiche(b);
    printf("\n   La somme des deux donne le unbounded_int suivant: \n");
    unbounded_int scf = unbounded_int_somme(c, b);
    affiche(scf);

    printf("\n\n--------\n");
    printf("Test 4 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(e);
    printf("\n   b est representé comme suit : \n");
    affiche(d);
    printf("\n   La somme des deux donne le unbounded_int suivant: \n");
    unbounded_int sae = unbounded_int_somme(e, d);
    affiche(sae);

    printf("\n\n---------------------------------------------------------------\n");
    printf("Soustraction de deux unbounded_int {unbounded_int_difference} : \n");
    printf("---------------------------------------------------------------\n\n");

    printf("--------\n");
    printf("Test 1 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(a);
    printf("\n   b est representé comme suit : \n");
    affiche(d);
    printf("\n   La différence des deux donne le unbounded_int suivant: \n");
    unbounded_int dad = unbounded_int_difference(a, d);
    affiche(dad);

    printf("\n\n--------\n");
    printf("Test 2 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(b);
    printf("\n   b est representé comme suit : \n");
    affiche(e);
    printf("\n   La différence des deux donne le unbounded_int suivant: \n");
    unbounded_int dbe = unbounded_int_difference(b, e);
    affiche(dbe);

    printf("\n\n--------\n");
    printf("Test 3 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(e);
    printf("\n   b est representé comme suit : \n");
    affiche(f);
    printf("\n   La différence des deux donne le unbounded_int suivant: \n");
    unbounded_int dcf = unbounded_int_difference(e, f);
    affiche(dcf);

    printf("\n\n--------\n");
    printf("Test 4 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(a);
    printf("\n   b est representé comme suit : \n");
    affiche(b);
    printf("\n   La différence des deux donne le unbounded_int suivant: \n");
    unbounded_int daa = unbounded_int_difference(a, b);
    affiche(daa);

    printf("\n\n--------------------------------------------------------------\n");
    printf("Multiplication de deux unbounded_int {unbounded_int_produit} : \n");
    printf("--------------------------------------------------------------\n\n");

    printf("--------\n");
    printf("Test 1 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(a);
    printf("\n   b est representé comme suit : \n");
    affiche(d);
    printf("\n   La produit des deux donne le unbounded_int suivant: \n");
    unbounded_int mad = unbounded_int_produit(a, d);
    affiche(mad);

    printf("\n\n--------\n");
    printf("Test 2 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(b);
    printf("\n   b est representé comme suit : \n");
    affiche(e);
    printf("\n   La produit des deux donne le unbounded_int suivant: \n");
    unbounded_int mbe = unbounded_int_produit(b, e);
    affiche(mbe);

    printf("\n\n--------\n");
    printf("Test 3 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(b);
    printf("\n   b est representé comme suit : \n");
    affiche(f);
    printf("\n   La produit des deux donne le unbounded_int suivant: \n");
    unbounded_int mcf = unbounded_int_produit(b, f);
    affiche(mcf);

    printf("\n\n--------\n");
    printf("Test 4 :\n");
    printf("--------\n");
    printf("   a est representé comme suit : \n");
    affiche(d);
    printf("\n   b est representé comme suit : \n");
    affiche(e);
    printf("\n   La produit des deux donne le unbounded_int suivant: \n");
    unbounded_int mde = unbounded_int_produit(d, e);
    affiche(mde);
    printf("\n");

    // Libération de la mémoire :
    liberer(a);
    liberer(b);
    liberer(c);

    liberer(d);
    liberer(e);
    liberer(f);

    free(sd);
    free(se);
    free(sf);

    liberer(sad);
    liberer(sbe);
    liberer(scf);
    liberer(sae);

    liberer(dad);
    liberer(dbe);
    liberer(dcf);
    liberer(daa);

    liberer(mad);
    liberer(mbe);
    liberer(mcf);
    liberer(mde);

    return 0;
}