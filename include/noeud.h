#ifndef NOEUD_H
#define NOEUD_H

#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct s_noeud
{
    int noeud;
    int nb_paquets_traites; /* nombre de paquets recus et qui sont passés par le noeud */
    int nb_paquets_recus;
    int nb_paquets_emis;
    int nb_paquets_perdus;
    int nb_paquets_passage; /* nombre de paquets qui passe par le noeud */
    int nb_paquets_file;    /* nombre de paquet en file d'attente */
    struct s_noeud *s;
    struct s_noeud *p;
} p_noeud, *Node;

Node create_Node(Node node);
void update_Node(Node node, Trace trace, Data data);
void free_Node(Node f);
void printf_Node(Node node, int my_node);
#endif