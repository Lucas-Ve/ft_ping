# ft_ping

This project implements a simplified version of the `ping` command using **ICMP** (Internet Control Message Protocol) and **raw sockets** in C. The program sends ICMP Echo Request packets to a specified IP address and waits for ICMP Echo Reply packets in response. It calculates statistics such as the Round-Trip Time (RTT) and handles various ICMP errors, mimicking the behavior of the standard `ping` tool.

## Table of Contents
- [What is ICMP?](#what-is-icmp)
- [What is a Raw Socket?](#what-is-a-raw-socket)
- [Project Overview](#project-overview)
- [How to Compile and Run](#how-to-compile-and-run)
- [Key Functions](#key-functions)

## What is ICMP?
The **Internet Control Message Protocol** (ICMP) is primarily used for sending error messages and operational information indicating success or failure when communicating with another IP address. It is a vital part of the Internet Protocol suite and is used by tools like `ping` and `traceroute`.

### ICMP Types Used:
- **Type 8**: ICMP Echo Request – Sent to request a response from a host.
- **Type 0**: ICMP Echo Reply – Response to an Echo Request.
- **Type 11**: Time Exceeded – Sent when the packet's TTL (Time To Live) expires.

## What is a Raw Socket?
A **raw socket** allows direct sending and receiving of IP packets without any protocol-specific transport layer processing by the operating system. This provides greater control over network communication, but typically requires superuser privileges to use.

Raw sockets are used here to directly send and receive ICMP packets, as they bypass the TCP/UDP layer and allow us to work directly with lower-level protocols like ICMP.

## Project Overview
This project consists of several functions to create raw sockets, build ICMP packets, send them over the network, and receive responses.

### Workflow:
1. **Create a raw socket** using `socket()` function.
2. **Construct an ICMP Echo Request packet** using the `create_icmp_packet()` function.
3. **Send the packet** using the `send_icmp_packet()` function.
4. **Receive the ICMP reply** using the `receive_icmp_reply()` function.
5. Handle errors such as timeouts or ICMP Time Exceeded (TTL expired) messages.

## How to Compile and Run
To compile the program, you can use `make`:

```bash
make
```
Note: You must run the program with root privileges to create raw sockets:

`sudo ./ft_ping <IP_ADDRESS>`

`sudo ./ft_ping -h`

`sudo ./ft_ping <IP_ADDRESS> -v`

## Key Functions

### 1. `int create_socket()`
This function creates a raw socket with the `AF_INET` family, `SOCK_RAW` type, and the `IPPROTO_ICMP` protocol. If the socket creation fails, it exits the program with an error message.

### 2. `unsigned short checksum(void *b, int len)`
Calculates the checksum required by ICMP packets to verify the integrity of the data. It sums up the data and inverts the result to create the checksum.

### 3. `void create_icmp_packet(struct icmphdr *icmp_hdr, int sequence)`
Constructs an ICMP Echo Request packet, sets the type to `8` (Echo Request), and calculates the checksum.

### 4. `void send_icmp_packet(int sockfd, const char *ip_addr, struct icmphdr *icmp_hdr)`
Sends an ICMP packet over the raw socket to the specified IP address.

### 5. `void receive_icmp_reply(int sockfd, int *ttl, PingStats *stats)`
Receives ICMP replies and handles errors (such as TTL exceeded) and timeouts. If verbose mode is enabled, it prints the IP and ICMP headers of the received packet.
