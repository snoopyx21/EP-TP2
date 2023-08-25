#ifndef FLUX_H
#define FLUX_H

#include "trace.h"

typedef struct s_flux
{
    short fid;
    float debut_flux;
    float fin_flux;
    int noeud_source;
    int noeud_dest;
    int duree_vie;
    int nb_paquets;
    int nb_paquets_traites;
    int nb_paquets_recus;
    int nb_paquets_emis;
    int nb_paquets_perdus;
    float delai_moyen_acheminement_paquet;
    float debit_moyen; /* somme taille paquet / temps flux */

    struct s_flux *p;
    struct s_flux *s;
} p_flux, *Flux;

Flux create_Flux(Flux flux);
void update_Flux(Flux flux, Trace trace, Data data);
void free_Flux(Flux f);
void printf_Flux(Flux f, int i);

#endif