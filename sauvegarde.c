#include "SDL.h"
#include "maSDL.h"    //bibliothèque avec des fonction d'affichage pour le jeu 2048
#include "clashloyal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>

int uniteListe(TListePlayer joueur) // donne la liste d'unite du joueur
{
    int nbEntite;
    nbEntite = 1;
    TListePlayer temp = joueur;
    while(temp->suiv!=NULL)
    {
        nbEntite = nbEntite+1;
        temp = temp->suiv;
    }
    return nbEntite;
}


/*void sauvegardeBinaire (TListePlayer joueur1, TListePlayer joueur2, int elixirDuJoueur1, int elixirDuJoueur2)
{
    FILE *save;
    save = fopen("partiebin.clb","wb");
    int listeJoueur1 = uniteListe(joueur1);
    int listeJoueur2 = uniteListe(joueur2);
    fwrite(&listeJoueur1,sizeof(int),1,save);
    fwrite(&listeJoueur2,sizeof(int),1,save);
    fwrite(&elixirDuJoueur1,sizeof(int),1,save);
    fwrite(&elixirDuJoueur2,sizeof(int),1,save);
    TListePlayer temp1 = joueur1;
    TListePlayer temp2 = joueur2;
    while (temp1 !=NULL)
    {
        Tunite uniteJoueur1 = temp1->pdata;
        fwrite(uniteJoueur1,sizeof(Tunite),1,save);
        temp1 = temp1->suiv;
    }
    while (temp2 !=NULL)
    {
        Tunite uniteJoueur2 = temp2->pdata;
        fwrite(uniteJoueur2,sizeof(Tunite),1,save);
        temp2 = temp2->suiv;
    }
    fclose(save);
}

void lectureBinaire (TListePlayer joueur1, TListePlayer joueur2, int elixirDuJoueur1, int elixirDuJoueur2)
{
    FILE *save;
    save = fopen("partiebin.clb","rb");
    int listeJoueur1 = uniteListe(joueur1);
    int listeJoueur2 = uniteListe(joueur2);
    fread(&listeJoueur1,sizeof(int),1,save);
    fread(&listeJoueur2,sizeof(int),1,save);
    fread(&elixirDuJoueur1,sizeof(int),1,save);
    fread(&elixirDuJoueur2,sizeof(int),1,save);
    TListePlayer temp1 = joueur1;
    TListePlayer temp2 = joueur2;
    while (temp1 !=NULL)
    {
        Tunite uniteJoueur1 = temp1->pdata;
        fread(uniteJoueur1,sizeof(Tunite),1,save);
        temp1 = temp1->suiv;
    }
    while(temp2!=NULL)
    {
        Tunite uniteJoueur2 = temp2->pdata;
        fread(uniteJoueur2,sizeof(Tunite),1,save);
        temp2 = temp2->suiv;
    }
    fclose(save);
}

void sauvegardeSequentielle(TListePlayer joueur1, TListePlayer joueur2, int elixirDuJoueur1, int elixirDuJoueur2)
{
    FILE *save;
    save = fopen("partieseq.cls.","w");
    int listeJoueur1 = uniteListe(joueur1);
    int listeJoueur2 = uniteListe(joueur2);
    fprintf(save,"%d %d\n",elixirDuJoueur1,listeJoueur1);
    fprintf(save,"%d %d\n", elixirDuJoueur2,listeJoueur2);
    TListePlayer temp1 = joueur1;
    TListePlayer temp2 = joueur2;
    while (temp1 !=NULL)
    {
        Tunite uniteJoueur1 = temp1->pdata;
        fprintf(save,"%d %d %d %d\n", uniteJoueur1->pointsDeVie, uniteJoueur1->posX, uniteJoueur1->posY,uniteJoueur1->peutAttaquer);
        temp1 = temp1->suiv;
    }
    while (temp2 !=NULL)
    {
        Tunite uniteJoueur2 = temp2->pdata;
        fprintf(save,"%d %d %d %d\n", uniteJoueur2->pointsDeVie,uniteJoueur2->posX, uniteJoueur2->posY,uniteJoueur2->peutAttaquer);
        temp2 = temp2->suiv;
    }
    fclose(save);
}
*/
