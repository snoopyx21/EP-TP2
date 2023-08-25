#include "../include/flux.h"

Flux create_Flux(Flux flux)
{
    flux->fid                   = 0;
    flux->debut_flux            = 0;
    flux->fin_flux              = 0;
    flux->noeud_source          = 0;
    flux->noeud_dest            = 0;
    flux->duree_vie             = 0;
    flux->nb_paquets            = 0;
    flux->nb_paquets_traites    = 0;
    flux->nb_paquets_recus      = 0;
    flux->nb_paquets_emis       = 0;
    flux->nb_paquets_perdus     = 0;
    flux->delai_moyen_acheminement_paquet      = 0;
    return flux;
}
void update_Flux(Flux flux, Trace trace, Data data)
{
    Flux tmp = (Flux) NULL;
    int trouve;

    tmp = flux->p;
    trouve = 0;
    
    while( tmp != NULL)
    {
        if (tmp->fid == trace->fid)
        {
            tmp->fin_flux = trace->t;
            tmp->nb_paquets++;
            if(trace->code == 0)
            {
                tmp->nb_paquets_traites++;
                tmp->nb_paquets_emis++;
                tmp->duree_vie++;
                if(data->gnuplot == 1) fprintf(data->fichier_nb_flux_actif, "%f %d\n", trace->t, tmp->duree_vie);
            }
            if(trace->code == 2)
            {
                tmp->nb_paquets_traites++;
            }
            if (trace->code == 3)
            {
                tmp->nb_paquets_traites++;
                tmp->duree_vie--;
                if(data->gnuplot == 1) fprintf(data->fichier_nb_flux_actif, "%f %d\n", trace->t, tmp->duree_vie);
            }
            if (trace->code == 4)
            {
                tmp->duree_vie--;
                tmp->nb_paquets_perdus++;
                if(data->gnuplot == 1) fprintf(data->fichier_nb_flux_actif, "%f %d\n", trace->t, tmp->duree_vie);
            }
            trouve = 1;
            break;
        }
        else
        {
            tmp = tmp->s;
        }
    }

    /* nouvelle file à insérer - file non trouvée dans la liste chaînée */
    if (trouve == 0)
    {
        tmp = (Flux) malloc( sizeof( p_flux ) );
        tmp->fid                = trace->fid;
        tmp->debut_flux         = trace->t;
        tmp->fin_flux           = trace->t;
        tmp->noeud_source       = trace->s;
        tmp->noeud_dest         = trace->d;
        tmp->duree_vie          = 0;
        tmp->nb_paquets         = 0;
        tmp->nb_paquets_traites = 0;
        tmp->nb_paquets_recus   = 0;
        tmp->nb_paquets_emis    = 0;
        tmp->nb_paquets_perdus  = 0;
        tmp->delai_moyen_acheminement_paquet   = 0;

        /* insertion en début de liste */
        if ( flux->p == NULL)
        {
            flux->p = tmp;
        }
        else
        {
            tmp->s = flux->p;
            flux->p = tmp;
        }

        if(trace->code == 0)    
        {
            tmp->nb_paquets_emis++;
            tmp->duree_vie++;
            if(data->gnuplot == 1) fprintf(data->fichier_nb_flux_actif, "%f %d\n", trace->t, tmp->duree_vie);
        }
        if(trace->code == 2)
        {
            tmp->nb_paquets_traites++;
        }
        if (trace->code == 3)
        {
            tmp->duree_vie--;
            tmp->nb_paquets_traites++;
            if(data->gnuplot == 1) fprintf(data->fichier_nb_flux_actif, "%f %d\n", trace->t, tmp->duree_vie);
        }
        if (trace->code == 4)
        {
            tmp->duree_vie--;
            tmp->nb_paquets_perdus++;
            if(data->gnuplot == 1) fprintf(data->fichier_nb_flux_actif, "%f %d\n", trace->t, tmp->duree_vie);
        }
        tmp->nb_paquets++;
        /* nouveau flux à insérer dans data */
        data->nb_flux++;
    }

}
void free_Flux(Flux f)
{
    Flux tmp, tmp2;

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
void printf_Flux(Flux f, int my_fid)
{
    Flux tmp;
    int trouve;

    tmp = f->p;
    trouve = 0;

    while( tmp != NULL)
    {
        if ( tmp->fid == my_fid ) 
        {
            printf("\n*******************Affichage Flux*********************\n");
            printf( "FID                    : %d\n"
                    "Début Flux             : %lf\n"
                    "Fin Flux               : %lf\n"
                    "Source                 : N%d\n"
                    "Destination            : N%d\n"
                    "Nombre de Paquets      : %d\n"
                    "Paquets traités        : %d\n"
                    "Paquets émis           : %d\n"
                    "Paquets reçues         : %d\n"
                    "Paquets perdus         : %d\n"
                    "Délai moyen            : %lf\n",
                    tmp->fid,
                    tmp->debut_flux,
                    tmp->fin_flux,
                    tmp->noeud_source,
                    tmp->noeud_dest,
                    tmp->nb_paquets,
                    tmp->nb_paquets_traites,
                    tmp->nb_paquets_emis,
                    tmp->nb_paquets_recus,
                    tmp->nb_paquets_perdus,
                    (float)tmp->delai_moyen_acheminement_paquet);
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
        fprintf(stderr, "Flux %d n'existe pas, réessayez.\n", my_fid);
    }
}