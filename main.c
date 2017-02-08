#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLEBAT 3
#define TAILLEGRILLE 5
#define NBBAT 3

//3 bateaux de taille 3
//grille de 5 sur 5 (val de 0 a 4)

typedef struct cellule{
    int ligne;
    int colonne;
}cellule;

typedef struct bateau{
    cellule partie_bateau[TAILLEBAT];
    int etat;
}bateau;

typedef struct flotte{
    bateau num_bateau[NBBAT];
    int etat;
}flotte;


enum BOOL{
    False,True,
};

enum SENS{
    horizontal, vertical,
};

enum ETATELEMENT{
    mort, vivant,
};

int compare_cellule(cellule c1, cellule c2);
void crea_bateau(bateau *b);
int app_bateau(cellule celltest, bateau bat);
int app_flotte(cellule celltest,flotte f);
int verif_flotte_detr(flotte f);
int verif_bateau_detr(flotte armee_j, int i);

int main()
{
    /*
    REGLES DE GAMEPLAY CHOISIES :

    3 bateaux de 3 cellules sont places aleatoirement sur une grille de 5*5 cellules avant la partie
    Un joueur contre un joueur
    Toute cellule touchee voit ses valeurs tomber a -1,-1
    */
    int i,j,ligne,colonne,joueur,bateaucoule;
    cellule testcell;

    srand(time(NULL));

    flotte armee_j[2];

    for (i=0;i<2;i++){
        armee_j[i].etat=vivant;
        for (j=0;j<NBBAT-1;j++){
            crea_bateau(&armee_j[i].num_bateau[j]);
            armee_j[i].num_bateau[j].etat=vivant;
        }
    }

    printf("--- Bataille navale ---\n");

    joueur=0;

    do{
        colonne=-1;
        ligne=-1;

        if(joueur==0){
            printf("----Joueur 1----\n");
        }else{
            printf("----Joueur 2----\n");
        }

        do{ //le joueur donne la ligne et la colonne du tir
            printf("Numero de la oolonne ? (de 1 a %i)  : ",TAILLEGRILLE);
            scanf("%i",&colonne);
        }while (colonne>TAILLEGRILLE || colonne<1);


        do{
            printf("Numero de la ligne ? (de 1 a %i ) : ",TAILLEGRILLE);
            scanf("%i",&ligne);
        }while (ligne>TAILLEGRILLE || ligne<1);

        testcell.colonne=colonne-1;
        testcell.ligne=ligne-1;

        if (joueur==0){
            if (app_flotte(testcell,armee_j[1])==True){ //une cellule de la flotte a ete touchee
                for (i=0;i<NBBAT-1;i++){
                    if(app_bateau(testcell,armee_j[1].num_bateau[i])==True){
                        for(j=0;j<TAILLEBAT-1;j++){
                            if (compare_cellule(testcell,armee_j[1].num_bateau[i].partie_bateau[j])==True){
                            armee_j[1].num_bateau[i].partie_bateau[j].colonne=-1; //changement valeurs de la cellule touchee
                            armee_j[1].num_bateau[i].partie_bateau[j].ligne=-1;
                            }
                        }
                        // le bateau touche est il detruit ?
                        if (verif_bateau_detr(armee_j[0],i)){
                            armee_j[1].num_bateau[i].etat=mort;
                        }
                    }
                }

                //la flotte a ete detruite ?
                if (verif_flotte_detr(armee_j[1])==True){
                    armee_j[1].etat=mort;
                }

                printf(" -----> TOUCHE ! (ligne %i, colonne %i)\n\n",ligne,colonne);
            }else{ //la flotte n a pas ete touchee
                printf(" -----> Rate (ligne %i, colonne %i)\n\n",ligne,colonne);
            }

        }else if(joueur==1){
            if (app_flotte(testcell,armee_j[0])){ //un bateau a ete touche
                for (i=0;i<NBBAT-1;i++){
                    if(app_bateau(testcell,armee_j[0].num_bateau[i])==True){
                        for(j=0;j<TAILLEBAT-1;j++){
                            if (compare_cellule(testcell,armee_j[0].num_bateau[i].partie_bateau[j])==True){
                                armee_j[0].num_bateau[i].partie_bateau[j].colonne=-1; //changement valeurs de la cellule touchee
                                armee_j[0].num_bateau[i].partie_bateau[j].ligne=-1;
                            }
                        }
                        //le bateau touche a t-il ete detruit ?
                        if (verif_bateau_detr(armee_j[0],i)){
                            armee_j[0].num_bateau[i].etat=mort;
                        }
                    }
                }

                //la flotte a ete detruite ?
                if (verif_flotte_detr(armee_j[0])){
                    armee_j[0].etat=mort;
                }

                printf("TOUCHE !\n\n");
            }else{
                printf("Rate\n\n");
            }
        }

        if (joueur==1){ //changement de joueur
            joueur=0;
        }else if(joueur==0){
            joueur=1;
        }

    }while (armee_j[0].etat==vivant && armee_j[1].etat==vivant);

    printf("\n");

    if (armee_j[0].etat==vivant){
        printf("Joueur 1 gagne !");
    }else{
        printf("Joueur 2 gagne !");
    }

    return 0;
}




void crea_bateau(bateau *b){
/*
BUT : Donner une ligne et une colonne pour chaque partie d'un bateau (comprises entre 0 et 4)
ENTREE : Un bateau
SORTIE : Les cellules du bateau avec des valeurs de ligne et colonne comprises dans la grille
*/

    int i,sens,aleat;

    sens = rand() % 2; //determine si le bateau sera place horizontalement ou verticalement

    if (sens==horizontal){ // meme ligne

        aleat=rand () % 5;
        for (i=0;i<TAILLEBAT;i++){
            b->partie_bateau[i].ligne=aleat;
        }

        aleat=rand () % 3; //voir doc cplusplus
        for (i=0;i<TAILLEBAT;i++){
            b->partie_bateau[i].colonne=aleat+i;
        }

    }else if(sens==vertical){ //meme colonne
        aleat=rand () % 3;
        for (i=0;i<TAILLEBAT;i++){
            b->partie_bateau[i].ligne=aleat+i;
        }

        aleat=rand () % 5;
        for (i=0;i<TAILLEBAT;i++){
            b->partie_bateau[i].colonne=aleat;
        }
    }
}

int compare_cellule(cellule c1, cellule c2){
/*
BUT : Verifier si deux cellules sont les memes
ENTREE : 2 cellules
SORTIE : Vrai si les cellules ont des coordonnees identiques, Faux sinon (valeurs issues d'une enum)
*/

    if (c1.ligne==c2.ligne && c1.colonne==c2.colonne){
        return True;
    }else{
        return False;
    }
}

int app_bateau(cellule celltest, bateau bat){
/*
BUT : Verifier si une cellule appartient a un bateau
ENTREE : Une cellule et un bateau
SORTIE : Vrai si la cellule a tester a les memes valeurs qu'une cellule du bateau, Faux sinon (valeurs issues d'une enum)
*/
    int i,res;

    res=False;

    for (i=0;i<TAILLEBAT;i++){
        if (compare_cellule(celltest,bat.partie_bateau[i])==True){
            res=True;
            break;
        }
    }

    return res;
}

int app_flotte(cellule celltest,flotte f){
/*
BUT : Dire si une cellule appartient a une flotte
ENTREE : Une cellule et une flotte
SORTIE : Vrai si la cellule a tester a les memes valeurs qu'une cellule d'un bateau de la flotte (=appartient à la flotte), Faux sinon (issu d'une enum)
*/

    int i,res;

    res=False;

    for (i=0;i<NBBAT;i++){
        if (app_bateau(celltest,f.num_bateau[i])==True){
            res=True;
            break;
        }
    }

    return res;
}

int verif_flotte_detr(flotte f){
/*
BUT : Permet de voir si la flotte passee en parametre est detruite
ENTREE : Une flotte
SORTIE : True si la flotte est detruite, False sinon
*/
    int i,flotte_detr;

    flotte_detr=True;

    for (i=0;i<NBBAT;i++){
        if (f.num_bateau[i].etat==vivant){
        flotte_detr=False;
        break;
        }
    }

    return flotte_detr;
}

int verif_bateau_detr(flotte armee_j, int placebat){
/*
BUT : Permet de voir si un bateau est detruit ou non
ENTREE : La flotte du bateau, la position du bateau dans la flotte
SORTIE : True si le bateu est coule, False sinon
*/
    int  i,bateaucoule;

    bateaucoule=True;

    for(i=0;i<TAILLEBAT-1;i++){
        if (armee_j.num_bateau[placebat].partie_bateau[i].colonne!=-1 && armee_j.num_bateau[placebat].partie_bateau[i].ligne!=-1){
            bateaucoule=False;
            break;
        }
    }

    return bateaucoule;
}

/*
BUT :
ENTREE :
SORTIE :
*/
