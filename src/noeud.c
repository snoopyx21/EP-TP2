#include "../include/noeud.h"

Node create_Node(Node node)
{
    node->noeud                     = 0;
    node->nb_paquets_traites        = 0;
    node->nb_paquets_recus          = 0;
    node->nb_paquets_emis           = 0;
    node->nb_paquets_perdus         = 0;
    node->nb_paquets_passage        = 0;
    node->nb_paquets_file           = 0;
    return node;
}

void update_Node(Node node, Trace trace, Data data)
{
    Node tmp;
    int trouve;
    
    trouve = 0;
    tmp = node->p;

    while( tmp != NULL)
    {
        /* recherche du noeud dans la liste chaînée des noeuds déjà enregistré */
        if (tmp->noeud == trace->pos)
        {
            /* traitement du code de la trace de simulation */
            if(trace->code == 0)
            {
                tmp->nb_paquets_emis++;
                tmp->nb_paquets_file++;
                tmp->nb_paquets_traites++;
            }
            if(trace->code == 1)
            {
                tmp->nb_paquets_passage++;
                tmp->nb_paquets_file++;
            }
            if(trace->code == 2)
            {
                tmp->nb_paquets_traites++;
            }
            if (trace->code == 3)
            {
                tmp->nb_paquets_file--;
                tmp->nb_paquets_recus++;
                tmp->nb_paquets_traites++;
            }
            if (trace->code == 4)
            {
                tmp->nb_paquets_file--;
                tmp->nb_paquets_perdus++;
            }
            trouve = 1;
            break;
        }
        else
        {
            tmp = tmp->s;
        }
    }

    /* nouveau noeud à insérer - noeud non trouvé dans la liste chaînée */
    if (trouve == 0)
    {
        tmp = (Node) malloc( sizeof( p_noeud ) );
        tmp->noeud              = trace->pos;
        tmp->nb_paquets_passage = 0;
        tmp->nb_paquets_traites = 0;
        tmp->nb_paquets_recus   = 0;
        tmp->nb_paquets_emis    = 0;
        tmp->nb_paquets_perdus  = 0;
        tmp->nb_paquets_file    = 0;

        /* insertion en début de file */
        if ( node->p == NULL)
        {
            node->p = tmp;
        }
        else
        {
            tmp->s = node->p;
            node->p = tmp;
        }

        /* traitement du code de la trace de simulation */
        if(trace->code == 0)
        {
            tmp->nb_paquets_emis++;
            tmp->nb_paquets_file++;
        }
        if(trace->code == 1)
        {
            tmp->nb_paquets_passage++;
            tmp->nb_paquets_file++;
            tmp->nb_paquets_traites++;
        }
        if(trace->code == 2)
        {
            tmp->nb_paquets_traites++;
        }        
        if (trace->code == 3)
        {
            tmp->nb_paquets_file--;
            tmp->nb_paquets_recus++;
            tmp->nb_paquets_traites++;
        }
        if (trace->code == 4)
        {
            tmp->nb_paquets_file--;
            tmp->nb_paquets_perdus++;
        }
        /* nouveau noeud */
        data->nb_noeud++;
    }

}
void free_Node(Node f)
{
    Node tmp, tmp2;

    tmp = f->p;
    tmp2 = NULL;

    while(tmp != NULL)
    {
        tmp2 = tmp;
        tmp = tmp->s;
        //free(tmp2->s);
        //free(tmp2->p);
        free(tmp2);
    }
}

void printf_Node(Node node, int my_node)
{
    Node tmp;
    int trouve;

    trouve = 0;
    tmp = node->p;
    while (tmp != NULL)
    {
        if (tmp->noeud == my_node)
        {
            printf("\n*******************Affichage Noeud*********************\n");
            printf( "Noeud                      : N%d\n"
                    "Nombre de Paquets traités  : %d\n"
                    "Nombre de Paquets recus    : %d\n"
                    "Nombre de Paquets emis     : %d\n"
                    "Nombre de Paquets perdus   : %d\n"
                    "Nombre de Paquets passés   : %d\n",
                    tmp->noeud,
                    tmp->nb_paquets_traites,
                    tmp->nb_paquets_recus,
                    tmp->nb_paquets_emis,
                    tmp->nb_paquets_recus,
                    tmp->nb_paquets_passage);
            printf("******************************************************\n\n");
            trouve = 1;
            break;
        }
        else
        {
            tmp = tmp->s;    
        }
    }
    if (trouve == 0)
    {
        fprintf(stderr, "Node N%d n'existe pas, réessayez.\n", my_node);
    }
}