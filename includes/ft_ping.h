#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <netdb.h>
#include <math.h>
#include <limits.h>

typedef struct {
    int packets_transmitted;
    int packets_received;
    long total_time;  // Temps total pour tous les paquets (en ms)
    double rtt_min;
    double rtt_max;
    double rtt_sum;
    double rtt_sum_squared;
} PingStats;

// Fonction pour afficher l'aide (-?)
void print_help(void);

// Fonction pour créer la socket brute ICMP
int create_socket(void);
void create_icmp_packet(struct icmphdr *icmp_hdr, int sequence);
void send_icmp_packet(int sockfd, const char *ip_addr, struct icmphdr *icmp_hdr);
void receive_icmp_reply(int sockfd, int *ttl);
unsigned short checksum(void *b, int len);
int is_valid_ipv4(const char *ip_addr);
int resolve_hostname_to_ip(const char *hostname, char *ip_str, size_t maxlen);
void parsing(int argc, char **argv, char **ip_addr, char *ip_str, size_t ip_str_len);
void set_socket_ttl(int sockfd, int ttl);

// Initialisation des statistiques
void init_stats(PingStats *stats);
void record_rtt(PingStats *stats, double rtt);
void packet_transmitted(PingStats *stats);
void packet_received(PingStats *stats);
void print_final_stats(PingStats *stats, const char *ip_addr);
double calculate_time_diff(struct timespec *start, struct timespec *end);

long calculate_time_diff_total(struct timespec *start, struct timespec *end);

#endif