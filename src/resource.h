//returns current version id of the resource or null pointer if resource doesn't exist
char *get_local_version_id(char *resource_path);

//returns success or error code
int *save_current_version_id(char *resource_path, char *version_id);

//returns number of bytes fetched or -1 for error
int get_resource(char *resource_path, char *resource_data);

//returns number of bytes fetched or -1 for error
int save_resource(char *resource_path, char *resource_data);
