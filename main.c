/* Date de création : 02/10/2013 */
/* Date de la dernière modification : 05/10/2013 */
/* Commentaire : Petit jeu semblable a super mario mais avec un personnage différent (naruto) */
/* Version 2 : Que des changements au niveau du code source */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "const.h"
#include "jeu.h"

int main (int argc, char *argv[])
{
/* SDL déclaration */
SDL_Surface *ecran=NULL, *menu=NULL, *instruction=NULL, *flag=NULL;
SDL_Rect pos_menu, pos_flag;
SDL_Event event;
int continuer=1, afficher_instruction=0, t_ctrl=0, choix_menu=0; /* 0 içi c'est le premier choix (Jouer) du menu */
/* SDL mixer déclaration */
Mix_Music *intro_music=NULL;
/* SDL initialisation */
SDL_Init(SDL_INIT_EVERYTHING);
   /* Image de l'application */
   SDL_WM_SetIcon(IMG_Load("naru_res\\icone.png"), NULL);
   /* Configuration de l'ecran */
   ecran=SDL_SetVideoMode(FENETRE_L, FENETRE_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
   /* Nom de l'application */
   SDL_WM_SetCaption("Super Naruto", NULL);
   /* Chargement des images */
      /* Menu */
      menu=IMG_Load("naru_res\\menu.jpg");
      /* Instruction */
      instruction=IMG_Load("naru_res\\instruction.jpg");
      /* flag */
      flag=IMG_Load("naru_res\\akatsuki_flag.png");
         /* Transparence du flag */
         SDL_SetColorKey(flag, SDL_SRCCOLORKEY, SDL_MapRGB(flag->format, 0, 255, 0));
   /* Positionnement */
   pos_menu.x=0;
   pos_menu.y=0;
   pos_flag.x=250-flag->w;
   pos_flag.y=235-flag->h;
/* SDL mixer initialisation */
Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
   /* Chargement de la musique d'entrée */
   intro_music=Mix_LoadMUS("naru_res\\intro1.wav");
while (continuer)
{
SDL_WaitEvent(&event);
   switch (event.type)
   {
      /* Fermeture de l'application */
      case SDL_QUIT: continuer=0; break;
      /* Gestion des évenements clavier */  
      case SDL_KEYDOWN: /* Si touche enfoncée */
            switch(event.key.keysym.sym)
            {
               case SDLK_UP: pos_flag.x=250-flag->w;
                             pos_flag.y=235-flag->h;
                             choix_menu=0;
                             break;
               case SDLK_DOWN: pos_flag.x=250-flag->w;
                               pos_flag.y=295-flag->h;
                               choix_menu=1;
                               break;
               case SDLK_RETURN: if (choix_menu) /* Si choix_menu==1 (Quitter) */ 
                                    continuer=0; /* On quitte le programme */
                                 else
                                    afficher_instruction=1; /* On affiche les instructions */
                                 break;
               case SDLK_LCTRL: if (afficher_instruction) /* Si touche CTRL GAUCHE enfoncée */
                                {   
                                   afficher_instruction=0; 
                                   jouer(ecran); /* On commence le jeu */
                                }
                                break; 
               case SDLK_RCTRL: if (afficher_instruction) /* Si touche CTRL DROIT enfoncée */
                                {   
                                   afficher_instruction=0; 
                                   jouer(ecran); /* On commence le jeu */
                                }
                                break; 
            }
      break; /* sortie de SDL_KEYDOWN */
      case SDL_KEYUP: /* Si touche relachée */
         if (t_ctrl) t_ctrl=0; 
      break; /* sortie de SDL_KEYUP */
   }
   /* Blit */
       /* Instruction */
      if (afficher_instruction)
         SDL_BlitSurface(instruction, NULL, ecran, &pos_menu);
      else
      {
      /* Menu */
      SDL_BlitSurface(menu, NULL, ecran, &pos_menu);
      /* Akatsuki_flag (pour faire un choix dans le menu) */
      SDL_BlitSurface(flag, NULL, ecran, &pos_flag);
      }
   /* Mise a jour de l'écran */
   SDL_Flip(ecran);
   /* On joue le son */
   if (!Mix_PlayingMusic())
      Mix_PlayMusic(intro_music, -1);
}
/* Vidage SDL mixer */
Mix_FreeMusic(intro_music);
Mix_CloseAudio();
/* Vidage SDL (surfaces) */
SDL_FreeSurface(menu);
SDL_FreeSurface(instruction);
SDL_FreeSurface(flag);
SDL_Quit();
return EXIT_SUCCESS;    
}
