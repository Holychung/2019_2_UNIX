#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#define True 1
#define False 0

// functional pointer 
typedef FILE *(*fopen_t)(const char*, const char*);
static fopen_t old_fopen = NULL;
typedef DIR *(*opendir_t)(const char*);
static opendir_t old_opendir = NULL;
static int (*__xstat_old)(int, const char*, struct stat*) = NULL;
static int (*old_remove)(const char*) = NULL;
static int (*old_rmdir)(const char*) = NULL;
static int (*old_unlink)(const char*) = NULL;
static int (*old_symlink)(const char*, const char*) = NULL;
static ssize_t (*old_readlink)(const char*, char*, size_t) = NULL;
static int (*old_link)(const char*, const char*) = NULL;
static int (*old_rename)(const char*, const char*) = NULL;
static int (*old_openat)(int, const char*, int) = NULL;
static int (*old_open)(const char*, int) = NULL;
static int (*old_mkdir)(const char*, mode_t) = NULL;
static int (*old_creat)(const char*, mode_t) = NULL;
static int (*old_chown)(const char*, uid_t, gid_t) = NULL;
static int (*old_chdir)(const char*) = NULL;

char *parse_pathname (int, const char*);
int check_path_valid (const char*);
int check_substr (char*, char*);

// int system (const char *command) {
//   fprintf(stderr, "[sandbox] system(%s): not allowed\n", command);
//   return -1;
// }

int execvp (const char *file, char *const argv[]) {
  fprintf(stderr, "[sandbox] execvp(%s): not allowed\n", file);
  return -1;
}

int execve (const char *filename, char *const argv[], char *const envp[]) {
  fprintf(stderr, "[sandbox] execve(%s): not allowed\n", filename);
  return -1;
}

int execv (const char *path, char *const argv[]) {
  fprintf(stderr, "[sandbox] execv(%s): not allowed\n", path);
  return -1;
}

int execlp (const char *file, const char *arg, ...) {
  fprintf(stderr, "[sandbox] execlp(%s): not allowed\n", file);
  return -1;
}

int execle (const char *path, const char *arg, ...) {
  fprintf(stderr, "[sandbox] execle(%s): not allowed\n", path);
  return -1;
}

int execl (const char *path, const char *arg, ...) {
  fprintf(stderr, "[sandbox] execl(%s): not allowed\n", path);
  return -1;
}

int __xstat (int x, const char *filename, struct stat *st) {
  if (check_path_valid(filename)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      __xstat_old = dlsym(handle, "__xstat");
  } else {
    __xstat_old = NULL;
  }
  if (__xstat_old != NULL) {
    int ret = __xstat_old(x, filename, st);
    return ret;
  } else
    fprintf(stderr, "[sandbox] __xstat: access to '%s' is not allowed\n", filename);  
  return -1;
}

int remove (const char *pathname) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_remove = dlsym(handle, "remove");
  } else {
    old_remove = NULL;
  }
  if (old_remove != NULL) 
    if (old_remove(pathname) == 0)
      return 0;
    else {
      fprintf(stderr, "Error remove: %s\n", strerror(errno));
      return -1;
    }
  else
    fprintf(stderr, "[sandbox] remove: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int rmdir (const char *pathname) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_rmdir = dlsym(handle, "rmdir");
  } else {
    old_rmdir = NULL;
  }
  if (old_rmdir != NULL) 
    if (old_rmdir(pathname) == 0)
      return 0;
    else {
      fprintf(stderr, "Error rmdir: %s\n", strerror(errno));
      return -1;
    }
  else
    fprintf(stderr, "[sandbox] rmdir: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int unlink (const char *pathname) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_unlink = dlsym(handle, "unlink");
  } else {
    old_unlink = NULL;
  }
  if (old_unlink != NULL) 
    if (old_unlink(pathname) == 0)
      return 0;
    else {
      fprintf(stderr, "Error unlink: %s\n", strerror(errno));
      return -1;
    }
  else
    fprintf(stderr, "[sandbox] unlink: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int symlink (const char *target, const char *linkpath) {
  if (check_path_valid(target) == False) {
    fprintf(stderr, "[sandbox] symlink: access to '%s' is not allowed\n", target);
    return -1;
  }
  if (check_path_valid(linkpath) == False) {
    fprintf(stderr, "[sandbox] symlink: access to '%s' is not allowed\n", linkpath);
    return -1;
  }
  void *handle = dlopen("libc.so.6", RTLD_LAZY);
  if(handle != NULL)
    old_symlink = dlsym(handle, "symlink");
  else
    old_symlink = NULL;

  if (old_symlink != NULL) {
    if (old_symlink(target, linkpath) == 0) {
      return 0;
    } else {
      fprintf(stderr, "Error symlink: %s\n", strerror(errno));
      return -1;
    }
  }
  return -1;
}

ssize_t readlink (const char *pathname, char *buf, size_t bufsiz) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_readlink = dlsym(handle, "readlink");
  } else {
    old_readlink = NULL;
  }
  if (old_readlink != NULL) {
    ssize_t fd = old_readlink(pathname, buf, bufsiz);
    if (fd == -1) 
      fprintf(stderr, "Error readlink: %s\n", strerror(errno));
    return fd;
  }
  else
    fprintf(stderr, "[sandbox] readlink: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int link (const char *oldpath, const char *newpath) {
  if (check_path_valid(oldpath) == False) {
    fprintf(stderr, "[sandbox] link: access to '%s' is not allowed\n", oldpath);
    return -1;
  }
  if (check_path_valid(newpath) == False) {
    fprintf(stderr, "[sandbox] link: access to '%s' is not allowed\n", newpath);
    return -1;
  }
  void *handle = dlopen("libc.so.6", RTLD_LAZY);
  if(handle != NULL)
    old_link = dlsym(handle, "link");
  else
    old_link = NULL;

  if (old_link != NULL) {
    if (old_link(oldpath, newpath) == 0) {
      return 0;
    } else {
      fprintf(stderr, "Error link: %s\n", strerror(errno));
      return -1;
    }
  }
  return -1;
}

int rename (const char *oldpath, const char *newpath) {
  if (check_path_valid(oldpath) == False) {
    fprintf(stderr, "[sandbox] rename: access to '%s' is not allowed\n", oldpath);
    return -1;
  }
  if (check_path_valid(newpath) == False) {
    fprintf(stderr, "[sandbox] rename: access to '%s' is not allowed\n", newpath);
    return -1;
  }
  void *handle = dlopen("libc.so.6", RTLD_LAZY);
  if(handle != NULL)
    old_rename = dlsym(handle, "rename");
  else
    old_rename = NULL;

  if (old_rename != NULL) {
    if (old_rename(oldpath, newpath) == 0) {
      return 0;
    } else {
      fprintf(stderr, "Error rename: %s\n", strerror(errno));
      return -1;
    }
  }
  return -1;
}

int openat (int dirfd, const char *pathname, int flags, ...) {
  char *real_path = parse_pathname(dirfd, pathname);
  if (check_path_valid(real_path)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_openat = dlsym(handle, "openat");
  } else {
    old_openat = NULL;
  }
  if (old_openat != NULL) {
    int fd = old_openat(dirfd, pathname, flags);
    if (fd == -1) 
      fprintf(stderr, "Error openat: %s\n", strerror(errno));
    return fd;
  }
  else
    fprintf(stderr, "[sandbox] openat: access to '%s' is not allowed\n", real_path);  
  return -1;
}

int open (const char *pathname, int flags, ...) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_open = dlsym(handle, "open");
  } else {
    old_open = NULL;
  }
  if (old_open != NULL) {
    int fd = old_open(pathname, flags);
    if (fd == -1) 
      fprintf(stderr, "Error open: %s\n", strerror(errno));
    return fd;
  }
  else
    fprintf(stderr, "[sandbox] open: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int mkdir (const char *pathname, mode_t mode) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_mkdir = dlsym(handle, "mkdir");
  } else {
    old_mkdir = NULL;
  }
  if (old_mkdir != NULL) 
    return old_mkdir(pathname, mode);
  else
    fprintf(stderr, "[sandbox] mkdir: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int creat (const char *pathname, mode_t mode) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_creat = dlsym(handle, "creat");
  } else {
    old_creat = NULL;
  }
  if (old_creat != NULL) {
    int fd = old_creat(pathname, mode);
    if (fd == -1) 
      fprintf(stderr, "Error creat: %s\n", strerror(errno));
    return fd;
  }
  else
    fprintf(stderr, "[sandbox] creat: access to '%s' is not allowed\n", pathname);  
  return -1;
}

int chown (const char *pathname, uid_t owner, gid_t group) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_chown = dlsym(handle, "chown");
  } else {
    old_chown = NULL;
  }
  if (old_chown != NULL) {
    if (old_chown(pathname, owner, group) == 0)
      return 0;
    else {
      fprintf(stderr, "Error chown: %s\n", strerror(errno));
      return -1;
    }
  }
  else
    fprintf(stderr, "[sandbox] chown: access to '%s' is not allowed\n", pathname);  
  return -1;
}

FILE *fopen (const char *pathname, const char *mode) {
  if (check_path_valid(pathname)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_fopen = (fopen_t)dlsym(handle, "fopen");
  } else {
    old_fopen = NULL;
  }
  if (old_fopen != NULL) {
    FILE *ptr = old_fopen(pathname, mode);
    if (ptr == NULL) {
      fprintf(stderr, "Error fopen: %s\n", strerror(errno));
      return NULL;
    } else
      return ptr;
  } else
    fprintf(stderr, "[sandbox] fopen: access to '%s' is not allowed\n", pathname);
  return NULL;
}

DIR *opendir (const char *name) {
  if (check_path_valid(name)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_opendir = (opendir_t)dlsym(handle, "opendir");
  } else {
    old_opendir = NULL;
  }
  if (old_opendir != NULL) {
    DIR *ptr = old_opendir(name);
    if (ptr == NULL) {
      fprintf(stderr, "Error opendir: %s\n", strerror(errno));
      return NULL;
    } else
      return ptr;
  } else
    fprintf(stderr, "[sandbox] opendir: access to '%s' is not allowed\n", name);
  return NULL;
}

int chdir (const char *path) {
  if (check_path_valid(path)) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle != NULL)
      old_chdir = dlsym(handle, "chdir");
  } else {
    old_chdir = NULL;
  }
  if (old_chdir != NULL) 
    if (old_chdir(path) == 0)
      return 0;
    else {
      fprintf(stderr, "Error chdir: %s\n", strerror(errno));
      return -1;
    }
  else
    fprintf(stderr, "[sandbox] chdir: access to '%s' is not allowed\n", path);  
  return -1;
}

char *parse_pathname (int dirfd, const char *pathname) {
  char *fd = malloc(100*sizeof(char));
  char *path = malloc(100*sizeof(char));
  char *absolute_path = malloc(100*sizeof(char));
  sprintf(fd, "%d", dirfd);
  strcat(path, "/proc/self/fd/");
  strcat(path, fd);
  strcat(path, "/");
  strcat(path, pathname);
  realpath(path, absolute_path);
  return absolute_path;
}

int check_path_valid (const char *pathname) {
  char *path_relative_valid = getenv("PATH_VALID");
  char *path_absolute_valid = malloc(100*sizeof(char));
  char *path_absolute_target = malloc(100*sizeof(char));
  char *exist_1 = realpath(path_relative_valid, path_absolute_valid);
  char *exist_2 = realpath(pathname, path_absolute_target);

  // if (exist_1 == NULL || exist_2 == NULL)
  //   return False;
  if (check_substr(path_absolute_valid, path_absolute_target)) {
    return True;
  }
  return False;
}

int check_substr (char *path_absolute_valid, char *path_absolute_target) {
  char *substr_path = malloc(100*sizeof(char));
  size_t len_path_target = strlen(path_absolute_target);
  size_t len_path_valid= strlen(path_absolute_valid);
  // target path must cover and longer or equal than valid path
  if (len_path_target < len_path_valid)
    return False;
  strncpy(substr_path, path_absolute_target, len_path_valid);
  if (strcmp(substr_path, path_absolute_valid) == 0)
    return True;
  else 
    return False;
}