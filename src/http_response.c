
#include "http.h"
#include "http_response.h"
#include "resource.h"
#include "constants.h"

int process_response(struct http_request *request, struct http_response *response){
	if(strcmp(response->status_code,BAD_REQUEST)){
		return FAILURE;
	}
	if(strcmp(response->status_code,RESOURCE_NOT_FOUND)){
		if(!request->accept_parts){
			return FAILURE;
		}
		delete_resource(request->path);
		return FAILURE;
	}
	if(strcmp(response->status_code,SAME_VERSION)){
		return SUCCESS;
	}
	if(strcmp(response->status_code,RESOURCE_FOUND)){
		if(save_resource(request->path,response->body)){
			return FAILURE;
		}
		if(save_current_version_id(request->path,response->version_id)){
			return FAILURE;
		}
		return SUCCESS;
	}
	if(strcmp(response->status_code,PARTS_FOUND)){
		char *old_cache,*new_cache;
		if(get_resource(request->path,old_cache)){
			return FAILURE;
		}
		if(process_delta(old_cache,response->body,new_cache)){
			return FAILURE;
		}
		if(save_resource(request->path,new_cache)){
			return FAILURE;
		}
		if(save_current_version_id(request->path,response->version_id)){
			return FAILURE;
		}
		return SUCCESS;
	}
	return FAILURE;
}
int process_delta(char *old_cache,char *delta,char *new_cache){
	//to be implemented

	return SUCCESS;
}
