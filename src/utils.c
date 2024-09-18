#include "ft_ping.h"



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

void parsing(int argc, char **argv, PingStats *stats)
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
            stats->verbose = 1;
            printf("Verbose mode activated.\n");
        }
        else if(argv[i][0] != '-')
        {
            stats->ip_addr = argv[i];

            if (!is_valid_ipv4(stats->ip_addr))
            {
                if (!resolve_hostname_to_ip(stats->ip_addr, stats->ip_str, sizeof(stats->ip_str)))
                {
                    printf("ping: %s: Name or service not known\n", stats->ip_addr);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                // Copie l'IP directement dans le champ `ip_str` de la structure PingStats
                strncpy(stats->ip_str, stats->ip_addr, sizeof(stats->ip_str));
                stats->ip_str[sizeof(stats->ip_str) - 1] = '\0'; // Assurez-vous que la chaîne est bien terminée
            }
        }
        else{
            print_help();
            exit(EXIT_SUCCESS);
        }

    }
}

long calculate_time_diff_total(struct timespec *start, struct timespec *end)
{
    return ((end->tv_sec - start->tv_sec) * 1000) + ((end->tv_nsec - start->tv_nsec) / 1000000);
}
