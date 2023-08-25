#include "../include/trace.h"

void create_Trace(Trace trace)
{
    trace->t        = 0;     
    trace->code     = 0;
    trace->pid      = 0;
    trace->fid      = 0;       
    trace->s        = 0;
    trace->d        = 0;
    trace->pos      = 0;
}
void printf_trace(Trace trace)
{
    printf("Trace : %lf %hd %d %hd %d %d %d\n",
        trace->t,
        trace->code,
        trace->pid,
        trace->fid,                
        trace->s,
        trace->d,
        trace->pos );
}
void create_Data(Data data)
{
    data->nb_paquets                    = 0;    
    data->nb_paquets_traites            = 0;   
    data->nb_paquets_emis               = 0;   
    data->nb_paquets_recus              = 0;   
    data->nb_paquets_perdus             = 0; 
    data->nb_paquets_actifs             = 0;   
    data->nb_flux                       = 0;   
    data->nb_noeud                      = 0;   
    data->nb_evenement                  = 0;   
    data->delai_moyen                   = 0;  
    data->temps_attente_moyen_files     = 0;
    data->temps_transmission_moyen_liens= 0;
    data->nb_file_paquets               = 0;
    data->nb_flux_actif                 = 0;

    data->gnuplot                       = 0;
}

void file_gnuplot(Data data)
{
    data->gnuplot = 1;
    data->fichier_nb_paquets_perdus     = fopen("./gnuplot/nb_paquets_perdus.dat", "w");
    if(data->fichier_nb_paquets_perdus == NULL)
    {
        perror("fopen");
        return;
    }
    data->fichier_nb_paquets_transit    = fopen("./gnuplot/nb_paquets_transit.dat", "w");
    if(data->fichier_nb_paquets_transit == NULL)
    {
        perror("fopen");
        return;
    }
    data->fichier_nb_flux_actif         = fopen("./gnuplot/nb_flux_actif.dat", "w");
    if(data->fichier_nb_flux_actif == NULL)
    {
        perror("fopen");
        return;
    }
    data->fichier_nb_file_paquets       = fopen("./gnuplot/nb_file_paquets.dat", "w");
    if(data->fichier_nb_file_paquets == NULL)
    {
        perror("fopen");
        return;
    }
}
void update_Data(Trace trace, Data data)
{
    switch(trace->code)
    {
        case 0:
            data->nb_paquets_emis++;
            data->nb_paquets++;
            data->nb_paquets_actifs++;
            data->nb_file_paquets++;
            if (data->gnuplot == 1)
            {
                fprintf(data->fichier_nb_file_paquets,"%f %d\n", trace->t, data->nb_file_paquets);
            }
            data->nb_paquets_transit++;
            break;
        case 1:
            data->nb_file_paquets++;
            if (data->gnuplot == 1)
            {
                fprintf(data->fichier_nb_file_paquets,"%f %d\n", trace->t, data->nb_file_paquets);
            }
            break;
        case 2:
            data->nb_paquets_traites++;
            data->nb_file_paquets--;
            if (data->gnuplot == 1)
            {
                fprintf(data->fichier_nb_file_paquets,"%f %d\n", trace->t, data->nb_file_paquets);
            }
            break;
        case 3:
            data->nb_paquets_recus++;
            data->nb_paquets_actifs--;
            data->nb_file_paquets--;
            if (data->gnuplot == 1)
            {
                fprintf(data->fichier_nb_file_paquets,"%f %d\n", trace->t, data->nb_file_paquets);
            }
            data->nb_paquets_transit--;
            break;
        case 4:
            data->nb_paquets_perdus++;
            if(data->gnuplot == 1)
            {
                fprintf(data->fichier_nb_paquets_perdus, "%f %d\n", trace->t, data->nb_paquets_perdus);
            }
            data->nb_paquets_actifs--;
            data->nb_file_paquets--;
            if (data->gnuplot == 1)
            {
                fprintf(data->fichier_nb_file_paquets,"%f %d\n", trace->t, data->nb_file_paquets);
            }
            data->nb_paquets_transit--;
            break;
        default:
            fprintf(stderr, "trace.c - insertion_Data : code de traitement d'événement inconnu\n");
            break;
    }

    if (trace->code != 1 && trace->code != 2 && data->gnuplot == 1)
    {
        fprintf(data->fichier_nb_paquets_transit,"%f %d\n", trace->t, data->nb_paquets_transit);
    }
    
    data->nb_evenement++;
}
void free_Data(Data data)
{
    free(data);
}
void printf_Data(Data data)
{
    printf("*******************Affichage DATA*********************\n");
    printf( "Nombre de Paquets              : %d\n"
            "Paquets traités                : %d\n"
            "Paquets émis                   : %d\n"
            "Paquets reçues                 : %d\n"
            "Paquets perdus                 : %d\n"
            "Paquets actifs                 : %d\n"
            "Nombre de Flux                 : %d\n"
            "Nombre de Noeuds               : %d\n"
            "Nombre d'Evenements            : %d\n"
            "Délai moyen                    : %f\n"
            "Temps attente moyen files      : %f\n"
            "Temps transmission moyen liens : %f\n"
            "Taux perte moyen               : %f\n",
            data->nb_paquets,               
            data->nb_paquets_traites,       
            data->nb_paquets_emis,          
            data->nb_paquets_recus,         
            data->nb_paquets_perdus,        
            data->nb_paquets_actifs,       
            data->nb_flux,                  
            data->nb_noeud,                 
            data->nb_evenement,             
            (float)data->delai_moyen/(float)data->nb_paquets_recus,                           
            (float)data->temps_attente_moyen_files/(float)data->nb_paquets_recus,
            (float)data->temps_transmission_moyen_liens/(float)data->nb_paquets_recus,
            (float)data->nb_paquets_perdus/(float)data->nb_paquets_emis*100);    
    printf("******************************************************\n");   
}

void help_command(void)
{
    printf("\n  Help command - usage : ./eval <file_name>.txt <matrice>.txt\n");
    printf("  Options with eval (possibility to have all options):\n");
    printf("  GNUPLOT   : -g\n");
    printf("  Node      : -n N<ID>\n");
    printf("  Packet    : -p <pid>\n");
    printf("  Flux      : -f <fid>\n\n");
}

int file_gnuplot_execution(Data data)
{
        int a, rc;

        char * gnuplot_titre1[MAX_CHAR_GNUPLOT] = { "set terminal png\n",
                                                    "set output './gnuplot/gnuplot_paquets_perdus.png'\n",
                                                    "set title \"Nombre de paquets perdus au cours du temps\"\n",
                                                    "set xlabel 'Temps'\n",
                                                    "set ylabel 'Nombre de paquets perdus'\n",
                                                    "plot './gnuplot/nb_paquets_perdus.dat' u 1:2 w l\n" ,
                                                    "exit\n"};
        FILE * gnuplot_paquets_perdus = NULL;
        gnuplot_paquets_perdus = popen("gnuplot -persistant", "w");
        for(a = 0 ; a < 7 ; a++)
        {
            fprintf(gnuplot_paquets_perdus, "%s", gnuplot_titre1[a]);
        }
        /* fermeture fichier */
        rc = fclose(data->fichier_nb_paquets_perdus);
        if( rc == -1 )
        {
            perror("fclose(data->fichier_nb_paquets_perdus)");
            return errno;
        }
        if (gnuplot_paquets_perdus != NULL) 
        {
            rc = pclose(gnuplot_paquets_perdus);
            if( rc == -1 )
            {
                perror("fclose(data->fichier_nb_paquets_perdus)");
                return errno;
            }
        }


        char * gnuplot_titre2[MAX_CHAR_GNUPLOT] = { "set terminal png\n",
                                                    "set output './gnuplot/gnuplot_paquets_transit.png'\n",
                                                    "set title \"Nombre de paquets en transit au cours du temps\"\n",
                                                    "set xlabel 'Temps'\n",
                                                    "set ylabel 'Nombre de paquets en transit'\n",
                                                    "plot './gnuplot/nb_paquets_transit.dat' u 1:2 w l\n" ,
                                                    "exit\n "};
        FILE * gnuplot_paquets_transit = NULL;
        gnuplot_paquets_transit = popen("gnuplot -persistant", "w");
        for(a = 0 ; a < 7 ; a++)
        {
            fprintf(gnuplot_paquets_transit, "%s", gnuplot_titre2[a]);
        }
        /* fermeture fichier */
        rc = fclose(data->fichier_nb_paquets_transit);
        if( rc == -1 )
        {
            perror("fclose(data->fichier_nb_paquets_transit)");
            return errno;
        }
        if (gnuplot_paquets_transit != NULL) 
        {
            rc = pclose(gnuplot_paquets_transit);
            if( rc == -1 )
            {
                perror("fclose(data->fichier_nb_paquets_perdus)");
                return errno;
            }
        }


        char * gnuplot_titre3[MAX_CHAR_GNUPLOT] = { "set terminal png\n",
                                                    "set output './gnuplot/gnuplot_flux_actif.png'\n",
                                                    "set title \"Nombre de flux actifs au cours du temps\"\n",
                                                    "set xlabel 'Temps'\n",
                                                    "set ylabel 'Nombre de flux actifs'\n",
                                                    "plot './gnuplot/nb_flux_actif.dat' u 1:2 w l\n" ,
                                                    "exit\n"};
        FILE * gnuplot_flux_actif = NULL;
        gnuplot_flux_actif = popen("gnuplot -persistant", "w");
        for(a = 0 ; a < 7 ; a++)
        {
            fprintf(gnuplot_flux_actif, "%s", gnuplot_titre3[a]);
        }
        /* fermeture fichier */
        rc = fclose(data->fichier_nb_flux_actif);
        if( rc == -1 )
        {
            perror("fclose(data->fichier_nb_flux_actif)");
            return errno;
        }
        if (gnuplot_flux_actif != NULL) 
        {
            rc = pclose(gnuplot_flux_actif);
            if( rc == -1 )
            {
                perror("fclose(data->fichier_nb_paquets_perdus)");
                return errno;
            }
        }


        char * gnuplot_titre4[MAX_CHAR_GNUPLOT] = { "set terminal png\n",
                                                    "set output './gnuplot/gnuplot_file_attente.png'\n",
                                                    "set title \"Nombre de paquets en attente dans l\'ensemble des files d'attente\"\n",
                                                    "set xlabel 'Temps'\n",
                                                    "set ylabel 'Taille de toutes les files attente'\n",
                                                    "plot './gnuplot/nb_file_paquets.dat' u 1:2 w l\n",
                                                    "exit\n" };
        FILE * gnuplot_file_attente = NULL;
        gnuplot_file_attente = popen("gnuplot -persistant", "w");
        for(a = 0 ; a < 7 ; a++)
        {
            fprintf(gnuplot_file_attente, "%s", gnuplot_titre4[a]);
        }
        /* fermeture fichier */
        rc = fclose(data->fichier_nb_file_paquets);
        if( rc == -1 )
        {
            perror("fclose(data->fichier_nb_file_paquets)");
            return errno;
        }
        if (gnuplot_file_attente != NULL)
        { 
            pclose(gnuplot_file_attente);
            if( rc == -1 )
            {
                perror("fclose(data->fichier_nb_paquets_perdus)");
                return errno;
            }
        }
        return 0;

}