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
}

// Enregistrer le RTT pour un paquet reçu
void record_rtt(PingStats *stats, long rtt) {
    stats->rtt_sum += rtt;
    stats->rtt_sum_squared += rtt * rtt;
    if (rtt < stats->rtt_min) stats->rtt_min = rtt;
    if (rtt > stats->rtt_max) stats->rtt_max = rtt;
    stats->total_time += rtt;
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
void print_final_stats(PingStats *stats, const char *ip_addr) {
    printf("\n\n%f, %ld, %ld\n\n", stats->rtt_sum, stats->rtt_max, stats->rtt_min);
    int packet_loss = ((stats->packets_transmitted - stats->packets_received) * 100) / stats->packets_transmitted;
    double rtt_avg = stats->rtt_sum / stats->packets_received;
    double rtt_mdev = sqrt((stats->rtt_sum_squared / stats->packets_received) - (rtt_avg * rtt_avg));

    printf("\n--- %s ping statistics ---\n", ip_addr);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n",
           stats->packets_transmitted, stats->packets_received, packet_loss, stats->total_time);
    printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
           stats->rtt_min / 1000.0, rtt_avg / 1000.0, stats->rtt_max / 1000.0, rtt_mdev / 1000.0);
}
