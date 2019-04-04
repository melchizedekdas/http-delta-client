
#include <stdio.h>
#include "tcp_socket.h"
#include "http.h"
#include "http_response.h"
#include "constants.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(){
	int client_socket, bytes_received;
	char request_data[BUFFER_SIZE], response_data[BUFFER_SIZE];
	struct http_request request;
	struct http_response response;

	client_socket=connect_to_server(SERVER_IP,SERVER_PORT);
	if(client_socket==FAILURE){
		printf("Error connecting to server");
		return FAILURE;
	}

	char *path="index";

	//char *local_version_id=get_local_version_id(path);

	char *local_version_id=(char *)malloc(sizeof(char)*10);
	if(get_local_version_id(path,local_version_id)==FAILURE){
		local_version_id=0;
	}

	request.path=path;
	if(local_version_id!=NULL){
		request.accept_parts=true;
		request.version_id=local_version_id;
	}
	else{
		request.accept_parts=false;
	}
	if(generate_request(&request,request_data)==FAILURE){
		printf("Error creating request");
		return FAILURE;
	}

	//for debug start
	printf("[request begin]\n%s\n[request end]\n",request_data);
	fflush(0);
	//for debug end

	if(send_request(client_socket, request_data)==FAILURE){
		printf("Error sending request");
		return FAILURE;
	}

	if(recv_response(client_socket, response_data,BUFFER_SIZE)==FAILURE){
		printf("Error receiving response");
		return FAILURE;
	}

	//for debug start
	printf("[response begin]\n%s\n[response end]\n",response_data);
	fflush(0);
	//for debug end

	if(parse_response(&response, response_data)==FAILURE){
		printf("Error parsing response");
		return FAILURE;
	}

	if(close_connection(client_socket)==FAILURE){
		printf("Error closing connection");
		return FAILURE;
	}

	if(process_response(&request,&response)==FAILURE){
		printf("Error processing response");
		return FAILURE;
	}
	return SUCCESS;
}
