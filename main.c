#include "arithmod.h"
#include "polynome.h"
#include "encryption.h"
#include "noisesize.h"
#include "operationbgv.h"
#include "switching&relin.h"
#include "parametre.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define DEGRE_MAX 4
int main(void)
{
    printf("\t Etude et implementation du chiffrement homomorphe BGV\n");
    printf("\t Mouhamed Aly THIAW \n");
    printf("\t Tuteurs de stage : Vincent ZUCCA, Christophe NEGRE\n");
    printf("\t Superviseur academique : Sylvain DUQUESNE \n");
    printf("\t Etablissement / Formation : Rennes 1 Universite, Master de Mathématiques de l’Information, Cryptographie\n");
    printf("\t Universite d’accueil : Universite de Perpignan Via Domitia\n");
    printf("\n");

    int saisie_utilisateur=0,nombre_module=0;
    do{
        srand(time(NULL));
        PolyRNS pk[2],cipher[2],cipher2[2],cipheradd[2],ciphermult[3],cipherms[3],cipherlk[2];
        float Bdec = 0.0;
        printf("\t Donner le nombre de modules\n");
        scanf("%d",&nombre_module);
        pk[0].nombre = nombre_module;
        pk[0].level = 0;
        Integer *Q=Setup(Q_Modulis,pk[0].nombre,Q_precomp,shift);
        Integer *P=Setup(P_Modulis,pk[0].nombre,P_precomp,shift);
        Poly sk,plaintext,plaintext2,decrypt,decrypt2,decryptadd,decryptmult,cms,rlkms,msgrns;
        Alloc_Poly(&sk,DEGRE_MAX,Q[0]);Alloc_Poly(&plaintext,DEGRE_MAX,Q[0]);Alloc_Poly(&msgrns,DEGRE_MAX,Q[0]);
        Alloc_Poly(&rlkms,DEGRE_MAX,Q[0]);Alloc_Poly(&plaintext2,DEGRE_MAX,Q[0]);
        Alloc_Poly(&decrypt,DEGRE_MAX,Q[0]);Alloc_Poly(&decrypt2,DEGRE_MAX,Q[0]);
        Alloc_Poly(&decryptadd,DEGRE_MAX,Q[0]);Alloc_Poly(&decryptmult,DEGRE_MAX,Q[0]);Alloc_Poly(&cms,DEGRE_MAX,Q[0]);
        Alloc_PolyRNS(&pk[0],pk[0].nombre,Q);Alloc_PolyRNS(&pk[1],pk[0].nombre,Q);Alloc_PolyRNS(&cipher[0],pk[0].nombre,Q);
        Alloc_PolyRNS(&cipher[1],pk[0].nombre,Q);Alloc_PolyRNS(&cipher2[0],pk[0].nombre,Q);Alloc_PolyRNS(&cipher2[1],pk[0].nombre,Q);
        Alloc_PolyRNS(&cipheradd[0],pk[0].nombre,Q);Alloc_PolyRNS(&cipheradd[1],pk[0].nombre,Q);Alloc_PolyRNS(&ciphermult[0],pk[0].nombre,Q);
        Alloc_PolyRNS(&ciphermult[1],pk[0].nombre,Q);Alloc_PolyRNS(&ciphermult[2],pk[0].nombre,Q);Alloc_PolyRNS(&cipherms[0],pk[0].nombre,Q);
        Alloc_PolyRNS(&cipherms[1],pk[0].nombre,Q);Alloc_PolyRNS(&cipherms[2],pk[0].nombre,Q);
        Alloc_PolyRNS(&cipherlk[0],pk[0].nombre,Q);Alloc_PolyRNS(&cipherlk[1],pk[0].nombre,Q);
        printf("\t Generation des cles \n");
        KeyGenRNS(&sk,pk,Q,pk[0].nombre);
        ConvertSk(&sk,&sk,Q[0]);
        printf("cle secrete = ");
        affiche(&sk,DEGRE_MAX);
        printf("cle publique = ");
        for(int i=0;i<pk[0].nombre;i++){
            affiche(&pk[0].tab[i],DEGRE_MAX);
            affiche(&pk[1].tab[i],DEGRE_MAX);
            printf("\n");
        }
        printf("\n");
        printf("\t Textes clairs \n");
        printf("\n");
        printf("\t message1 = ");
        for(int i = 0; i < DEGRE_MAX; i++){
            plaintext.coef[i] = rand()%2;
            plaintext2.coef[i] = rand()%2;
        }
        affiche(&plaintext,DEGRE_MAX);
        printf("\t message2 = ");
        affiche(&plaintext2,DEGRE_MAX);
        printf("\n");
        printf("\t Chiffrement \n");
        printf("\n");
        EncryptionRNS(&plaintext,cipher,pk,Q,pk[0].nombre);EncryptionRNS(&plaintext2,cipher2,pk,Q,pk[0].nombre);
        DecryptionRNS(cipher,2,&sk,&decrypt,Q,pk[0].level,pk[0].nombre);DecryptionRNS(cipher2,2,&sk,&decrypt2,Q,pk[0].level,pk[0].nombre);
        AdditionRNS(cipher,cipher2,cipheradd,Q,pk[0].nombre);MultiplicationRNS(cipher,cipher2,ciphermult,Q,pk[0].nombre);
        DecryptionRNS(cipheradd,2,&sk,&decryptadd,Q,pk[0].level,pk[0].nombre);
        printf("chiffre 1 = ");
        for(int i=0;i<pk[0].nombre;i++){
            affiche(&cipher[0].tab[i],DEGRE_MAX);
            affiche(&cipher[1].tab[i],DEGRE_MAX);
            printf("\n");
        }
        printf("\n");
        printf("chiffre 2 = ");
        for(int i=0;i<pk[0].nombre;i++){
            affiche(&cipher2[0].tab[i],DEGRE_MAX);
            affiche(&cipher2[1].tab[i],DEGRE_MAX);
            printf("\n");
        }
        printf("\n");
        printf("\t Dechiffrement chiffre 1  et chiffre 2\n");printf("\n");
        printf("\t Dec(cipher1) = ");
        affiche(&decrypt,DEGRE_MAX);
        printf("\t Dec(cipher2) = ");
        affiche(&decrypt2,DEGRE_MAX);
        printf("\n");
        printf("\t Operations homomorphes\n");printf("\n");
        printf("\t chiffre 1 + chiffre 2\n");
        for(int i=0;i<pk[0].nombre;i++){
            affiche(&cipheradd[0].tab[i],DEGRE_MAX);
            affiche(&cipheradd[1].tab[i],DEGRE_MAX);
            printf("\n");
        }
        printf("\n");
        printf("\t chiffre 1 x chiffre 2\n");
        for(int i=0;i<pk[0].nombre;i++){
            affiche(&ciphermult[0].tab[i],DEGRE_MAX);
            affiche(&ciphermult[1].tab[i],DEGRE_MAX);
            affiche(&ciphermult[2].tab[i],DEGRE_MAX);
            printf("\n");
        }
        printf("\n");
        printf("\t Dechiffrement de la somme et du produit\n");printf("\n");
        printf("\t Dec(cipheradd) = ");
        affiche(&decryptadd,DEGRE_MAX);
        printf("\t Dec(ciphermult) = ");
        DecryptionRNS(ciphermult,3,&sk,&decryptmult,Q,pk[0].level,pk[0].nombre);

        affiche(&decryptmult,DEGRE_MAX);
        printf("\n");
        printf("\t Taille du Bruit contenu dans les chiffres\n");
        Bdec = Q[0].modulus/2.0-0.5;
        printf("\t Bdec == %f\n",log2(Bdec));
        printf("\t Noisesize(cipher1) == %f\n",NoiseSizeRNS(cipher,2,&sk,Q,pk[0].level,pk[0].nombre));
        printf("\t Noisesize(cipher2) == %f\n",NoiseSizeRNS(cipher2,2,&sk,Q,pk[0].level,pk[0].nombre));
        printf("\t Noisesize (cipheradd) == %f\n",NoiseSizeRNS(cipheradd,2,&sk,Q,pk[0].level,pk[0].nombre));
        printf("\t Noisesize (ciphermult) == %f\n",NoiseSizeRNS(ciphermult,3,&sk,Q,pk[0].level,pk[0].nombre));
        printf("\n");
        printf("\t Relinearisation des chiffres \n");
        PolyRNS *rlk=RelinearKeyRNS(Q,P,&sk,pk[0].nombre);
        PolyRNS *relin = RelinearisationGHS(ciphermult,rlk,Q,P,pk[0].nombre,0);
        DecryptionRNS(relin,2,&sk,&msgrns,Q,pk[0].level,pk[0].nombre);
        printf("\t Dec(relin) = ");
        affiche(&msgrns,DEGRE_MAX);
        //printf("\t Noisesize(relin) == %f\n",NoiseSizeRNS(relin,2,&sk,Q,pk[0].level,pk[0].nombre));
        printf("\n");
        printf("\tTechnique de changement de module (Module Switching)\n");printf("\n");
        ModuleSwitching(ciphermult,3,cipherms,Q);
        DecryptionRNS(cipherms,3,&sk,&cms,Q,pk[0].level,pk[0].nombre);
        printf("\t Dec(cipherms) = ");
        affiche(&cms,DEGRE_MAX);
        printf("\t Noisesize(cipherms) == %f\n",NoiseSizeRNS(cipherms,3,&sk,Q,pk[0].level,pk[0].nombre));
        /*printf("\t Relinearisation des chiffres \n");
        PolyRNS *rlk=RelinearKeyRNS(Q,P,&sk,pk[0].nombre);
        PolyRNS *relin = RelinearisationGHS(ciphermult,rlk,Q,P,pk[0].nombre,1);
        DecryptionRNS(relin,2,&sk,&msgrns,Q,pk[0].level,1);
        printf("\t Dec(relin) = ");
        affiche(&msgrns,DEGRE_MAX);
        printf("\t Noisesize(relin) == %f\n",NoiseSizeRNS(relin,2,&sk,Q,pk[0].level,1));*/
        free(Q);free(P);Free_Poly(&sk);Free_Poly(&plaintext);Free_Poly(&plaintext2);Free_Poly(&cms);Free_Poly(&rlkms);
        Free_Poly(&decrypt);Free_Poly(&decrypt2);Free_Poly(&decryptadd);Free_Poly(&decryptmult);Free_Poly(&msgrns);
        for(int j=0;j<2;j++){
            Free_Poly(&pk[0].tab[j]);
            Free_Poly(&pk[1].tab[j]);
            Free_Poly(&cipher[0].tab[j]);
            Free_Poly(&cipher[1].tab[j]);
            Free_Poly(&cipher2[0].tab[j]);
            Free_Poly(&cipher2[1].tab[j]);
            Free_Poly(&cipheradd[0].tab[j]);
            Free_Poly(&cipheradd[1].tab[j]);
            Free_Poly(&ciphermult[0].tab[j]);
            Free_Poly(&ciphermult[1].tab[j]);
            Free_Poly(&ciphermult[2].tab[j]);
            Free_Poly(&cipherms[0].tab[j]);
            Free_Poly(&cipherms[1].tab[j]);
        }

        free(rlk);free(relin);
        printf("\t Taper 1 pour continuer ou 2 pour quitter\n");
        scanf("%d",&saisie_utilisateur);
    }while(saisie_utilisateur!=2);
    printf("\t Merci pour visite a bientot!\n");
	system("PAUSE");
    return 0;
}
