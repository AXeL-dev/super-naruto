#ifndef _JEU_H
#define _JEU_H

/* Prototypes */
void jouer(SDL_Surface *ecran);
void score_plus_plus(int *score, char *score_texte);
void reduire_vitesse_boucle(int *temp_precedent);

#endif
