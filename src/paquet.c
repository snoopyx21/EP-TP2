#include "../include/paquet.h"

Paquet create_Paquet(Paquet paquet)
{
    paquet->pid                     = 0;
    paquet->source_noeud            = 0;
    paquet->dest_noeud              = 0;
    paquet->temps_attente_files     = 0;
    paquet->temps_chemin            = 0;
    paquet->taille_paquet           = 0;
    return paquet;
}

void update_Paquet(Trace trace, Data data, Node l_noeud, Paquet l_paquet, Flux l_flux)
{
    Paquet  tmp;
    Node    tmp_noeud;
    Flux    tmp_flux;

    if (trace->code == 0)
    {
        tmp                         = (Paquet)NULL;
        tmp                         = (Paquet) malloc(sizeof ( p_paquet ) );
        tmp->s                      = (Paquet) NULL;
        tmp->p                      = (Paquet) NULL;
        tmp->pid                    = trace->pid;
        tmp->source_noeud           = trace->s;
        tmp->dest_noeud             = trace->d;
        tmp->temps_chemin           = 0;
        tmp->temps_attente_files    = 0;

        Chemin chemin               = (Chemin)NULL;
        chemin                      = (Chemin) malloc(sizeof ( p_chemin ) );
        chemin->noeud_actuel        = trace->pos;
        chemin->date                = trace->t;
        chemin->code                = trace->code;
        chemin->p                   = chemin;
        chemin->s                   = (Chemin) NULL;
        tmp->c                      = chemin;
        
        if ( l_paquet->p == NULL)
        {
            l_paquet->p = tmp;
        }
        else
        {
            tmp->s = l_paquet->p;
            l_paquet->p = tmp;
        }
    }

    /* le paquet existe déjà - il est en transit */
    else
    {
        tmp = l_paquet->p;
        while( tmp != NULL )
        {
            if(tmp->pid == trace->pid)
            {
                Chemin tmp_chemin2;
                Chemin tmp_chemin;

                tmp_chemin2                 = (Chemin)NULL;
                tmp_chemin2                 = (Chemin) malloc(sizeof(p_chemin ) );
                tmp_chemin2->code           = trace->code;
                tmp_chemin2->date           = trace->t;
                tmp_chemin2->noeud_actuel   = trace->pos;
                tmp_chemin2->s              = (Chemin)NULL;
                
                if (tmp->c->p == NULL)
                { 
                    fprintf(stderr, "error : memory path error in paquet.c\n");
                    exit(0);
                }
                else
                {
                    tmp_chemin = tmp->c->p;
                }

                /* insertion du nouveau noeud dans le chemin en fin de liste chaînée */
                if (tmp_chemin->s == NULL)
                {
                    tmp_chemin->s   = tmp_chemin2;
                    tmp_chemin2->p  = tmp_chemin->p;
                    //tmp_chemin      = tmp_chemin->s;
                }
                else 
                {
                    while(tmp_chemin->s != NULL)
                    {
                        tmp_chemin = tmp_chemin->s;
                    }
                    tmp_chemin->s   = tmp_chemin2;
                    tmp_chemin2->p  = tmp_chemin->p;
                    //tmp_chemin      = tmp_chemin->s;
                }


                /* traitement du code reçu - le paquet arrive dans une file d'attente (3 cas possible) */
                if( (tmp_chemin->code == 0 && tmp_chemin2->code == 2) ||
                    (tmp_chemin->code == 1 && tmp_chemin2->code == 3) ||
                    (tmp_chemin->code == 1 && tmp_chemin2->code == 2) )
                {
                    tmp->temps_attente_files += tmp_chemin2->date - tmp_chemin->date ;
                }
                if( (tmp_chemin->code == 2 && tmp_chemin2->code == 1) ||
                    (tmp_chemin->code == 2 && tmp_chemin2->code == 3) )
                {
                    tmp->temps_chemin += tmp_chemin2->date - tmp_chemin->date ;
                }

                if(trace->code == 2)
                {
                    tmp_noeud = (Node) NULL;
                    tmp_noeud = l_noeud->p;
                    while(tmp_noeud != NULL)
                    {
                        /* si code == 2 et le noeud est dans la liste chaînée */
                        if (tmp_noeud->noeud == trace->pos)
                        {
                            /* sortie de file d'attente du noeud - on pointe sur le nouveau noeud mais on doit maj la file */
                            tmp_noeud->nb_paquets_passage++;
                            tmp_noeud->nb_paquets_file--;
                            break;
                        }
                        else
                        {
                            tmp_noeud = tmp_noeud->s;
                        }
                    }
                }

                /* paquet arrive dans son noeud de destination */
                if(trace->code == 3)
                {
                    data->delai_moyen                       += tmp->temps_chemin + tmp->temps_attente_files;
                    data->temps_transmission_moyen_liens    += tmp->temps_chemin;
                    data->temps_attente_moyen_files         += tmp->temps_attente_files;
                    
                    tmp_flux = (Flux) NULL;
                    tmp_flux = l_flux->p;

                    while( tmp_flux != NULL)
                    {
                        if (tmp_flux->fid == trace->fid)
                        {
                            tmp_flux->nb_paquets_recus++;
                            tmp_flux->delai_moyen_acheminement_paquet += tmp->temps_chemin + tmp->temps_attente_files;
                            break;
                        }
                        else
                        {
                            tmp_flux = tmp_flux->s;
                        }
                    }
                }
                break;
            }
            else
            {
                tmp = tmp->s;
            }
        }
    }
}
void free_Paquet(Paquet paquet)
{
    Paquet tmp, tmp2;
    Chemin tmp_chemin, tmp_chemin2;
    
    tmp = paquet->p;
    tmp2 = NULL;

    while(tmp != NULL)
    {
        tmp_chemin = tmp->c->p;
        tmp_chemin2 = NULL;
        while(tmp_chemin != NULL)
        {
            tmp_chemin2 = tmp_chemin;
            tmp_chemin = tmp_chemin->s;
            //free(tmp_chemin2->s);
            //free(tmp_chemin2->p);
            free(tmp_chemin2);
        }
        tmp2 = tmp;
        tmp = tmp->s;
        //free(tmp2->s);
        //free(tmp2->p);
        free(tmp2);
    }
}

void printf_Paquet(Paquet paquet, int my_pid, int **  matrice_adjacence)
{
    Paquet tmp;
    int trouve;
    Chemin tmp_chemin;

    tmp     = paquet->p;
    trouve  = 0;

    while( tmp != NULL)
    {
        if ( tmp->pid == my_pid ) 
        {
            tmp_chemin = tmp->c->p;
            if(tmp_chemin->s != NULL && tmp_chemin->s->s != NULL)
            {
                tmp->taille_paquet = ( matrice_adjacence[tmp_chemin->noeud_actuel-1][tmp_chemin->s->noeud_actuel-1]
                                     + matrice_adjacence[tmp_chemin->s->noeud_actuel-1][tmp_chemin->noeud_actuel-1] )
                                     * (tmp_chemin->s->s->date - tmp_chemin->s->date);
            }
            else 
            {
                tmp->taille_paquet = 0;
            }

            printf("*******************Affichage Paquet*********************\n");
            printf( "PID                    : %d\n"
                    "Source                 : N%d\n"
                    "Destination            : N%d\n"
                    "Temps attente files    : %lf\n"
                    "Temps chemin           : %lf\n"
                    "Taille Paquet          : %lf\n",
                    tmp->pid,
                    tmp->source_noeud,
                    tmp->dest_noeud,
                    tmp->temps_attente_files,
                    tmp->temps_chemin,
                    tmp->taille_paquet);

            printf( "\n-------------------------------\n"
                    "Chemin emprunté par le paquet : \n");
            printf("Numero \t"
                    "Noeud  \t"
                    "Code   \t"
                    "Date   \t\n");
            int i = 1;
            while(tmp_chemin != NULL)
            {
                printf( "%d  \t"
                        "N%d \t"
                        "%d \t"
                        "%lf \t\n",
                        i,
                        tmp_chemin->noeud_actuel,
                        tmp_chemin->code,
                        tmp_chemin->date);
                tmp_chemin = tmp_chemin->s;
                i++;
            }
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
        fprintf(stderr, "Paquet %d n'existe pas, réessayez.\n", my_pid);
    }
}


