#include "ft_ping.h"

void print_help(void)
{
    printf("Usage: ft_ping [options] <destination>\n");
    printf("Options:\n");
    printf("  -v       : verbose output\n");
    printf("  -?       : display this help message\n");
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

void print_ip_header(struct iphdr *ip_hdr)
{
    char src_ip[INET_ADDRSTRLEN], dest_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_hdr->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_hdr->daddr), dest_ip, INET_ADDRSTRLEN);

    printf("\n--- IP Header ---\n");
    printf("Version: %d\n", ip_hdr->version);
    printf("Header Length: %d (in 32-bit words)\n", ip_hdr->ihl);
    printf("Type Of Service : %d\n", ip_hdr->tos);
    printf("Total Length: %d\n", ntohs(ip_hdr->tot_len));
    printf("Identification: 0x%x\n", ntohs(ip_hdr->id));
    printf("Fragment Offset: %d\n", ip_hdr->frag_off);
    printf("Time To Live (TTL): %d\n", ip_hdr->ttl);
    printf("Protocol: %d\n", ip_hdr->protocol);
    printf("Checksum: %d\n", ip_hdr->check);
    printf("Source IP: %s\n", src_ip);
    printf("Destination IP: %s\n", dest_ip);
    printf("--------------------\n");
}

void print_icmp_header(struct icmphdr *icmp_hdr)
{
    printf("\n--- ICMP Header ---\n");
    printf("Type: %d\n", icmp_hdr->type);
    printf("Code: %d\n", icmp_hdr->code);
    printf("Checksum: 0x%x\n", ntohs(icmp_hdr->checksum));
    printf("--------------------\n");
}