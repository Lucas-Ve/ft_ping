#include "ft_ping.h"

int sockfd;
int controleC = 1;

void handle_sigint(int sig)
{
    (void)sig;
    // close(sockfd);
    controleC = 0;
    // exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int ttl = 0;
    char *ip_addr = NULL;

    if (argc < 2)
    {
        printf("Error: missing arguments. Use -? for help.\n");
        return 1;
    }
    char ip_str[INET_ADDRSTRLEN];
    parsing(argc, argv, &ip_addr, ip_str, sizeof(ip_str));
    PingStats stats;
    init_stats(&stats);  // Initialiser les statistiques
    sockfd = create_socket();
    struct icmphdr icmp_hdr;
    int sequence = 0;

    printf("PING %s (%s): 56 data bytes\n", ip_addr, ip_str);
    signal(SIGINT, handle_sigint);
    while (controleC)
    {
        struct timespec start, end;  // Variables pour enregistrer le temps
        create_icmp_packet(&icmp_hdr, sequence++);
        packet_transmitted(&stats);
        // Enregistrer le temps avant l'envoi du paquet
        clock_gettime(CLOCK_MONOTONIC, &start);
        send_icmp_packet(sockfd, ip_str, &icmp_hdr);
        receive_icmp_reply(sockfd, &ttl);
        // Enregistrer le temps après la réception du paquet
        clock_gettime(CLOCK_MONOTONIC, &end);
        packet_received(&stats);
        printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", ip_str, sequence, ttl, calculate_time_diff(&start, &end));
        record_rtt(&stats, calculate_time_diff(&start, &end));
        sleep(1);
    }
    if(controleC == 0)
    {
       print_final_stats(&stats, ip_addr);
       close(sockfd);
    }
    return 0;
}
