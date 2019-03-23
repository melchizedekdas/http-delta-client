
#include "http.h"
#include "constants.h"
#include<string.h>

#define NEW_LINE "\n"
#define COLON ":"
#define SPACE " "
#define HEADER_DELIM ": "
#define ENCODING_DELIM ", "
#define GET "GET"
#define HTTP_V "HTTP/1.1"
#define VERSION_ID_REQ "If-None-Match"
#define ACCEPT_ENCODING "Accept-Encoding"
#define DELTA_ENCODING "Delta"
#define CONTENT_LENGTH "Content-Length"
#define E_TAG "ETag"
#define LENGTH_BUFFER 10

int parse_response(struct http_response *response, char *response_data){
	//server code below.. needs to be implemented.


	char *overall_save_ptr;
	//parse first line of request
	if(strcmp(GET,strtok_r(request_data, SPACE, &overall_save_ptr))){
		//1st token of first line is method name. only GET is supported.
		return BAD_REQUEST;
	}
	//2nd token of first line is path
	request->path=strtok_r(overall_save_ptr, SPACE, &overall_save_ptr);
	if(strcmp(HTTP_V,strtok_r(overall_save_ptr, NEW_LINE, &overall_save_ptr))){
		//3rd token of first line is http version number. only http/1.1 is supported.
		return BAD_REQUEST;
	}
	//parse headers
	char *header = strtok_r(overall_save_ptr, NEW_LINE, &overall_save_ptr);
	char *header_key, *header_value, *header_save_ptr;

	request->accept_parts=false;
	while (header != NULL) {
		//code to parse header line
		if(strlen(header)==0){
			//Empty line. Implies header over
			break;
		}
		header_key=strtok_r(header, HEADER_DELIM, &header_save_ptr);
		header_value=strtok_r(header_save_ptr, HEADER_DELIM, &header_save_ptr);
		//Use header key and value
		if(!strcmp(header_key,VERSION_ID_REQ)){
			//Version id header
			request->version_id=header_value;
		}
		else if(!strcmp(header_key,ACCEPT_ENCODING)){
			//accept encoding header
			char *encoding_save_ptr;
			char *encoding=strtok_r(header_value, ENCODING_DELIM, &encoding_save_ptr);
			while (encoding != NULL) {
				//Process each encoding
				if(!strcmp(encoding,DELTA_ENCODING)){
					//accepts delta encoding
					request->accept_parts=true;
				}
				encoding=strtok_r(encoding_save_ptr, ENCODING_DELIM, &encoding_save_ptr);
			}
		}
		header = strtok_r(overall_save_ptr, NEW_LINE, &overall_save_ptr);
	}
	return SUCCESS;
}
char *strcpy_return_end(char *dest, char *source){
	int i;
	for(i=0;source[i];i++){
		dest[i]=source[i];
	}
	return dest+i;
}
char *status_msg(char *status_code){
	if(!strcmp(status_code,RESOURCE_FOUND)){
		return "OK";
	}
	else if(!strcmp(status_code,SAME_VERSION)){
		return "Not Modified";
	}
	else if(!strcmp(status_code,PARTS_FOUND)){
		return "Delta Encoding";
	}
	else if(!strcmp(status_code,RESOURCE_NOT_FOUND)){
		return "Not found";
	}
	else if(!strcmp(status_code,BAD_REQUEST)){
		return "Bad request";
	}
	else{
		return "Internal Error";
	}
}
int generate_request(struct http_request *request, char *request_data){
	//server code below.. needs to be implemented.


	char *response_ptr=response_data, *status_code=response->status_code;
	response_ptr=strcpy_return_end(response_ptr,HTTP_V);
	response_ptr=strcpy_return_end(response_ptr,SPACE);
	response_ptr=strcpy_return_end(response_ptr,status_code);
	response_ptr=strcpy_return_end(response_ptr,SPACE);
	response_ptr=strcpy_return_end(response_ptr,status_msg(status_code));
	response_ptr=strcpy_return_end(response_ptr,NEW_LINE);

	if(!strcmp(status_code,RESOURCE_FOUND)){
		//Content length header
		response_ptr=strcpy_return_end(response_ptr,CONTENT_LENGTH);
		response_ptr=strcpy_return_end(response_ptr,COLON);
		char content_length_str[LENGTH_BUFFER];
		itoa(response->content_length, content_length_str, 10);
		response_ptr=strcpy_return_end(response_ptr,content_length_str);
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);

		//E_TAG header
		response_ptr=strcpy_return_end(response_ptr,E_TAG);
		response_ptr=strcpy_return_end(response_ptr,COLON);
		response_ptr=strcpy_return_end(response_ptr,response->version_id);
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);

		//Body
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);
		response_ptr=strcpy_return_end(response_ptr,response->body);
		*response_ptr=0;
	}
	else if(!strcmp(status_code,SAME_VERSION)){
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);
	}
	else if(!strcmp(status_code,PARTS_FOUND)){
		//Content length header
		response_ptr=strcpy_return_end(response_ptr,CONTENT_LENGTH);
		response_ptr=strcpy_return_end(response_ptr,COLON);
		char content_length_str[LENGTH_BUFFER];
		itoa(response->content_length, content_length_str, 10);
		response_ptr=strcpy_return_end(response_ptr,content_length_str);
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);

		//E_TAG header
		response_ptr=strcpy_return_end(response_ptr,E_TAG);
		response_ptr=strcpy_return_end(response_ptr,COLON);
		response_ptr=strcpy_return_end(response_ptr,response->version_id);
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);

		//Body
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);
		response_ptr=strcpy_return_end(response_ptr,response->body);
		*response_ptr=0;
	}
	else if(!strcmp(status_code,RESOURCE_NOT_FOUND)){
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);
	}
	else if(!strcmp(status_code,BAD_REQUEST)){
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);
	}
	else{
		response_ptr=strcpy_return_end(response_ptr,NEW_LINE);
	}

	return SUCCESS;
}
