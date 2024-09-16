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

int is_valid_ipv4(const char *ip_addr)
{
    struct sockaddr_in sa;
    // inet_pton retourne 1 si l'adresse IP est valide, 0 sinon
    int result = inet_pton(AF_INET, ip_addr, &(sa.sin_addr));
    return result == 1;
}

// Fonction pour vérifier si l'entrée est un nom d'hôte valide
int resolve_hostname_to_ip(const char *hostname, char *ip_str, size_t maxlen)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4 uniquement

    int status = getaddrinfo(hostname, NULL, &hints, &res);
    
    if (status == 0) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, maxlen);  // Convertit l'adresse en chaîne de caractères
        freeaddrinfo(res);  // Libérer la mémoire allouée par getaddrinfo
        return 1;  // Nom d'hôte valide avec résolution d'adresse IP
    } else {
        return 0;  // Nom d'hôte invalide ou résolution échouée
    }
}

void parsing(int argc, char **argv, char **ip_addr, char *ip_str, size_t ip_str_len)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            print_help();
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            printf("Verbose mode activated.\n");
        }
        else
        {
            *ip_addr = argv[i];
            if (!is_valid_ipv4(*ip_addr))
            {
                if (!resolve_hostname_to_ip(*ip_addr, ip_str, ip_str_len))
                {
                    printf("ping: %s: Name or service not known\n", *ip_addr);
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
                // Utilisez ip_str_len ici au lieu de sizeof(ip_str)
                strncpy(ip_str, *ip_addr, ip_str_len);
                ip_str[ip_str_len - 1] = '\0'; // Sécurité pour s'assurer que la chaîne est bien terminée
            }
            printf("Resolved IP address: %s\n", ip_str);
        }
    }
}
