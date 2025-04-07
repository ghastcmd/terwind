#include "pch.h"
#include "files.h"

void files_opendir(const char* path, files_dir_t* hnd)
{
#ifdef _WIN32
    char s_buff[MAX_PATH];
    strcpy(s_buff, path);
    strcat(s_buff, "\\*");

    files_folder_t ffd;
    *hnd = FindFirstFileA(s_buff, &ffd);
    FindNextFileA(*hnd, &ffd);
#else
    *hnd = opendir(path);
    readdir(*hnd);
    readdir(*hnd);  
#endif
}

bool files_nextfile(files_dir_t hnd, files_folder_t* ffd)
{
#ifdef _WIN32
    return FindNextFileA(hnd, ffd);
#else
    *ffd = readdir(hnd);
    return (bool)*ffd;
#endif
}

void files_close(files_dir_t dir)
{
#ifdef _WIN32
    FindClose(dir);
#else
    closedir(dir);
#endif
}

char* files_getname(files_folder_t* ffd)
{
#ifdef _WIN32
    return ffd->cFileName;
#else
    return (*ffd)->d_name;
#endif
}

void files_list_names(const char* path)
{
    files_dir_t dir;
    files_opendir(path, &dir);

    files_folder_t ffd;
    while(files_nextfile(dir, &ffd))
    {
        register char* str = files_getname(&ffd);
        puts(str);
    }

    files_close(dir);
}

bool files_find_file(const char* path)
{
#ifdef _WIN32
    files_folder_t ffd;
    HANDLE ret = FindFirstFileA(path, &ffd);
    return ret == INVALID_HANDLE_VALUE ? 0 : 1;
#else
    return (bool)opendir(path);
#endif
}

#ifndef DT_DIR
#define DT_DIR 4
#endif

#ifdef _WIN32
bool files_is_folder(files_folder_t* ffd)
#else
bool files_is_folder(const char* path)
#endif
{
#ifdef _WIN32
    return ffd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
#else
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
#endif
}

void files_reset_permissions(const char* path)
{
#ifdef _WIN32
    SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL);
#else
    chmod(path, 0777);
#endif
}

void files_rmdir_r(const char* path);

#ifdef _WIN32
static void files_rm_obj(const char* path, files_folder_t* ffd)
#define _ARG_FILES_RM_OBJ_ ffd
#else
static void files_rm_obj(const char* path)
#define files_rm_obj(path, ffd) files_rm_obj(path)
#define _ARG_FILES_RM_OBJ_ path
#endif
{
    if (files_is_folder(_ARG_FILES_RM_OBJ_))
        files_rmdir_r(path);
    else
    {
        if (remove(path) != 0)
        {
            files_reset_permissions(path);
            remove(path);
        }
    }
}

void files_rm(const char* path)
{
#ifdef _WIN32
    files_folder_t ffd;
    FindFirstFileA(path, &ffd);
    files_rm_obj(path, &ffd);
#else
    files_rm_obj(path, NULL);
#endif
}

void files_rmdir_r(const char* path)
{    
    files_dir_t dir;
    files_opendir(path, &dir);
    
    chdir(path);

    files_folder_t ffd;
    while (files_nextfile(dir, &ffd))
    {
        register char* ffd_name = files_getname(&ffd);
        files_rm_obj(ffd_name, &ffd);
    }

    files_close(dir);
    chdir("..");
    rmdir(path);
}