#ifndef JEU2048_H_INCLUDED
#define JEU2048_H_INCLUDED

#define LARGEURJEU 11
#define HAUTEURJEU 19
#include <stdbool.h>
#include <math.h>

//============================= Structure et Type ===================================

typedef enum{tour, tourRoi, archer, chevalier, dragon, gargouille} TuniteDuJeu;
typedef enum{sol, solEtAir, air } Tcible;


typedef struct {
    TuniteDuJeu nom;
    Tcible cibleAttaquable;	//indique la position des unités que l’on peut attaquer
    Tcible maposition;		//indique soit « air » soit « sol », utile pour savoir
                            //qui peut nous attaquer
    int pointsDeVie;
    float vitesseAttaque;	//en seconde, plus c’est petit plus c’est rapide
    int degats;
    int portee ;			//en mètre, distance sur laquelle on peut atteindre une
                            //cible

    float vitessedeplacement;	//en m/s
    int posX, posY;			//position sur le plateau de jeu
    int peutAttaquer;		//permet de gérer le fait que chaque unité attaque une
                            //seule fois par tour ;
                            //0 = a déjà attaqué, 1 = peut attaquer ce tour-ci
                            // à remettre à 1 au début de chaque tour
    int coutEnElixir;
} Tunite;

typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unité
} *TListePlayer;


typedef Tunite* ** TplateauJeu;  ////tableau a deux dimensions de largeur 11 et hauteur 19 contenant des pointeurs (Tunite*)

//============================= plateauDuJeu ===================================

TplateauJeu AlloueTab2D(int largeur, int hauteur);
void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur);
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

//========================= Joueur =======================================

TListePlayer initPlayervide();
TListePlayer ajoutObj(Tunite *obj, TListePlayer player);
bool tourRoiDetruite(TListePlayer player1,TListePlayer player2);

//======================= Deplacement =========================================

TplateauJeu deplacement (Tunite* uniteSeDeplace, TplateauJeu jeu,int sensDeplacement);
void moveJoueur(TListePlayer player,TplateauJeu jeu);

//========================== Combat ======================================

int distance(Tunite *unite1, Tunite *unite2);
bool atteignable(Tunite *unite1, Tunite *unite2);
TListePlayer trieUnite(TListePlayer player);
TListePlayer quiEstAPortee (TListePlayer player, Tunite *uniteAdverse);
int PV(Tunite *unite);
bool peutattaquer(Tunite *unite);
int degatsubis(Tunite *uniteciblee, Tunite *uniteattaquee);
TListePlayer combat(TListePlayer player, Tunite *uneUniteDeLautreJoueur);
void restattaque(TListePlayer joueur1,TListePlayer joueur2);
TListePlayer supprimeUnite (TListePlayer player, Tunite *UniteDetruite);
void attaquer (TListePlayer player1, TListePlayer player2,TplateauJeu jeu);

//========================== Elixir ======================================

int stockElixir (TListePlayer player, int elixirPlayer);

//========================= Unite =======================================

Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeDragon(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);
Tunite *creeTour(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);

//========================= Creation Unite =======================================

Tunite *AcheteUnite(int *elixirEnStockduJoueur);
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite,TplateauJeu jeu);

#endif // JEU2048_H_INCLUDED
