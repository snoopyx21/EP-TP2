#ifndef PAQUET_H
#define PAQUET_H

#include "noeud.h"
#include "flux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct s_chemin
{
    int noeud_actuel;
    float date;
    //float date_sortie;
    int code;
    struct s_chemin *s;
    struct s_chemin *p;
} p_chemin, *Chemin;

typedef struct s_paquet
{
    int pid;
    int source_noeud;
    int dest_noeud;
    Chemin c;
    float temps_attente_files;
    float temps_chemin;
    float taille_paquet;
    struct s_paquet *s;
    struct s_paquet *p;
} p_paquet, *Paquet;

Paquet create_Paquet(Paquet paquet);
void update_Paquet(Trace trace, Data data, Node l_noeud, Paquet l_paquet, Flux l_flux);
void free_Paquet(Paquet paquet);
void printf_Paquet(Paquet paquet, int my_pid, int ** matrice_adjacence);


#endif