#ifndef TRACE_H
#define TRACE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define SIZE_FIRST_LINE 256         // utilisation pour matrice d'adjacence
#define MAX_CHAR_GNUPLOT 1024       // utilisation pour gnuplot

typedef struct s_trace_de_simulation
{
    float t;
    short code;
    int pid;
    short fid; 
    short tos;
    int  s; 
    int d; 
    int pos;
} p_trace_de_simulation, *Trace;

typedef struct s_data 
{
    int nb_paquets; 
    int nb_paquets_traites;
    int nb_paquets_emis;
    int nb_paquets_recus;
    int nb_paquets_perdus;      // gnuplot
    int nb_paquets_actifs;
    int nb_file_paquets;        // gnuplot
    int nb_paquets_transit;     // gnuplot
    int nb_flux;
    int nb_flux_actif;          // gnuplot
    int nb_noeud;
    int nb_evenement;
    float delai_moyen;
    float temps_attente_moyen_files;
    float temps_transmission_moyen_liens;

    int gnuplot;
    FILE * fichier_nb_paquets_perdus;
    FILE * fichier_nb_paquets_transit;
    FILE * fichier_nb_flux_actif;
    FILE * fichier_nb_file_paquets;

} p_data, *Data;

void create_Trace(Trace trace);
void printf_trace(Trace trace);
void file_gnuplot(Data data);
void create_Data(Data data);
void update_Data(Trace trace, Data data);
void free_Data(Data data);
int file_gnuplot_execution(Data data);
void printf_Data(Data data);
void help_command(void);
#endif