
//returns client socket_fd
int connect_to_server(char *server_ip,int server_port);

//returns the number of bytes transmitted, or -1 on failure
int recv_response(int client_socket, char *recv_data, int buffer_size);

//returns the number of bytes received, or -1 on failure.
int send_request(int client_socket, char *send_data);

//returns success or error code
int close_connection(int client_socket);
