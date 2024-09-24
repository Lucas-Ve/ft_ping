#include "ft_ping.h"

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("Error creating raw socket");
        exit(1);
    }
    return sockfd;
}

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void create_icmp_packet(struct icmphdr *icmp_hdr, int sequence)
{
    icmp_hdr->type = ICMP_ECHO;  // Type 8: Echo Request
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = getpid();  // Utilisez l'ID du processus comme identifiant
    icmp_hdr->un.echo.sequence = sequence;
    icmp_hdr->checksum = 0;  // Initialisez à 0 avant de calculer la somme de contrôle
    icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr));  // Calculez la somme de contrôle
}


void send_icmp_packet(int sockfd, const char *ip_addr, struct icmphdr *icmp_hdr)
{
    // int setttl = 1;
    // if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &setttl, sizeof(setttl)) < 0)
    // {
    //     perror("Error setting TTL");
    //     exit(1);
    // }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_addr);

    if (sendto(sockfd, icmp_hdr, sizeof(*icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error sending ICMP packet");
        exit(1);
    }
}

void receive_icmp_reply(int sockfd, int *ttl, PingStats *stats)
{
    stats->time_out = 0;
    stats->ttl = 0;
    char buf[1024];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Définir un timeout de 2 secondes
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

    if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len) < 0)
    {
        stats->time_out = 1;
        return;
    }

    struct iphdr *ip_hdr = (struct iphdr *)buf;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(buf + (ip_hdr->ihl * 4));
    *ttl = ip_hdr->ttl;
    stats->errorCount = 0;
    // Vérifier s'il y a une erreur ICMP
    printf("iddddd %d %d\n\n", icmp_hdr->un.echo.id, getpid());
    if (icmp_hdr->type != ICMP_ECHOREPLY && icmp_hdr->type != 1 && icmp_hdr->type != 8)  // Si ce n'est pas une réponse de type Echo ou ICMP de type 1 (Destination Unreachable)
    {
        stats->error++;
        stats->errorCount = 1;

        // Afficher l'en-tête IP et ICMP si verbose est activé
        if (stats->verbose == 1)
        {
            printf("\nError:");
            print_ip_header(ip_hdr);   // Afficher l'en-tête IP
            print_icmp_header(icmp_hdr);  // Afficher l'en-tête ICMP
        }

        // Gestion des erreurs spécifiques
        if (icmp_hdr->type == 11)  // ICMP Time Exceeded
        {
            stats->ttl = 1;
        }
        else
        {
            print_icmp_err(icmp_hdr->type, icmp_hdr->code);  // Affiche le message d'erreur spécifique
        }
    }
    else if (checksum(icmp_hdr, sizeof(struct icmphdr)) != 0) {
        stats->errorCount = 1;  // Marquer l'erreur de checksum
        return;
    }
    else if (icmp_hdr->un.echo.id != getpid()) {
        stats->errorCount = 1;  // Marquer l'erreur de PID
        return;
    }
}