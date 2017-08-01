#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "const.h"
#include "jeu.h"


/* Fonction qui lance le jeu */
void jouer(SDL_Surface *ecran)
{
/* SDL déclaration */
SDL_Surface *map[N_MAP]={NULL}, *barre_vie=NULL, *naruto[NARUTO_SPRITES]={NULL}, *ennemie[ENNEMIE_SPRITES]={NULL}, *kunai[ARMES_SPRITES]={NULL}, *autre_images[N_AUTRE_IMAGES]={NULL};
SDL_Rect pos_map, pos_barre_vie, pos_naruto, pos_ennemie1,/* pos_ennemie2,*/ pos_kunai, pos_a_i_brique, pos_a_i_msg, pos_blit_kunai;
SDL_Event event;
int jouer=1, wait_event=0, temp_precedent=0, score=0, i=0, statut_naruto=DROITE, son=1, saut=0, run=0, lancer_kunai=0, attaque=0, effet_sonore[N_EFFET_S]={0}, arme_actuelle=KUNAI1, kunai_a_lancer=DROITE, touche_ctrl=0, statut_ennemie=GAUCHE, nombre_ennemie=1, ennemie_tuer=0, vie_perdu=0;   
/* TTF déclaration */
SDL_Surface *afficher_score=NULL;
SDL_Rect pos_score;
char score_texte[3]="0";
SDL_Color couleur_blanche={255, 255, 255};
TTF_Font *police=NULL;
/* SDL mixer déclaration */
   /* Music à jouer */
   Mix_Music *music[2]={NULL};
   /* Effets sonores */
   Mix_Chunk *punch=NULL; /* punch = taper/donner un coup en français :p */
   Mix_Chunk *throw_kunai=NULL; /* throw = lancer */
/* SDL initialisation */
   /* Chargement des images */
      /* Map */
      map[0]=IMG_Load("naru_res\\map1.jpg");
      //map[1]=IMG_Load("naru_res\\map2.jpg");
      /* Barre_vie (On crée et on colorie la barre en vert) */
      barre_vie=SDL_CreateRGBSurface(SDL_HWSURFACE, BARRE_VIE_L, BARRE_VIE_H, 32, 0, 0, 0, 0);
      SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 0, 255, 0));
      /* Sprites naruto */
      naruto[GAUCHE]=IMG_Load("naru_res\\naruto_g.png");
      naruto[DROITE]=IMG_Load("naru_res\\naruto_d.png");
      naruto[BAS_GAUCHE]=IMG_Load("naru_res\\naruto_b_g.png");
      naruto[BAS_DROITE]=IMG_Load("naru_res\\naruto_b_d.png");
      naruto[SAUT_GAUCHE]=IMG_Load("naru_res\\naruto_saut_g.png");
      naruto[SAUT_DROITE]=IMG_Load("naru_res\\naruto_saut_d.png");
      naruto[RUN_GAUCHE]=IMG_Load("naru_res\\naruto_run_g.png");
      naruto[RUN_DROITE]=IMG_Load("naru_res\\naruto_run_d.png");
      naruto[ATTAQUE_GAUCHE]=IMG_Load("naru_res\\naruto_atq_g.png");
      naruto[ATTAQUE_DROITE]=IMG_Load("naru_res\\naruto_atq_d.png");
      naruto[TOUCHER_GAUCHE]=IMG_Load("naru_res\\naruto_toucher_g.png");
      naruto[TOUCHER_DROITE]=IMG_Load("naru_res\\naruto_toucher_d.png");
      naruto[FAIL]=IMG_Load("naru_res\\naruto_fail.png");
      naruto[WIN]=IMG_Load("naru_res\\naruto_win.png");
      /* Sprites ennemie */
      ennemie[GAUCHE]=IMG_Load("naru_res\\ennemie_g.png");
      ennemie[DROITE]=IMG_Load("naru_res\\ennemie_d.png");
      ennemie[DISPARAITRE]=IMG_Load("naru_res\\ennemie_disp.png");
      /* Sprites kunai (armes) */
      kunai[GAUCHE]=IMG_Load("naru_res\\kunai_g.png");
      kunai[DROITE]=IMG_Load("naru_res\\kunai_d.png");
      kunai[MAIN]=IMG_Load("naru_res\\main.png");
      kunai[KUNAI2]=IMG_Load("naru_res\\kunai2.png");
      kunai[KUNAI3]=IMG_Load("naru_res\\kunai3.png");
      /* autre_images */
      autre_images[BRIQUE]=IMG_Load("naru_res\\brique.png");
      autre_images[FAIL_MSG]=IMG_Load("naru_res\\fail_msg.png");
      autre_images[WIN_MSG]=IMG_Load("naru_res\\win_msg.png");
   /* Transparence des sprites */
   for (i=0; i<NARUTO_SPRITES; i++)
   {
      /* On enleve la couleur verte des sprites */
      SDL_SetColorKey(naruto[i], SDL_SRCCOLORKEY, SDL_MapRGB(naruto[i]->format, 0, 255, 0));
      if (i<ENNEMIE_SPRITES) SDL_SetColorKey(ennemie[i], SDL_SRCCOLORKEY, SDL_MapRGB(ennemie[i]->format, 0, 255, 0));
      if (i<ARMES_SPRITES) SDL_SetColorKey(kunai[i], SDL_SRCCOLORKEY, SDL_MapRGB(kunai[i]->format, 0, 255, 0));
      if (i<N_AUTRE_IMAGES) SDL_SetColorKey(autre_images[i], SDL_SRCCOLORKEY, SDL_MapRGB(autre_images[i]->format, 0, 255, 0));   
   }
   /* Positionnement */
   pos_map.x=0;
   pos_map.y=0;
   pos_barre_vie.x=FENETRE_L/2-barre_vie->w/2;
   pos_barre_vie.y=20;
   pos_naruto.x=10;
   pos_naruto.y=SAUT_MIN;
   pos_ennemie1.x=FENETRE_L-ennemie[GAUCHE]->w;
   pos_ennemie1.y=SAUT_MIN+2;
   pos_kunai.x=0;
   pos_kunai.y=0;
   pos_blit_kunai.x=10;
   pos_blit_kunai.y=10;
   pos_a_i_brique.x=ecran->w/2-autre_images[BRIQUE]->w/2;
   pos_a_i_brique.y=SAUT_MAX+autre_images[BRIQUE]->h*3-7;
   pos_a_i_msg.x=ecran->w/2-autre_images[WIN_MSG]->w/2;
   pos_a_i_msg.y=ecran->h/2-autre_images[WIN_MSG]->h/2;
/* TTF initialisation */
TTF_Init();
   /* Police */
   police=TTF_OpenFont("naru_res\\arialbd.ttf", 20);
   /* Positionnement */
   pos_score.x=FENETRE_L-30;
   pos_score.y=10;
   /* Score (surface) */
   afficher_score=TTF_RenderText_Blended(police, score_texte, couleur_blanche);
/* SDL mixer initialisation */
   /* Chargement des musiques */
   music[0]=Mix_LoadMUS("naru_res\\intro2.wav");
   music[1]=Mix_LoadMUS("naru_res\\intro3.wav");
   /* Chargement des effets sonores */
   punch=Mix_LoadWAV("naru_res\\punch.wav");
   throw_kunai=Mix_LoadWAV("naru_res\\throw.wav");
/* Activation de la répétition des touches clavier */
SDL_EnableKeyRepeat(10, 10);
/* Gestion d'événements */
while (jouer)
{
if (!wait_event)
{
   SDL_PollEvent(&event);
   reduire_vitesse_boucle(&temp_precedent);
}
else
   SDL_WaitEvent(&event);
   switch (event.type)
   {
      /* Fermeture de l'application */
      case SDL_QUIT: jouer=0; break;
      /* Gestion des évenements clavier + collisions avec la brique */  
      case SDL_KEYDOWN: /* Si touche enfoncée */
         switch(event.key.keysym.sym)
         {
            case SDLK_UP: if (jouer!=FIN)
                          {
                             if (pos_naruto.y>=SAUT_MAX)
                             {
                                /* Si naruto n'est pas au dessus de la brique (n'est pas dans le champ de la brique) */
                                if (pos_naruto.x<pos_a_i_brique.x-naruto[DROITE]->w+BRIQUE_NARUTO_DIF || pos_naruto.x>pos_a_i_brique.x+autre_images[BRIQUE]->w-BRIQUE_NARUTO_DIF)
                                {
                                   pos_naruto.y-=SAUT_PIX;
                                   if (statut_naruto==DROITE || statut_naruto==BAS_DROITE || statut_naruto==TOUCHER_DROITE)
                                      statut_naruto=SAUT_DROITE;
                                   else if (statut_naruto==GAUCHE || statut_naruto==BAS_GAUCHE || statut_naruto==TOUCHER_GAUCHE)
                                      statut_naruto=SAUT_GAUCHE;
                                   saut=1;
                                }
                                else /* Si nn */
                                {
                                   /* Si naruto est au dessous de la brique et qu'il ne dépasse pas la largueur de la brique */
                                   if (pos_naruto.y>=pos_a_i_brique.y+autre_images[BRIQUE]->h)
                                   {
                                      pos_naruto.y-=SAUT_PIX;
                                      if (statut_naruto==DROITE || statut_naruto==BAS_DROITE || statut_naruto==TOUCHER_DROITE)
                                         statut_naruto=SAUT_DROITE;
                                      else if (statut_naruto==GAUCHE || statut_naruto==BAS_GAUCHE || statut_naruto==TOUCHER_GAUCHE)
                                         statut_naruto=SAUT_GAUCHE;
                                      saut=1;
                                   }
                                   /* Si nn si on est au dessus de la brique et qu'on veut sauter (A faire apres :p, chui fatigué) */
                                   /* Par contre SAUT_MAX est une constante, il faut prendre en compte ça ! */
                               }
                            }
                         }
                         break; 
            case SDLK_DOWN: if (jouer!=FIN)
                            {
                               if (statut_naruto==DROITE || statut_naruto==TOUCHER_DROITE || statut_naruto==SAUT_DROITE)
                                   statut_naruto=BAS_DROITE;
                               else if (statut_naruto==GAUCHE || statut_naruto==TOUCHER_GAUCHE || statut_naruto==SAUT_GAUCHE)
                                   statut_naruto=BAS_GAUCHE;
                               /* Si naruto n'est pas a terre */
                               if (pos_naruto.y<SAUT_MIN)
                               { 
                                  /* Si naruto n'est pas au dessus de la brique ni dans le champ de la brique */
                                  if (pos_naruto.x<pos_a_i_brique.x-naruto[DROITE]->w+BRIQUE_NARUTO_DIF || pos_naruto.x>pos_a_i_brique.x+autre_images[BRIQUE]->w-BRIQUE_NARUTO_DIF)
                                     pos_naruto.y+=SAUT_PIX;
                               }
                            }
                            break;
            case SDLK_LEFT: if (jouer!=FIN) 
                            {
                               /* Si naruto ne dépasse pas l'ecran d'agauche */
                               if (pos_naruto.x>10)
                               {
                                  /* Si naruto est a terre ou au dessus de la brique */
                                  if (pos_naruto.y==SAUT_MIN || pos_naruto.y+naruto[RUN_GAUCHE]->h<=pos_a_i_brique.y+L_R_BRIQUE_DIF)
                                  {
                                     pos_naruto.x-=SAUT_PIX;
                                     statut_naruto=RUN_GAUCHE;
                                     run=1;
                                  }
                               }
                            }
                            break;
            case SDLK_RIGHT: if (jouer!=FIN) 
                             {
                                /* Si naruto ne dépasse pas l'ecran d'adroite */
                                if (pos_naruto.x<(FENETRE_L-naruto[DROITE]->w))
                                {
                                   /* Si naruto est a terre ou au dessus de la brique */
                                   if (pos_naruto.y==SAUT_MIN || pos_naruto.y+naruto[RUN_GAUCHE]->h<=pos_a_i_brique.y+L_R_BRIQUE_DIF)
                                   {
                                      pos_naruto.x+=SAUT_PIX;
                                      statut_naruto=RUN_DROITE;
                                      run=1;
                                   }
                                }
                             }
                             break;
            case SDLK_SPACE: if (!lancer_kunai && jouer!=FIN) 
                             {
                                if (statut_naruto==DROITE || statut_naruto==BAS_DROITE || statut_naruto==SAUT_DROITE || statut_naruto==RUN_DROITE || statut_naruto==TOUCHER_DROITE)
                                {
                                   if (arme_actuelle==KUNAI1)
                                      kunai_a_lancer=DROITE;
                                   else if (arme_actuelle==MAIN)
                                   {
                                      statut_naruto=ATTAQUE_DROITE;
                                      kunai_a_lancer=arme_actuelle;
                                   }
                                   else
                                      kunai_a_lancer=arme_actuelle;
                                   pos_kunai.x=pos_naruto.x+naruto[DROITE]->w/2;
                                   if (statut_naruto==TOUCHER_DROITE) statut_naruto=DROITE; /* Si naruto a été touché par l'ennemie d'adroite */
                                }
                                else if (statut_naruto==GAUCHE || statut_naruto==BAS_GAUCHE || statut_naruto==SAUT_GAUCHE || statut_naruto==RUN_GAUCHE || statut_naruto==TOUCHER_GAUCHE)
                                {
                                   if (arme_actuelle==KUNAI1)
                                      kunai_a_lancer=GAUCHE;
                                   else if (arme_actuelle==MAIN)
                                   {
                                      statut_naruto=ATTAQUE_GAUCHE;
                                      kunai_a_lancer=arme_actuelle;
                                   }
                                   else
                                      kunai_a_lancer=arme_actuelle;
                                   pos_kunai.x=pos_naruto.x-3;
                                   if (statut_naruto==TOUCHER_GAUCHE) statut_naruto=GAUCHE; /* Si naruto a été touché par l'ennemie d'agauche */
                                }
                                pos_kunai.y=pos_naruto.y+naruto[DROITE]->h/4;
                                if (kunai_a_lancer!=MAIN) /* Si l'arme actuelle n'est pas la main */
                                {
                                   effet_sonore[SON_KUNAI]=1; /* On joue le 1er effet_sonore */
                                   lancer_kunai=1;
                                }
                                else
                                {
                                   effet_sonore[SON_PUNCH]=1;
                                   attaque=1;
                                }
                             }
                             break;
            case SDLK_a: if (touche_ctrl && jouer!=FIN) 
                         {
                         if (arme_actuelle<ARMES_SPRITES-1)
                            arme_actuelle++;
                         else
                            arme_actuelle=KUNAI1;
                         touche_ctrl=0;
                         }
                         break;
            case SDLK_LCTRL: if (jouer==FIN) /* Si touche CTRL GAUCHE enfoncée */
                                jouer=0;
                             else
                                touche_ctrl=1;
                             break; 
            case SDLK_RCTRL: if (jouer==FIN) /* Si touche CTRL DROIT enfoncée */
                                jouer=0;
                             else
                                 touche_ctrl=1;
                             break;   
         }
      break; /* sortie de SDL_KEYDOWN */
      case SDL_KEYUP: /* Si touche relachée */
      /* On remet la valeur de la variable liée au saut de naruto a zéro */
         if (saut) saut=0;
         if (touche_ctrl) touche_ctrl=0; 
      break; /* sortie de SDL_KEYUP */
   }
   /* Blit */
      /* Map */
      SDL_BlitSurface(map[0], NULL, ecran, &pos_map);
      /* Brique */
      SDL_BlitSurface(autre_images[BRIQUE], NULL, ecran, &pos_a_i_brique);
      /* Kunai */
      SDL_BlitSurface(kunai[arme_actuelle], NULL, ecran, &pos_blit_kunai);
      /* Barre de vie */
         /* Couleur de la barre */
         if (barre_vie->w>=BARRE_VIE_L/2 && barre_vie->w<BARRE_VIE_L-50)
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 128, 0));
         else if (barre_vie->w<BARRE_VIE_L/2)
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 0, 0));
         else
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 0, 255, 0));
      SDL_BlitSurface(barre_vie, NULL, ecran, &pos_barre_vie);
      /* Score */
      SDL_BlitSurface(afficher_score, NULL, ecran, &pos_score);
      /* Naruto */
      /* Gestion des collisions entre naruto et l'ennemie */
         /* Si naruto se met a travers de l'ennemie */
         if (!ennemie_tuer && (pos_naruto.x==pos_ennemie1.x || pos_naruto.x==pos_ennemie1.x+5 || pos_ennemie1.x==pos_naruto.x+5) && (pos_naruto.y+2==pos_ennemie1.y || pos_naruto.y+10==pos_ennemie1.y || pos_ennemie1.y==pos_naruto.y+10))
         {
            /* Si naruto est a gauche */
            if (statut_naruto==GAUCHE || statut_naruto==BAS_GAUCHE || statut_naruto==SAUT_GAUCHE || statut_naruto==RUN_GAUCHE || statut_naruto==TOUCHER_DROITE)
            {
               if (statut_ennemie==GAUCHE) /* Si l'ennemie attaque d'agauche */
                  statut_naruto=TOUCHER_DROITE;
               else
                  statut_naruto=TOUCHER_GAUCHE;
                  
            }
            /* Si nn si naruto est a droite */
            else if (statut_naruto==DROITE || statut_naruto==BAS_DROITE || statut_naruto==SAUT_DROITE || statut_naruto==RUN_DROITE || statut_naruto==TOUCHER_GAUCHE)
            {
               if (statut_ennemie==DROITE) /* Si l'ennemie attaque d'adroite */
                  statut_naruto=TOUCHER_GAUCHE;
               else
                  statut_naruto=TOUCHER_DROITE;
            }
            if (vie_perdu<300) /* Si on a pas perdu toute la vie on diminue la barre de vie */
            { 
               vie_perdu+=50;
               /* On modifie la barre de vie */
               barre_vie=SDL_CreateRGBSurface(SDL_HWSURFACE, BARRE_VIE_L-vie_perdu, BARRE_VIE_H, 32, 0, 0, 0, 0);
               /* On joue l'effet sonore punch */
               effet_sonore[SON_PUNCH]=1;
               if (vie_perdu==300) /* Si toute la vie perdu */
               {
                  pos_naruto.y+=10; 
                  statut_naruto=FAIL;
                  ennemie_tuer=1;
                  son=2;
                  jouer=FIN; /* Fin du jeu */
               }
            }
         }
      SDL_BlitSurface(naruto[statut_naruto], NULL, ecran, &pos_naruto);
      /* Ennemie */
      if (nombre_ennemie==1 && !ennemie_tuer)
         SDL_BlitSurface(ennemie[statut_ennemie], NULL, ecran, &pos_ennemie1);
      //else if (nombre_ennemie==2 && !ennemie_tuer[1])
         //SDL_BlitSurface(ennemie[statut_ennemie], NULL, ecran, &pos_ennemie2);
      /* Message de fin de jeu */
      if (jouer==FIN)
      {
         if (score==20) /* Si partie gagné c'est que le score==20 */
            SDL_BlitSurface(autre_images[WIN_MSG], NULL, ecran, &pos_a_i_msg);
         else /* Si nn , ça veux dire qu'on a perdu */
            SDL_BlitSurface(autre_images[FAIL_MSG], NULL, ecran, &pos_a_i_msg);
      }
/* Gestion des mouvements + collisions d'attaque avec kunai ou main... */
   /* Mouvements ennemies */
      /* Ennemie1 */
      if (!ennemie_tuer)
      {
         /* Si kunai lancer */
         if (lancer_kunai && pos_ennemie1.x-pos_kunai.x<=KUNAI_ENNEMIE_DIF && pos_ennemie1.x-pos_kunai.x>=-KUNAI_ENNEMIE_DIF && pos_kunai.y-pos_ennemie1.y<=POS_DIF && pos_kunai.y-pos_ennemie1.y>=-POS_DIF)
         {
            SDL_BlitSurface(ennemie[DISPARAITRE], NULL, ecran, &pos_ennemie1);
            ennemie_tuer=1;
            lancer_kunai=0;
            score_plus_plus(&score, score_texte);
            afficher_score=TTF_RenderText_Blended(police, score_texte, couleur_blanche);
            //fprintf(stderr, "%d, %d, %d\n", pos_ennemie1.y, pos_kunai.y, pos_kunai.y-pos_ennemie1.y);
         }
         /* Si naruto attaque a la main */
         else if (attaque && ((statut_ennemie==GAUCHE && statut_naruto==ATTAQUE_DROITE) ||  (statut_ennemie==DROITE && statut_naruto==ATTAQUE_GAUCHE)) && pos_ennemie1.x-pos_naruto.x<=ATQ_DIF && pos_ennemie1.x-pos_naruto.x>=-ATQ_DIF && pos_naruto.y+2==pos_ennemie1.y) /* +2 pixel (car pos_ennemie1.y=SAUT_MIN+2) */
         {
            SDL_BlitSurface(ennemie[DISPARAITRE], NULL, ecran, &pos_ennemie1);
            ennemie_tuer=1;
            score_plus_plus(&score, score_texte);
            afficher_score=TTF_RenderText_Blended(police, score_texte, couleur_blanche);
         }
         /* Si l'ennemie ne dépasse pas l'ecran de la gauche */
         else if (pos_ennemie1.x>0 && statut_ennemie==GAUCHE)
         {
            pos_ennemie1.x-=VITESSE_ENNEMIE;
            statut_ennemie=GAUCHE;
         }
         /* Si nn (si l'ennemie dépasse l'ecran de la gauche) */
         else 
         {  
            /* Si l'ennemie ne dépasse pas l'ecran de la droite */
            if (pos_ennemie1.x<FENETRE_L-ennemie[GAUCHE]->w)
            {
               pos_ennemie1.x+=VITESSE_ENNEMIE;
               statut_ennemie=DROITE; 
            }
            /* Si l'ennemie dépasse l'ecran de la droite */
            else
            {
               pos_ennemie1.x-=VITESSE_ENNEMIE;
               statut_ennemie=GAUCHE;
            }
         } 
      }
      else /* Si ennemie tué */
      {
         if (jouer && jouer!=FIN) /* Si jeu pas encore fini */
         { 
            if (score<20) /* si score(d'ennemies tués) < 20 */
            {
               ennemie_tuer=0;
               statut_ennemie=GAUCHE;
               pos_ennemie1.x=FENETRE_L-ennemie[GAUCHE]->w;
            }
            else if (score==20) /* Si score égal à 20 (20 ennemies tués) */
            {
               statut_naruto=WIN;
               pos_naruto.y-=30;
               son=2;
               jouer=FIN; /* Fin du jeu */
            }
         }
      }
   /* Mouvements naruto */
      /* Si kunai (arme) lancée */
      if (lancer_kunai) 
      {
         if ((statut_naruto==DROITE || statut_naruto==BAS_DROITE || statut_naruto==SAUT_DROITE) && (kunai_a_lancer==DROITE || kunai_a_lancer==KUNAI2 || kunai_a_lancer==KUNAI3) && pos_kunai.x<FENETRE_L)
            pos_kunai.x+=VITESSE_KUNAI;
         else if ((statut_naruto==GAUCHE || statut_naruto==BAS_GAUCHE || statut_naruto==SAUT_GAUCHE) && (kunai_a_lancer==GAUCHE || kunai_a_lancer==KUNAI2 || kunai_a_lancer==KUNAI3) && pos_kunai.x>0)
            pos_kunai.x-=VITESSE_KUNAI;
         else 
            lancer_kunai=0;
         /* Blit du kunai */
         SDL_BlitSurface(kunai[kunai_a_lancer], NULL, ecran, &pos_kunai);
      }
      /* On fait descendre naruto quand il saute */
      if (jouer!=FIN && !saut && pos_naruto.y<SAUT_MIN)
      {       
         /* Si naruto n'est pas au dessus de la brique (n'est pas dans le champ de la brique) */            
         if (pos_naruto.x<pos_a_i_brique.x-naruto[DROITE]->w+BRIQUE_NARUTO_DIF || pos_naruto.x>pos_a_i_brique.x+autre_images[BRIQUE]->w-BRIQUE_NARUTO_DIF)
         {
            pos_naruto.y+=SAUT_PIX;
            if (pos_naruto.y==SAUT_MIN)
            { 
               if (statut_naruto==SAUT_DROITE)
                  statut_naruto=DROITE;
               else if (statut_naruto==SAUT_GAUCHE)
                  statut_naruto=GAUCHE;                         
            }
         }
         else /* Si nn (s'il est dans le champ de la brique) */
         {
             /* Si naruto saute (il ne va pas dépasser le dessous de la brique biensur) */
             if (pos_naruto.y>pos_a_i_brique.y && pos_naruto.y<SAUT_MIN)
             {
                /* On le fait descendre */
                pos_naruto.y+=SAUT_PIX;
                if (pos_naruto.y==SAUT_MIN)
                { 
                   if (statut_naruto==SAUT_DROITE)
                      statut_naruto=DROITE;
                   else if (statut_naruto==SAUT_GAUCHE)
                      statut_naruto=GAUCHE;                         
                }
             }
         }
      }
      /* On rend naruto a son etat initial si touche droite ou gauche enfoncé */
      if (run) /* @ ne pas changer en (!run) */
      {
         if (statut_naruto==RUN_DROITE)
            statut_naruto=DROITE;
         else if (statut_naruto==RUN_GAUCHE)
            statut_naruto=GAUCHE;  
         run=0; /* l'avantage de remettre la valeur de run a 0 içi non pas dans SDL_KEYUP, et que meme si naruto essaye de lancer une kunai en courant son statut va changer et si on clic sur la touche bas en courant naruto va descendre non pas rester en statut courant */
      }
       /* Si on a attaqué a la main on rend le statut de naruto a droite ou a gauche */
      if (attaque)
      {
         if (statut_naruto==ATTAQUE_DROITE)
             statut_naruto=DROITE;
         else if (statut_naruto==ATTAQUE_GAUCHE)
             statut_naruto=GAUCHE;
         attaque=0;
      }
   /* Mise a jour de l'écran */
   SDL_Flip(ecran);
   /* On joue le son */
      /* Throw_kunai */
      if (effet_sonore[SON_KUNAI])
      {
         Mix_PlayChannel( -1, throw_kunai, 0 );
         effet_sonore[SON_KUNAI]=0;
      }
      /* Punch */
      else if (effet_sonore[SON_PUNCH])
      {
         Mix_PlayChannel( -1, punch, 0 );
         effet_sonore[SON_PUNCH]=0;
      }
      /* music 1 */
      if (son==1 && !Mix_PlayingMusic())
         Mix_PlayMusic(music[0], -1);
      /* music 2 (de fin) */
      else if (son==2)
      {
         Mix_HaltMusic();
         son=3;
      }
      else if (son==3 && !Mix_PlayingMusic())
      {
         Mix_PlayMusic(music[1], -1);
         son=4;
      }
      else if (son==4 && !Mix_PlayingMusic())
      {
         Mix_HaltMusic();
         wait_event=1;             
      }
}
/* Vidage SDL mixer */
for (i=0; i<2; i++)
   Mix_FreeMusic(music[i]);
Mix_FreeChunk(punch);
Mix_FreeChunk(throw_kunai);
/* Vidage TTF */
SDL_FreeSurface(afficher_score);
TTF_CloseFont(police);
TTF_Quit();
/* Vidage SDL (surfaces) */
SDL_FreeSurface(barre_vie);
for (i=0; i<NARUTO_SPRITES; i++)
{
   SDL_FreeSurface(naruto[i]);
   if (i<ENNEMIE_SPRITES) SDL_FreeSurface(ennemie[i]);
   if (i<ARMES_SPRITES) SDL_FreeSurface(kunai[i]);
   if (i<N_AUTRE_IMAGES) SDL_FreeSurface(autre_images[i]);
   if (i<N_MAP) SDL_FreeSurface(map[i]);
}
}


/* Fonction qui incrémente le score */
void score_plus_plus(int *score, char *score_texte)
{
/* Incrémentation */
(*score)++;
/* Conversion du résultat */
sprintf(score_texte, "%d", (*score));
}


/* Fonction qui réduit la vitesse de la boucle */
void reduire_vitesse_boucle(int *temp_precedent)
{
int temp_actuel;
/* Récupération du temp actuel */
temp_actuel=SDL_GetTicks();
/* Si le temp d'attente de la boucle achevé */
if (temp_actuel - (*temp_precedent) >= BOUCLE_WAIT_TIME)
   (*temp_precedent)=temp_actuel;
/* Si nn */
else 
   SDL_Delay(BOUCLE_WAIT_TIME - (temp_actuel - (*temp_precedent)));   
}
