#include "ft_ping.h"

void print_help(void)
{
    printf("Usage: ft_ping [options] <destination>\n");
    printf("Options:\n");
    printf("  -v       : verbose output\n");
    printf("  -?  ou -h     : display this help message\n");
    printf("Examples:\n");
    printf("  ft_ping 127.0.0.1\n");
}

void print_icmp_err(int type, int code) {
	switch (type) {
	case ICMP_DEST_UNREACH:
		switch(code) {
		case ICMP_NET_UNREACH:
			printf("Destination Net Unreachable\n");
			break;
		case ICMP_HOST_UNREACH:
			printf("Destination Host Unreachable\n");
			break;
		case ICMP_PROT_UNREACH:
			printf("Destination Protocol Unreachable\n");
			break;
		case ICMP_PORT_UNREACH:
			printf("Destination Port Unreachable\n");
			break;
		case ICMP_FRAG_NEEDED:
			printf("Frag needed\n");
			break;
		case ICMP_SR_FAILED:
			printf("Source Route Failed\n");
			break;
		case ICMP_NET_UNKNOWN:
			printf("Destination Net Unknown\n");
			break;
		case ICMP_HOST_UNKNOWN:
			printf("Destination Host Unknown\n");
			break;
		case ICMP_HOST_ISOLATED:
			printf("Source Host Isolated\n");
			break;
		case ICMP_NET_ANO:
			printf("Destination Net Prohibited\n");
			break;
		case ICMP_HOST_ANO:
			printf("Destination Host Prohibited\n");
			break;
		case ICMP_NET_UNR_TOS:
			printf("Destination Net Unreachable for Type of Service\n");
			break;
		case ICMP_HOST_UNR_TOS:
			printf("Destination Host Unreachable for Type of Service\n");
			break;
		case ICMP_PKT_FILTERED:
			printf("Packet filtered\n");
			break;
		case ICMP_PREC_VIOLATION:
			printf("Precedence Violation\n");
			break;
		case ICMP_PREC_CUTOFF:
			printf("Precedence Cutoff\n");
			break;
		default:
			printf("Dest Unreachable, Bad Code: %d\n", code);
			break;
		}
		break;
	case ICMP_SOURCE_QUENCH:
		printf("Source Quench\n");
		break;
	case ICMP_REDIRECT:
		switch(code) {
		case ICMP_REDIR_NET:
			printf("Redirect Network");
			break;
		case ICMP_REDIR_HOST:
			printf("Redirect Host");
			break;
		case ICMP_REDIR_NETTOS:
			printf("Redirect Type of Service and Network");
			break;
		case ICMP_REDIR_HOSTTOS:
			printf("Redirect Type of Service and Host");
			break;
		default:
			printf("Redirect, Bad Code: %d", code);
			break;
		}
		break;
    }
}

// void print_ip_header(struct iphdr *ip_hdr)
// {
//     char src_ip[INET_ADDRSTRLEN], dest_ip[INET_ADDRSTRLEN];
//     inet_ntop(AF_INET, &(ip_hdr->saddr), src_ip, INET_ADDRSTRLEN);
//     inet_ntop(AF_INET, &(ip_hdr->daddr), dest_ip, INET_ADDRSTRLEN);

//     printf("\n--- IP Header ---\n");
//     printf("Version: %d\n", ip_hdr->version);
//     printf("Header Length: %d (in 32-bit words)\n", ip_hdr->ihl);
//     printf("Type Of Service : %d\n", ip_hdr->tos);
//     printf("Total Length: %d\n", ntohs(ip_hdr->tot_len));
//     printf("Identification: 0x%x\n", ntohs(ip_hdr->id));
//     printf("Fragment Offset: %d\n", ip_hdr->frag_off);
//     printf("Time To Live (TTL): %d\n", ip_hdr->ttl);
//     printf("Protocol: %d\n", ip_hdr->protocol);
//     printf("Checksum: %d\n", ip_hdr->check);
//     printf("Source IP: %s\n", src_ip);
//     printf("Destination IP: %s\n", dest_ip);
//     printf("--------------------\n");
// }

// void print_icmp_header(struct icmphdr *icmp_hdr)
// {
//     printf("\n--- ICMP Header ---\n");
//     printf("Type: %d\n", icmp_hdr->type);
//     printf("Code: %d\n", icmp_hdr->code);
//     printf("Checksum: 0x%x\n", ntohs(icmp_hdr->checksum));
//     printf("--------------------\n");
// }

// Fonction pour afficher un octet en hexadécimal
void print_hex_bytes(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 10 == 0) // Format pour 10 valeurs par ligne
            printf("\n");
    }
    printf("\n");
}

// Fonction pour afficher l'en-tête IP de manière détaillée
void print_ip_header(struct iphdr *ip_hdr) {
    char src_ip[INET_ADDRSTRLEN], dest_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_hdr->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_hdr->daddr), dest_ip, INET_ADDRSTRLEN);

    printf("IP Hdr Dump:\n");
    print_hex_bytes((const unsigned char *)ip_hdr, ip_hdr->ihl * 4); // Affiche les octets de l'en-tête IP

    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
    printf(" %d  %d  %02x %04x %04x   %1x %04x  %02x  %02x %04x %-15s %-15s\n",
           ip_hdr->version,                     // Version de l'IP
           ip_hdr->ihl,                         // Longueur de l'en-tête IP
           ip_hdr->tos,                         // Type of Service
           ntohs(ip_hdr->tot_len),              // Longueur totale
           ntohs(ip_hdr->id),                   // Identification
           (ntohs(ip_hdr->frag_off) & 0xE000) >> 13, // Flags
           ntohs(ip_hdr->frag_off) & 0x1FFF,    // Offset de fragmentation
           ip_hdr->ttl,                         // Time to Live
           ip_hdr->protocol,                    // Protocole
           ntohs(ip_hdr->check),                // Checksum de l'en-tête IP
           src_ip,                              // IP source
           dest_ip                              // IP destination
    );
}

// Fonction pour afficher l'en-tête ICMP de manière détaillée
void print_icmp_header(struct icmphdr *icmp_hdr) {
    printf("ICMP: type %d, code %d, size %ld, id 0x%04x, seq 0x%04x\n",
           icmp_hdr->type,                      // Type ICMP (Echo, Echo Reply, etc.)
           icmp_hdr->code,                      // Code ICMP
           sizeof(struct icmphdr),              // Taille de l'en-tête ICMP
           ntohs(icmp_hdr->un.echo.id),         // Identifiant (souvent le PID)
           ntohs(icmp_hdr->un.echo.sequence));  // Numéro de séquence
}
