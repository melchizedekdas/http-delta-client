
#include "http.h"
#include "http_response.h"
#include "resource.h"
#include "constants.h"
#include<string.h>
#include<stdlib.h>

#define RANGE_START "["
#define RANGE_END "]"
#define RANGE_SEPARATOR "-"
#define SIZE_START "("
#define SIZE_END ")"

struct delta_list{
	long byte_start, byte_end;
	char *content;
	struct delta_list *next;
};
int process_delta(char *old_cache,char *delta,char *new_cache);
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

int tokenize_delta(char *delta, struct delta_list **delta_head){
	delta_head=0;
	struct delta_list *previous_node=0, *current_node;
	char *delta_pointer=delta, *content;
	long size,i;
	while(*delta_pointer){
		current_node=(struct delta_list *)malloc(sizeof(struct delta_list));
		strtok_r(delta_pointer, RANGE_START, &delta_pointer);
		current_node->byte_start=atol(strtok_r(delta_pointer, RANGE_SEPARATOR, &delta_pointer));
		current_node->byte_end=atol(strtok_r(delta_pointer, RANGE_END, &delta_pointer));
		strtok_r(delta_pointer, SIZE_START, &delta_pointer);
		size=atol(strtok_r(delta_pointer, SIZE_END, &delta_pointer));
		current_node->content=(char *)malloc(sizeof(char)*(size+1));
		for(i=0;i<size;i++){
			current_node->content[i]=*delta_pointer++;
		}
		current_node->content[size]=0;
		current_node->next=0;
		if(previous_node){
			previous_node->next=current_node;
			previous_node=previous_node->next;
		}
		else{
			previous_node=current_node;
			delta_head=current_node;
		}
	}
	return SUCCESS;
}

int process_delta(char *old_cache,char *delta,char *new_cache){
	new_cache=(char *)malloc(sizeof(char)*BUFFER_SIZE);
	struct delta_list *delta_list;
	if(tokenize_delta(delta,&delta_list)){
		return FAILURE;
	}
	int old_cache_pointer=0, new_cache_pointer=0, delta_content_pointer=0;
	while(delta_list){
		while(old_cache_pointer<=delta_list->byte_start){
			new_cache[new_cache_pointer++]=old_cache[old_cache_pointer++];
		}
		delta_content_pointer=0;
		while(delta_list->content[delta_content_pointer]){
			new_cache[new_cache_pointer++]=delta_list->content[delta_content_pointer++];
		}
		old_cache_pointer=delta_list->byte_end;

		delta_list=delta_list->next;
	}
	while(old_cache[old_cache_pointer]){
		new_cache[new_cache_pointer++]=old_cache[old_cache_pointer++];
	}
	new_cache[new_cache_pointer]=0;
	return SUCCESS;
}
