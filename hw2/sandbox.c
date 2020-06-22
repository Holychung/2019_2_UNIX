#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>

void lauch_cmd (void);
int parse_cmdline(int argc, char *argv[]);
void print_usage_error (void);

char *system_cmd;

int main(int argc, char **argv) {
  system_cmd = malloc(500*sizeof(char));

  // set default path to ./sandboxso.so and defailt valid path to .
  // env variable only exists until program finish
  setenv("LD_PRELOAD", "./sandboxso.so", 1);
  setenv("PATH_VALID", ".", 1);
  if (argc < 2) {
    printf("no command given.\n");
    return 0;
  }
  if (parse_cmdline(argc, argv) == 0)
    lauch_cmd();

  /* Test Field */
  /* add LD_PRELOAD=./sandboxso.so before execute on cmdline */
  
  // char *test_valid_path = "./Makefile";
  // char *test_invalid_path = "../test.py";

  // fstat
  // struct stat stStatBuf;
  // int fd = 0;
  // fd = open("./Makefile", O_RDONLY);
  // if (fstat(fd, &stStatBuf) != -1)
  //   printf("suc1\n");
  // fd = open("../test.py", O_RDONLY);
  // if (fstat(fd, &stStatBuf) == -1)
  //   printf("suc2\n");
  // close(fd);
  // readlink
  // setenv("PATH_VALID", "/", 1);
  // char *symlink = "/proc/self/exe";
  // char *buf = malloc(100);
  // if (readlink(symlink, buf, 1000) != -1) {
  //   printf("buf: %s\n", buf);
  //   printf("suc1\n");
  // }
  // link
  // if (link(".", "./test") == 0)
  //   printf("suc1\n");
  // if (link("..", "./test/a") == -1)
  //   printf("suc2\n");
  // if (link(".", "./test") == -1)
  //   printf("suc3\n");
  // openat
  // int fd = open(".", O_RDONLY);
  // if (openat(fd, "Makefile", O_RDONLY) != -1)
  //   printf("suc1\n");
  // if (openat(fd, "../test.py", O_RDONLY) == -1)
  //   printf("suc2\n");
  // rename
  // if (rename("./test.py", "./test.py") == 0)
  //   printf("suc1\n");
  // if (rename("../test.py", "../test.py") == -1)
  //   printf("suc2\n");
  // if (rename("./non_exist.py", "./non_exist.py") == -1)
  //   printf("suc3\n");

  // open
  // if (open(test_valid_path, O_RDONLY) != -1)
  //   printf("suc1\n");
  // if (open(test_invalid_path, O_RDONLY) == -1)
  //   printf("suc2\n");

  // mkdir 
  // if (mkdir("./foo", S_IREAD) == 0)
  //   printf("suc1\n");
  // if (mkdir("../foo", S_IREAD) == -1)
  //   printf("suc2\n");

  // creat 
  // if (creat("./foo", O_CREAT) != -1)
  //   printf("suc1\n");
  // if (creat("../foo", O_CREAT) == -1)
  //   printf("suc2\n");

  // chown // user holy 1000 group holy 1000
  // int uid = 1000;
  // int gid = 1000;
  // if (chown(test_valid_path, uid, gid) == 0)
  //   printf("suc1\n");
  // if (chown(test_invalid_path, uid, gid) == -1)
  //   printf("suc2\n");

  // fopen
  // FILE *p = fopen("../test.py", "r");
  // if (p != NULL)
  //   printf("success\n");
  // else 
  //   printf("fail\n");

  // chdir
  // if (chdir(".") == 0)
  //   printf("suc1\n");
  // if (chdir("/") == -1)
  //   printf("suc2\n");
  // if (chdir("./foo/foo") == -1)
  //   printf("suc3\n");

  return 0;
}

void lauch_cmd (void) {
  char *ld_path = getenv("LD_PRELOAD");
  // printf("%s\n", ld_path);
  char *cmd = malloc(500*sizeof(char));
  strcat(cmd, "LD_PRELOAD=");
  strcat(cmd, ld_path);
  strcat(cmd, " ");
  strcat(cmd, system_cmd);

  if (system(cmd) < -1) {
    printf("system command error\n");
  }
}

int parse_cmdline(int argc, char *argv[]) {
  int ret = 0;
  int is_sandbox = 1;
  for (int i = 1; i < argc; i++) {
    if (is_sandbox) {
      if (argv[i][0] == '-') {
        if (strcmp(argv[i], "-d") == 0) {
          if (i+1 < argc) {
            // set valid base directory
            setenv("PATH_VALID", argv[i+1], 1);
            i++;
            continue;
          } else {
            print_usage_error();
            ret = -1;
          }
        } else if (strcmp(argv[i], "-p") == 0) {
          if (i+1 < argc) {
            // set path to sandboxso.so
            setenv("LD_PRELOAD", argv[i+1], 1);
            i++;
            continue;
          } else {
            print_usage_error();
            ret = -1;
          }
        } else if (strcmp(argv[i], "--") == 0) {
          is_sandbox = 0;
          continue;
        } else {
          // invalid options
          fprintf(stderr ,"invalid option -- \'%c\'\n", argv[i][1]);
          print_usage_error();
          ret = -1;
        }
      } else {
        // is_command
        strcat(system_cmd, argv[i]);
        strcat(system_cmd, " ");
      }
    } else {
      // is_command
      strcat(system_cmd, argv[i]);
      strcat(system_cmd, " ");
    }
  }
  return ret;
}

void print_usage_error (void) {
  printf("usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n");
  printf("        -p: set the path to sandbox.so, default = ./sandbox.so\n");
  printf("        -d: the base directory that is allowed to access, default = .\n");
  printf("        --: separate the arguments for sandbox and for the executed command\n");
}

