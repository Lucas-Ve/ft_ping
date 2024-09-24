#include "ft_ping.h"

int sockfd;
int controleC = 1;

void handle_sigint(int sig)
{
    (void)sig;
    controleC = 0;
}

int main(int argc, char *argv[])
{
    int ttl = 0;
    if (argc < 2)
    {
        printf("Error: missing arguments. Use -? for help.\n");
        return 1;
    }
    PingStats stats;
    init_stats(&stats);  // Initialiser les statistiques
    parsing(argc, argv, &stats);
    sockfd = create_socket();
    struct icmphdr icmp_hdr;
    int sequence = 0;
    if(stats.verbose == 1)
        printf("PING %s (%s): 56 data bytes, id 0x%04x = %d\n", stats.ip_addr, stats.ip_str, getpid(), getpid());
    else
        printf("PING %s (%s): 56 data bytes\n", stats.ip_addr, stats.ip_str);
    signal(SIGINT, handle_sigint);
    struct timespec start_time, end_time;  // Variables pour capturer le temps global
    // Capturer le temps de début du programme
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while (controleC)
    {
        struct timespec start, end;  // Variables pour enregistrer le temps
        create_icmp_packet(&icmp_hdr, sequence);
        packet_transmitted(&stats);
        // Enregistrer le temps avant l'envoi du paquet
        clock_gettime(CLOCK_MONOTONIC, &start);
        send_icmp_packet(sockfd, stats.ip_str, &icmp_hdr);
        receive_icmp_reply(sockfd, &ttl, &stats);
        // Enregistrer le temps après la réception du paquet
        clock_gettime(CLOCK_MONOTONIC, &end);
        if(stats.time_out == 0){
            if(stats.ttl == 1)
                printf("64 bytes from %s: icmp_seq=%d Time to live exceeded\n", stats.ip_str, sequence);
            else if(stats.errorCount == 0){
                packet_received(&stats);
                printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", stats.ip_str, sequence, ttl, calculate_time_diff(&start, &end));
                record_rtt(&stats, calculate_time_diff(&start, &end));
            }
        }
        sequence++;
        sleep(1);
    }
    if(controleC == 0)
    {
        // Capturer le temps de fin du programme
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        // Calculer le temps total écoulé (en millisecondes)
        stats.total_time = calculate_time_diff(&start_time, &end_time);
        print_final_stats(&stats);
        close(sockfd);
    }
    return 0;
}
