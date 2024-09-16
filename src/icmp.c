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
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_addr);

    if (sendto(sockfd, icmp_hdr, sizeof(*icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error sending ICMP packet");
        exit(1);
    }
}


void receive_icmp_reply(int sockfd, int *ttl)
{
    char buf[1024];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len) < 0)
    {
        perror("Error receiving ICMP reply");
        exit(1);
    }

    struct iphdr *ip_hdr = (struct iphdr *)buf;
    // struct icmphdr *icmp_hdr = (struct icmphdr *)(buf + (ip_hdr->ihl * 4));
    *ttl = ip_hdr->ttl;

    // if (icmp_hdr->type == ICMP_ECHOREPLY)
    // {
    //     printf("Received ICMP Echo Reply from %s\n", inet_ntoa(addr.sin_addr));
    // }
    // else
    // {
    //     printf("Received unexpected ICMP packet\n");
    // }
}