
#include "tcp_socket.h"
#include "constants.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "string.h"
#include "netinet/in.h"
#include "time.h"
#include "dirent.h"
#include "netdb.h"
#include <arpa/inet.h>

int connect_to_server(char *server_ip,int server_port){
	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server_ip);
	servaddr.sin_port = htons(server_port);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		return FAILURE;
	}
	return sockfd;
}

int recv_response(int client_socket, char *recv_data, int buffer_size){
	return recv(client_socket, recv_data, buffer_size, 0);
}

int send_request(int client_socket, char *send_data){
	return send(client_socket, send_data, strlen(send_data), 0);
}

int close_connection(int client_socket){
	return close(client_socket);
}
