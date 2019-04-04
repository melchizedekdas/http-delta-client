//returns success or error code
int get_local_version_id(char *resource_path, char *version_id);

//returns success or error code
int save_current_version_id(char *resource_path, char *version_id);

//returns number of bytes fetched or -1 for error
int get_resource(char *resource_path, char *resource_data);

//returns success or error code
int save_resource(char *resource_path, char *resource_data);

//returns success or error code
int delete_resource(char *resource_path);

//returns success or error code
int delete_meta_resource(char *resource_path);
