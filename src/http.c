
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
int generate_request(struct http_request *request, char *request_data){
	char *request_ptr=request_data;

	//first line of request
	request_ptr=strcpy_return_end(request_ptr,GET);
	request_ptr=strcpy_return_end(request_ptr,SPACE);
	request_ptr=strcpy_return_end(request_ptr,request->path);
	request_ptr=strcpy_return_end(request_ptr,SPACE);
	request_ptr=strcpy_return_end(request_ptr,HTTP_V);

	request_ptr=strcpy_return_end(request_ptr,NEW_LINE);

	if(request->accept_parts){
		//E-tag
		request_ptr=strcpy_return_end(request_ptr,VERSION_ID_REQ);
		request_ptr=strcpy_return_end(request_ptr,HEADER_DELIM);
		request_ptr=strcpy_return_end(request_ptr,request->version_id);
		request_ptr=strcpy_return_end(request_ptr,NEW_LINE);

		//ACCEPT-ENCODING
		request_ptr=strcpy_return_end(request_ptr,ACCEPT_ENCODING);
		request_ptr=strcpy_return_end(request_ptr,HEADER_DELIM);
		request_ptr=strcpy_return_end(request_ptr,DELTA_ENCODING);
	}
	return SUCCESS;
}
