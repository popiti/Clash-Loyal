//NOM prénom étudiant 1 :
//NOM prénom étudiant 2 :

#include "SDL.h"
#include "maSDL.h"    //bibliothèque avec des fonction d'affichage pour le jeu 2048
#include "clashloyal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>


/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU*40,
        HAUTEURJEU*40,
        SDL_WINDOW_SHOWN
    );

    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow);  //le sprite qui couvre tout l'écran
    SDL_Surface* pSpriteTour = SDL_LoadBMP("./data/Tour.bmp");  //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data/TourRoi.bmp"); //indice 1
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data/Archer.bmp"); //indice 2
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data/Chevalier.bmp"); //indice 3
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data/Dragon.bmp"); //indice 4
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data/Gargouille.bmp"); //indice 5
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Eau.bmp"); //indice 6  Ne figure pas dans l'enum TuniteDuJeu
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Herbe.bmp"); //indice 7 idem
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Pont.bmp"); //indice 8 idem
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp"); //indice 9 idem

    // ASTUCE : on stocke le sprite d'une unité à l'indice de son nom dans le type enum TuniteDuJeu, dans le tableau TabSprite
    // SAUF pour l'Eau, l''herbe et le pont qui apparaitront en l absence d'unité (NULL dans le plateau) et en foction de certains indices x,y définissant le chemin central
    SDL_Surface* TabSprite[10]={pSpriteTour,pSpriteTourRoi,pSpriteArcher,pSpriteChevalier,pSpriteDragon,pSpriteGargouille,pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre};


    if ( pSpriteTour )  //si le permier sprite a bien été chargé, on suppose que les autres aussi
    {
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
        affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);

        prepareAllSpriteDuJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
        maj_fenetre(pWindow);

        //A COMMENTER quand vous en aurez assez de cliquer sur ces popups ^^

        message("Welcome in ClashLoyal","Clash Loyal developpe par Theo MATHIEU et Walid EL MEJJAD");
        message("et fin","ECHAP->quitter, S/C ET D/V les gerer les sauvegardes");

        /**********************************************************************/
        /*                                                                    */
        srand(time(NULL));
        jeu = AlloueTab2D(11,20);
        TListePlayer joueur1;
        joueur1 = initPlayervide();
        TListePlayer joueur2;
        joueur2 = initPlayervide();
        joueur1 = ajoutObj(creeTourRoi(5,1),joueur1);
        joueur1 = ajoutObj(creeTour(5,3),joueur1);
        joueur2 = ajoutObj(creeTourRoi(5,17),joueur2);
        joueur2 = ajoutObj(creeTour(5,15),joueur2);
        int elixirJoueur1,elixirJoueur2;
        elixirJoueur1 = 0;
        elixirJoueur2 = 0;

        /*
        // FIN de vos variables                                                              */
        /**********************************************************************/

        // boucle principale du jeu
        int cont = 1;
        int j=1;
        while ( cont != 0 ){   //VOUS DEVEZ GERER (DETECTER) LA FIN DU JEU -> tourRoiDetruite
                SDL_PumpEvents(); //do events


                /***********************************************************************/
                /*                                                                     */
                /*                                                                     */
                printf("///////////////////////\n tour : %d \n//////////////////////\n",j);
                printf("Liste des unites joueur 1 : ");
                PositionnePlayerOnPlateau(joueur1,jeu);
                printf("\nListe des unites joueur 2 : ");
                PositionnePlayerOnPlateau(joueur2,jeu);
                printf("\n");
                printf("Nbliste 1 : %d",uniteListe(joueur1));
                printf("\n");
                printf("Nbliste 2 : %d",uniteListe(joueur2));
                printf("\n");
                printf("///////////////////////\n Phase elixir \n//////////////////////\n");
                elixirJoueur1 = stockElixir(joueur1,elixirJoueur1);
                printf("Stock Elixir joueur 1 : %d \n",elixirJoueur1);
                elixirJoueur2 = stockElixir(joueur2,elixirJoueur2);
                printf("Stock Elixir joueur 2 : %d \n",elixirJoueur2);
                printf("///////////////////////\n Phase Achat \n//////////////////////\n");
                Tunite *uniteJoueur1;
                uniteJoueur1 = (Tunite*)malloc(sizeof(Tunite));
                printf("////////Achat joueur 1 : ////////////\n");
                uniteJoueur1 =  AcheteUnite(&elixirJoueur1);
                if (uniteJoueur1!=NULL)
                {
                    AjouterUnite(&joueur1,uniteJoueur1,jeu);
                }
                Tunite *uniteJoueur2;
                uniteJoueur2 = (Tunite*)malloc(sizeof(Tunite));
                printf("////////Achat joueur 2 : ////////////\n");
                uniteJoueur2 =  AcheteUnite(&elixirJoueur2);
                if (uniteJoueur2!=NULL)
                {
                    AjouterUnite(&joueur2,uniteJoueur2,jeu);
                }
                printf("///////////////////////\n Phase combat \n//////////////////////\n");
                printf("Attaque joueur 1 :\n");
                attaquer(joueur1,joueur2,jeu);
                printf("Attaque joueur 2 :\n");
                attaquer(joueur2,joueur1,jeu);
                printf("///////////////////////\n Phase deplacement \n//////////////////////\n");
                printf("Deplacement joueur 1 :\n");
                mooveJoueur(joueur1,jeu);
                printf("Deplacement joueur 2 :\n");
                mooveJoueur(joueur2,jeu);
                restattaque(joueur1,joueur2);
                printf("///////////////////////\n Tour des rois \n//////////////////////\n");
                cont = tourRoiDetruite(joueur1,joueur2);
                affichePlateauConsole(jeu,11,20);
                /*                                                                     */
                /*                                                                     */
                // FIN DE VOS APPELS
                /***********************************************************************/
                //affichage du jeu à chaque tour
                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);  //valeur du délai à modifier éventuellement

                //LECTURE DE CERTAINES TOUCHES POUR LANCER LES RESTAURATIONS ET SAUVEGARDES
                const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                if ( pKeyStates[SDL_SCANCODE_V] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur D */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                       // sauvegardeBinaire(joueur1,joueur2,elixirJoueur1,elixirJoueur2);
                        message("Sauvegarde","Save binary file done");

                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_C] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur C */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        //lectureBinaire(joueur1,joueur2,elixirJoueur1,elixirJoueur2);
                        message("Lecture","Read binary done");

                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_D] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur D */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        // sauvegardeSequentielle(joueur1,joueur2,elixirJoueur1,elixirJoueur2);
                        message("Sauvegarde","Save sequential done");
                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_S] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur S */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        //lectureSequentielle(joueur1,joueur2,elixirJoueur1,elixirJoueur2);
                        message("Lecture","Read sequential done");
                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_ESCAPE] ){
                        cont = 0;  //sortie de la boucle
                }
                j++;
                getch();


        }
        //fin boucle du jeu

        SDL_FreeSurface(pSpriteTour); // Libération de la ressource occupée par le sprite
        SDL_FreeSurface(pSpriteTourRoi);
        SDL_FreeSurface(pSpriteArcher);
        SDL_FreeSurface(pSpriteChevalier);
        SDL_FreeSurface(pSpriteDragon);
        SDL_FreeSurface(pSpriteGargouille);
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pWinSurf);
    }
    else
    {
        fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}
