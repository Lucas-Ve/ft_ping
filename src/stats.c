#include "ft_ping.h"

// Initialiser les statistiques
void init_stats(PingStats *stats) {
    stats->packets_transmitted = 0;
    stats->packets_received = 0;
    stats->total_time = 0;
    stats->rtt_min = LONG_MAX;
    stats->rtt_max = 0;
    stats->rtt_sum = 0.0;
    stats->rtt_sum_squared = 0.0;
    stats->time_out = 0;
    stats->ttl = 0;
    stats->error = 0;
    stats->ip_addr = NULL;
    stats->verbose = 0;
    stats->errorCount = 0;
}

// Enregistrer le RTT pour un paquet reçu
void record_rtt(PingStats *stats, double rtt) {
    stats->rtt_sum += rtt;
    stats->rtt_sum_squared += rtt * rtt;

    if (rtt < stats->rtt_min) stats->rtt_min = rtt;
    if (rtt > stats->rtt_max) stats->rtt_max = rtt;
}

// Marquer un paquet comme transmis
void packet_transmitted(PingStats *stats) {
    stats->packets_transmitted++;
}

// Marquer un paquet comme reçu
void packet_received(PingStats *stats) {
    stats->packets_received++;
}

// Calculer la différence de temps entre deux timestamps (en ms)
double calculate_time_diff(struct timespec *start, struct timespec *end) {
    return ((end->tv_sec - start->tv_sec) * 1000.0) + ((end->tv_nsec - start->tv_nsec) / 1000000.0);
}

// Afficher les statistiques finales
void print_final_stats(PingStats *stats) {
    // Calcul de la perte de paquets
    int packet_loss = ((stats->packets_transmitted - stats->packets_received) * 100) / stats->packets_transmitted;
    
    // Calcul de la moyenne et de l'écart-type (mdev)
    double rtt_avg = stats->rtt_sum / stats->packets_received;
    double rtt_mdev = sqrt((stats->rtt_sum_squared / stats->packets_received) - (rtt_avg * rtt_avg));

    // Affichage des statistiques
    printf("\n--- %s ping statistics ---\n", stats->ip_addr);
    if(stats->error == 0){
        printf("%d packets transmitted, %d received, %d%% packet loss, time %3ldms\n",
               stats->packets_transmitted, stats->packets_received, packet_loss, stats->total_time);
    }
    else{
        printf("%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %3ldms\n",
               stats->packets_transmitted, stats->packets_received, stats->error, packet_loss, stats->total_time);
    }

    if(stats->packets_received != 0){
        // Les RTT sont déjà en millisecondes avec des décimales, donc pas besoin de les diviser par 1000
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
               stats->rtt_min, rtt_avg, stats->rtt_max, rtt_mdev);
    }
}



