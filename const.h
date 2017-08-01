#ifndef _CONST_H
#define _CONST_H

/* Constantes */
#define BOUCLE_WAIT_TIME 25
#define FENETRE_H 400
#define FENETRE_L 650
#define BARRE_VIE_H 10
#define BARRE_VIE_L 300
#define NARUTO_SPRITES 14
#define ENNEMIE_SPRITES 3
#define ARMES_SPRITES 5
#define N_MAP 1
#define N_EFFET_S 2
#define N_AUTRE_IMAGES 3
#define VITESSE_KUNAI 15
#define VITESSE_ENNEMIE 3
#define SAUT_MAX 200
#define SAUT_MIN 280
#define SAUT_PIX 4
#define KUNAI_ENNEMIE_DIF 10
#define BRIQUE_NARUTO_DIF 15
#define POS_DIF 13 /* 295(pos_kunai.y)-282(pos_ennemie.y) = 13 pixel */
#define ATQ_DIF 30
#define L_R_BRIQUE_DIF 12 /* LEFT_RIGHT_BRIQUE_DIF (touche gauche/droite) */
enum{GAUCHE, DROITE, BAS_GAUCHE, BAS_DROITE, SAUT_GAUCHE, SAUT_DROITE, RUN_GAUCHE, RUN_DROITE, ATTAQUE_GAUCHE, ATTAQUE_DROITE, TOUCHER_GAUCHE, TOUCHER_DROITE, FAIL, WIN};
enum{DISPARAITRE=2, FIN};
enum{KUNAI1=1, MAIN, KUNAI2, KUNAI3};
enum{SON_KUNAI, SON_PUNCH};
enum{BRIQUE, FAIL_MSG, WIN_MSG};

#endif
