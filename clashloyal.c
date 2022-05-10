#include "SDL.h"
#include "clashloyal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//=========================== MON CODE =====================================

//============================= plateauDuJeu ===================================

TplateauJeu AlloueTab2D(int colonne, int ligne)
{
    TplateauJeu jeu;
    int i;
    jeu = (Tunite**)malloc(sizeof(Tunite*)*ligne);
    for ( i=0;i<ligne;i++)
    {
        *(jeu +i) = (Tunite*)malloc(sizeof(Tunite)*colonne);
    }
    return jeu;
}

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur){  //code prof
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }
}

void PositionnePlayerOnPlateau(TListePlayer player,TplateauJeu jeu)
{
    int x,y;
    while (player != NULL)
    {
        x = player->pdata->posX;
        y = player->pdata->posY;
        jeu[x][y] = player->pdata;
        player = player->suiv;
        printf("(%d,%d)\t",x,y);
    }
}
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[6]={"T", "R", "A", "C", "D", "G"};

    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
            if (jeu[i][j] != NULL){
                    printf("\t \t \t %s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(" ");  //cad pas d'unité sur cette case
        }
        printf("\n");
    }
}
//========================= Joueur =======================================

//initialise un joueur vide
TListePlayer initPlayervide()
{
    TListePlayer player;
    player = (TListePlayer)malloc(sizeof(struct T_cell));
    player->pdata = NULL;
    player->suiv = NULL;
    return player;
}
TListePlayer ajoutObj(Tunite *obj, TListePlayer player)
{
    TListePlayer newTemp;
    newTemp = (TListePlayer)malloc(sizeof(struct T_cell));
    newTemp->pdata = obj;
    newTemp->suiv = NULL;
    TListePlayer temp = player;


    if(temp->pdata == NULL){
        player = newTemp;
    }
    else {
            while(temp->suiv != NULL)
                    {
                        temp = temp->suiv;
                    }
            temp->suiv = newTemp;
         }
    return player;
}

bool tourRoiDetruite(TListePlayer player1,TListePlayer player2)
{
    bool cont = true;
    printf("Point de vie de la tour du roi joueur 1 : %d / joueur 2 : %d\n",player1->pdata->pointsDeVie,player2->pdata->pointsDeVie);
    if (player1->pdata->pointsDeVie<=0)
       {
          cont = false;
       }
    if (player2->pdata->pointsDeVie<=0)
       {
          cont =false;
       }
    return cont;
}

//======================= Deplacement =========================================

TplateauJeu deplacement (Tunite* uniteSeDeplace, TplateauJeu jeu,int sensDeplacement)
{
    int i;
    i = uniteSeDeplace->vitessedeplacement;
    int x_origine,y_origine;
    x_origine = uniteSeDeplace->posX;
    y_origine = uniteSeDeplace->posY;
    while (i>0)
    {
        int x = uniteSeDeplace->posX;
        int y = uniteSeDeplace->posY;
        if (x < 5)
        {
            if (jeu[x+1][y+sensDeplacement]==NULL)
            {
                uniteSeDeplace->posY = y+sensDeplacement;
                uniteSeDeplace->posX = x+1;
            }
            i--;
        }
        else if (x >5)
        {
            if (jeu[x-1][y+sensDeplacement]==NULL)
            {
                uniteSeDeplace->posY = y+sensDeplacement;
                uniteSeDeplace->posX = x-1;
            }
            i--;
        }
        else if (x==5)
        {
            if (jeu[x][y+sensDeplacement]==NULL)
            {
                uniteSeDeplace->posY = y+sensDeplacement;
                uniteSeDeplace->posX = x;
            }
            i--;
        }
    }
    printf("deplacement unite : (%d,%d)=>(%d,%d)\n",x_origine,y_origine,uniteSeDeplace->posX,uniteSeDeplace->posY);
    jeu[x_origine][y_origine]= NULL;
    jeu[uniteSeDeplace->posX][uniteSeDeplace->posY]= uniteSeDeplace;
    return jeu;
}

void mooveJoueur(TListePlayer player,TplateauJeu jeu)
{
    TListePlayer temp = player;
    int y_TourRoi = temp->pdata->posY;
    int sens;
    if (y_TourRoi == 1)
    {
        sens = 1;
    }
    else
    {
        sens = -1;
    }
    while (temp!=NULL)
    {
        deplacement(temp->pdata,jeu,sens);
        temp = temp->suiv;
    }
}

//========================== Combat ======================================

//calcule la distance entre deux unites
int distance(Tunite *unite1, Tunite *unite2)
{
    int dist = sqrt(pow(abs(unite1->posX-unite2->posX),2)+pow(abs(unite1->posY-unite2->posY),2));
    printf("unite attaquante (%d,%d), unite attaque(%d,%d)",unite1->posX,unite1->posY,unite2->posX,unite2->posY);
    printf("la distance : %d\n",dist);
    return dist;
}
bool atteignable(Tunite *unite1, Tunite *unite2)
{
    bool ciblePossible;
    if (unite1->cibleAttaquable==solEtAir)
    {
        if (unite2->maposition==sol || unite2->maposition==air)
        {
            ciblePossible = true;
        }
    }
    else if (unite1->cibleAttaquable==sol)
    {
        if (unite2->maposition==sol)
        {
            ciblePossible = true;
        }
        else
        {
            ciblePossible = false;
        }
    }
    return ciblePossible;
}
// Trie de la liste des unites en fonction de leur vitesse d'attaque
/*/Si la liste est vide ou si il n'y a qu'un seul élément, il n'y a rien à trier. Sinon, on regarde si la vitesse d'attaque de
a première unité est supérieure à celle de la deuxième. Si ce n'est pas le cas, la liste doit être re-parcourue et on inverse
les deux unités. On avance ensuite dans la liste, en remplaçant les éléments à chaque fois que le cas vu précédemment se répète,
et ainsi de suite tant que la liste n'est pas entièrement triée. Au final, la fonction retourne la liste triée.
/*/
TListePlayer trieUnite(TListePlayer player)
{
  if( player == NULL )
    return NULL;
  if( player->suiv == NULL )
    return player;
  TListePlayer temp = player;
  int again;
  do {

    TListePlayer precedent = NULL;
    TListePlayer element = temp;
    TListePlayer suivant = element->suiv;
    again = 0;

    while( suivant != NULL ) {
      if( suivant->pdata->vitesseAttaque > element->pdata->vitesseAttaque)
        {
            again = 1;
            if( precedent == NULL )
            {
                temp = suivant;
            }
            else
            {
                precedent->suiv = suivant;
            }
            element->suiv = suivant->suiv;
            suivant->suiv = element;
            precedent = suivant;
            suivant = element->suiv;
      }
      else
        {
            precedent = element;
            element = element->suiv;
            suivant = element->suiv;
        }
    }
  } while(again == 1);
  return temp;
}

// toutes les unités à porté de l'unité adverse en partant de la liste de toutes les unités du joueur
TListePlayer quiEstAPortee (TListePlayer player, Tunite *uniteAdverse){
    TListePlayer temp = player;
    int i = 0;
    TListePlayer aPortee;
    aPortee = (TListePlayer)malloc(sizeof(struct T_cell));
    aPortee->pdata = NULL;
    aPortee->suiv = NULL;
    while(temp !=NULL)
    {
        if(temp->pdata->portee >= distance(temp->pdata,uniteAdverse)&& atteignable(temp->pdata,uniteAdverse) == 1){
            printf("add aporte\n");
            i=1;
            aPortee = ajoutObj(temp->pdata,aPortee);
            temp = temp->suiv;
        }else{
        temp = temp->suiv;
        }
    }
    if (i==0)
    {
        aPortee=NULL;
    }
    return aPortee;
}

int PV(Tunite *unite){
    return unite->pointsDeVie;
}

bool peutattaquer(Tunite *unite){
    return unite->peutAttaquer == 1;
}

int degatsubis(Tunite *uniteciblee, Tunite *uniteattaquee){
    return PV(uniteciblee)-uniteattaquee->degats;
}

TListePlayer combat(TListePlayer player, Tunite *uneUniteDeLautreJoueur)
{
    TListePlayer aportee = quiEstAPortee(player, uneUniteDeLautreJoueur);
    aportee = trieUnite(aportee);
    printf("liste a porte \n");
    while(aportee != NULL){

        if(aportee->pdata->peutAttaquer == 1 && uneUniteDeLautreJoueur->pointsDeVie > 0)
        {
            printf("unite a porte \n");
            printf("unite attaque :  point attaque=%d\n",aportee->pdata->degats);
            printf("unite subis attaque :  point de vie=%d\n",uneUniteDeLautreJoueur->pointsDeVie);
            uneUniteDeLautreJoueur->pointsDeVie = degatsubis(uneUniteDeLautreJoueur,aportee->pdata);
            aportee->pdata->peutAttaquer = 0;
            printf("unite attaque :  pv=%d\n",uneUniteDeLautreJoueur->pointsDeVie);

        }
        aportee = aportee->suiv;
    }
    return player;
}
void restattaque(TListePlayer joueur1,TListePlayer joueur2)
{
    TListePlayer temp1 = joueur1;
    TListePlayer temp2 = joueur2;
    while (temp2!=NULL)
    {
        temp2->pdata->peutAttaquer=1;
        temp2 = temp2->suiv;
    }
    while (temp1!=NULL)
    {
        temp1->pdata->peutAttaquer=1;
        temp1 = temp1->suiv;
    }
}

// on utilise un TListePlayer pour récupérer la liste du joueur directement. on a essayé le void mais il ne fait qu'une suppression statique sans réelle changement au final.
TListePlayer supprimeUnite (TListePlayer player, Tunite *UniteDetruite)
{
    TListePlayer temp = player;
    TListePlayer file = temp;
    file = (TListePlayer)malloc(sizeof(struct T_cell));
    if (UniteDetruite->nom == tourRoi)
    {
        player = NULL;
    }
    else
    {
        if (temp->pdata == UniteDetruite)
        {
            player = temp->suiv;
            printf("unite du roi est detruite ");
        }
        else
        {
            while (temp->pdata!= UniteDetruite || temp == NULL)
            {
                file = temp;
                temp = temp->suiv;
            }
            if (temp== NULL )
            {
                file->suiv = temp;
            }
            else
            {
                file->suiv = temp->suiv;
            }
        }
    printf("Unite supprime\n");
    }
    return player;
}

void attaquer (TListePlayer player1, TListePlayer player2,TplateauJeu Jeu)
{
    TListePlayer temp1 = player1;
    TListePlayer temp2 = player2;
    while (temp2!=NULL)
    {
        printf("tentative d'attaque\n");
        temp1 = combat(temp1,temp2->pdata);
        if (temp2->pdata->pointsDeVie <= 0 )
        {
            printf("L'unite detruite est (%d,%d)",temp2->pdata->posX,temp2->pdata->posY);
            player2 = supprimeUnite(player2,temp2->pdata);
            Jeu[temp2->pdata->posX][temp2->pdata->posY]=NULL;
        }
        temp2 = temp2->suiv;
    }

}
//========================== Elixir ======================================

int stockElixir (TListePlayer player, int elixirPlayer)
{
    int temp = elixirPlayer;
    TListePlayer tempPlayer = player;
    int random = (rand()%(3-1+1))+ 1; // (rand() % (MAX - MIN + 1)) + MIN
    printf("elixir gagne : %d \n",random);
    temp = random+temp;
    if (temp>10)
    {
        temp = 10;
    }
    return temp;
}

//========================= Unite =======================================

Tunite *creeTour(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tour;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 100;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 0;
    return nouv;
}
Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 120;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 0;
    return nouv;
}

Tunite *creeChevalier (int posx, int posy)
{
    Tunite *chevalier;
    chevalier = (Tunite*)malloc(sizeof(Tunite));
    TuniteDuJeu unite;
    chevalier->nom = unite +3;
    chevalier->cibleAttaquable = sol;
    chevalier->maposition = sol;
    chevalier->pointsDeVie = 400;
    chevalier->vitesseAttaque = 1.5;
    chevalier->degats = 250;
    chevalier->portee = 1;
    chevalier->vitessedeplacement = 2.0;
    chevalier->coutEnElixir = 4;
    chevalier->peutAttaquer = 1;
    chevalier->posX = posx;
    chevalier->posY = posy;
    return chevalier;
}

Tunite *creeGargouille (int posx, int posy)
{
    Tunite *gargouille;
    gargouille = (Tunite*)malloc(sizeof(Tunite));
    TuniteDuJeu unite;
    gargouille->nom = unite+5;
    gargouille->cibleAttaquable = solEtAir;
    gargouille->maposition = air;
    gargouille->pointsDeVie = 80;
    gargouille->vitesseAttaque = 0.6;
    gargouille->degats = 90;
    gargouille->portee = 1;
    gargouille->vitessedeplacement = 3.0;
    gargouille->coutEnElixir = 1;
    gargouille->peutAttaquer = 1;
    gargouille->posX= posx;
    gargouille->posY= posy;
    return gargouille;
}

Tunite *creeDragon(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 70;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 3;
    return nouv;
}

Tunite *creeArcher(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 2;
    return nouv;
}

//========================= Creation Unite =======================================

Tunite *AcheteUnite(int *elixirEnStockduJoueur)
{
    int elixir = *elixirEnStockduJoueur;
    Tunite *unite ;
    unite = (Tunite*)malloc(sizeof(Tunite));
    int randomUnite = (rand()%(4-1+1))+1;
    int randomProbaAchat = (rand()%(99-1+1))+1;
    printf("Proba achat : %d\n",randomProbaAchat);
    printf("Proba unite : %d\n",randomUnite);
    if (randomProbaAchat>=60)
    {
        if(randomUnite == 1)
        {
            if (elixir>=1)
            {
                unite = creeGargouille(0,0);
                *elixirEnStockduJoueur = *elixirEnStockduJoueur-1;
                printf("Achat unite 1 elixir restant : %d\n",*elixirEnStockduJoueur);
            }
        }else if (randomUnite==2)
        {
            if (elixir>=2)
            {
                unite = creeArcher(0,0);
                *elixirEnStockduJoueur = *elixirEnStockduJoueur-2;
                printf("Achat unite 2 elixir restant : %d\n",*elixirEnStockduJoueur);
            }
        }else if (randomUnite==3)
        {
            if(elixir>=3)
            {
                unite = creeDragon(0,0);
                *elixirEnStockduJoueur = *elixirEnStockduJoueur-3;
                printf("Achat unite 3 elixir restant : %d\n",*elixirEnStockduJoueur);
            }
        }
        else if (randomUnite==4)
        {
            if(elixir>=4)
            {
                unite = creeChevalier(0,0);
                *elixirEnStockduJoueur = *elixirEnStockduJoueur-4;
                printf("Achat unite 4 elixir restant : %d\n",*elixirEnStockduJoueur);
            }
        }
    }
    else
    {
        unite = NULL;
        printf("Aucun achat elixir restant : %d\n",elixir);
    }
    return unite;
}

void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite,TplateauJeu jeu)
{
    TListePlayer temp = *player;
    Tunite *unite = nouvelleUnite;
    int x,y;
    int i=1;
    x = (rand()%(6-4+1))+4;
    if (x==5)
    {
        x = 4;
    }
    while (i==1)
    {
        if (temp->pdata->posY==1)
        {
            y = 3;
        }
        else if (temp->pdata->posY==17)
        {
            y=15;
        }
        if (jeu[x][y]!=NULL)
        {
            if (x>5)
            {
                x = x+1;
            }
            else if (x<5)
            {
                x = x-1;
            }
        }
        else
        {
            i = 0;
        }
    }
    unite->posX=x;
    unite->posY=y;
    *player = ajoutObj(unite,*player);
    printf("nouvelle unite :(%d,%d)\n",x,y);
}
