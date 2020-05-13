#pragma once

#ifdef _WIN32
typedef HANDLE files_dir_t;
typedef WIN32_FIND_DATA files_folder_t;

#else
typedef DIR* files_dir_t;
typedef struct dirent* files_folder_t;

#endif

void files_opendir(const char* path, files_dir_t* hnd);

bool files_nextfile(files_dir_t hnd, files_folder_t* ffd);

void files_close(files_dir_t dir);

char* files_getname(files_folder_t* ffd);

void files_list_names(const char* path);

bool files_find_file(const char* path);

#ifdef _WIN32
bool files_is_folder(files_folder_t* ffd);
#else
bool files_is_folder(const char* path);
#endif

void files_reset_permissions(const char* path);

void files_rmdir_r(const char* path);

void files_rm(const char* path);