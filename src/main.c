#include "../include/trace.h"
#include "../include/flux.h"
#include "../include/noeud.h"
#include "../include/paquet.h"

/**********************************************************************************
 * pré-conditions pour pouvoir avoir l'affichage d'un noeud, d'un paquet ou d'un  *
 * flux : obligation de connaître son ID ou son nom (pour le noeud)               *
 **********************************************************************************/

int main(int argc, char * argv[])
{
    FILE *  f, * f_mat;
    Trace   trace;
    Paquet  l_paquet;
    Node    l_noeud;
    Flux    l_flux;
    Data    data;
    int     rc, 
            line, 
            inutile, 
            opt,
            my_node, 
            my_fid, 
            my_pid,
            a, b,
            i, j;
    int     *matrice_adjacence[SIZE_FIRST_LINE];
    char    inutile2;

    if (argc > 10)
    {
        /* affichage HELP */
        help_command();
        fprintf(stderr, "usage : ./eval <file_name>.txt <matrice>.txt\n");        
        return (-1);
    }

    /* initialisation des paramètres */
    rc                  = 0;
    inutile             = 0;
    line                = 0;
    opt                 = 9;
    my_pid              = (-1);
    my_node             = (-1);
    my_fid              = (-1);
    i                   = 0;
    j                   = 0;

    l_flux              = (Flux)NULL;
    l_flux              = (Flux)malloc(sizeof ( p_flux ) );
    l_flux              = create_Flux(l_flux);
    l_flux->p           = l_flux;
    l_flux->s           = (Flux)NULL;

    l_noeud             = (Node)NULL;
    l_noeud             = (Node)malloc(sizeof ( p_noeud ) );
    l_noeud             = create_Node(l_noeud);
    l_noeud->p          = l_noeud;
    l_noeud->s          = (Node)NULL;

    l_paquet            = (Paquet)NULL;
    l_paquet            = (Paquet)malloc(sizeof ( p_paquet ) );
    l_paquet            = create_Paquet(l_paquet);
    l_paquet->p         = l_paquet;
    l_paquet->c         = (Chemin)malloc(sizeof ( p_chemin ) );
    l_paquet->c->p      = l_paquet->c;
    l_paquet->c->s      = (Chemin)NULL;
    l_paquet->s         = (Paquet)NULL;

    data                = (Data)NULL;
    data                = (Data)malloc(sizeof(p_data));
    create_Data(data);

    

    /* ouverture fichier */
    f = fopen(argv[1], "r");
    if (f == NULL)
    {
        /* libération mémoire */
        free_Paquet(l_paquet);
        free_Node(l_noeud);
        free_Flux(l_flux);
        free_Data(data);
        
        perror("fopen");
        return errno;
    }
    f_mat = fopen(argv[2], "r");
    if (f_mat == NULL)
    {
        /* libération mémoire */
        free_Paquet(l_paquet);
        free_Node(l_noeud);
        free_Flux(l_flux);
        free_Data(data);
        perror("fopen");
        return errno;
    }

    
    while( (opt = getopt(argc, argv, "n:f:gp:") ) != -1)
    {
        switch( opt )
        {
            case 'n':
                sscanf(optarg, "%c%d", &inutile2, &my_node);
                break;
            case 'f':
                my_fid = atoi(optarg);
                break;
            case 'p':
                my_pid = atoi(optarg);
                break;
            case 'g':
                file_gnuplot(data);
                break;
            default:
                break;
        }
    }

    /*************************** lecture matrice adjacence *********************************/
    for( a = 0 ; a < SIZE_FIRST_LINE ; a++) matrice_adjacence[a] = (int *)malloc(SIZE_FIRST_LINE * sizeof(int));

    fprintf(stdout, "Chargement...\n");

    /* lecture première ligne pour obtenir i et j */
    while(line != EOF)
    {
        i++;
        line = fscanf(f_mat, "%d%c", &inutile, &inutile2);
        if (inutile2 == '\n')
            break;
    }

    j = i;
    /* reprise depuis le début du fichier : insertion de la matrice dans le tableau */
    rewind(f_mat);
    for(a = 0 ; a < i ; a++)
    {
        for(b = 0 ; b < j ; b++)
        {
            line = fscanf(f_mat, "%d", &matrice_adjacence[a][b]);
            if (line == EOF)
                break;
        }
    }
    /* fermeture fichier */
    rc = fclose(f_mat);
    if( rc == -1 )
    {
        /* libération mémoire */
        free_Paquet(l_paquet);
        free_Node(l_noeud);
        free_Flux(l_flux);
        free_Data(data);
        for( a = 0 ; a < SIZE_FIRST_LINE ; a++) free(matrice_adjacence[a]);
        perror("fclose(f_mat)");
        return errno;
    }
    /*************************** fin lecture matrice adjacence *********************************/



    /* lecture fichier */
    while( line != EOF)
    {
        /* création d'une trace de simulation pour enregistrer la ligne */
        trace = (Trace) malloc(sizeof(p_trace_de_simulation));
        create_Trace(trace);

        /* lecture de la ligne par utilisation de fscanf - on ne stocke rien dans une chaine de caractère */
        line = fscanf(f, "%f %hd",
            &trace->t,
            &trace->code);

        /* si code == 4, il y a un nombre d'arguments différents */
        if ( trace->code != 4)
        {
            line = fscanf(f, "%d %hd %d %d %c%d %c%d %c%d\n",
                &trace->pid,
                &trace->fid,
                &inutile,
                &inutile,
                &inutile2,
                &trace->s,
                &inutile2,
                &trace->d,
                &inutile2,
                &trace->pos );
        }
        else
        {
            line = fscanf(f, "%d %hd %d %c%d %c%d %c%d\n",
                &trace->pid,
                &trace->fid,
                &inutile,
                &inutile2,
                &trace->s,
                &inutile2,
                &trace->d,
                &inutile2,
                &trace->pos );
        }

        // printf_trace(trace);

        if ( line != EOF)
        {
            /* insertion des informations de la trace dans la files chaînées */
            update_Data(trace, data);
            update_Flux(l_flux, trace, data);
            update_Node(l_noeud, trace, data);
            update_Paquet(trace, data, l_noeud, l_paquet, l_flux);
        }
        free(trace);
    }

    /* affichage finale selon le getopt effectué plus tôt */
    if (my_node != (-1))
        printf_Node(l_noeud, my_node);
    if (my_fid != (-1))
        printf_Flux(l_flux, my_fid);
    if (my_pid != (-1))
        printf_Paquet(l_paquet, my_pid, matrice_adjacence);
    printf_Data(data);


/****************************************** GNUPLOT ***************************************************/
    if (data->gnuplot == 1)
    {
        if (file_gnuplot_execution(data) != 0)
        {
            /* libération mémoire */
            free_Paquet(l_paquet);
            free_Node(l_noeud);
            free_Flux(l_flux);
            free_Data(data);
            for( a = 0 ; a < SIZE_FIRST_LINE ; a++) free(matrice_adjacence[a]);
            return (-1);
        }
    }
/******************************************************************************************************/

    /* libération mémoire */
    free_Paquet(l_paquet);
    free_Node(l_noeud);
    free_Flux(l_flux);
    free_Data(data);
    for( a = 0 ; a < SIZE_FIRST_LINE ; a++) free(matrice_adjacence[a]);

    /* fermeture fichier */
    rc = fclose(f);
    if( rc == -1 )
    {
        perror("fclose(f)");
        return errno;
    }

    return (0);
}