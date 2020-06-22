#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include<string.h>
#include <arpa/inet.h>
#include <string>
#include <unordered_map>
#include <regex>

#include "utils.h"

void create_inode_table (std::unordered_map<std::string, std::string> &Hash_inode_table) {
  DIR *DIR_layer1, *DIR_layer2;
  struct dirent *dirent_layer1, *dirent_layer2;
  const char *path_proc = "/proc";
  
  DIR_layer1 = opendir(path_proc);
  if (DIR_layer1 == NULL) {
    fprintf(stderr,"Ls: can not open %s", path_proc);
  } else {
    while ((dirent_layer1 = readdir(DIR_layer1)) != NULL) {
      if (is_valid_directory(dirent_layer1)) {
        char *file_path = (char*)malloc(100);
        sprintf(file_path, "%s/%s/fd", path_proc, dirent_layer1->d_name);
        DIR_layer2 = opendir(file_path);
        while ((dirent_layer2 = readdir(DIR_layer2)) != NULL) {
          if (dirent_layer2->d_type == DT_LNK) {
            char* link_path = (char*)malloc(100);
            char* symlink = (char*)malloc(100);
            sprintf(link_path, "%s/%s", file_path, dirent_layer2->d_name);
            int result = readlink(link_path, symlink, 100);
            if (result == -1)
              printf("cannot read link %s\n", link_path);
            else {
              symlink[result] = '\0';
              // printf("symlink: %s\n", symlink);
              // printf("process: %s\n", dirent_layer1->d_name);
              // printf("inode: %s\n", inode);
              std::string inode = find_inode_by_symlink(symlink);
              Hash_inode_table[inode] = dirent_layer1->d_name;
            }
          }
        }
      }
    }
  }
  closedir(DIR_layer1);
  closedir(DIR_layer2);
}

void show_tcp_status (const char* path, std::unordered_map<std::string, std::string> &Hash_inode_table, std::vector <std::regex> &filter) {
  FILE *fp;
  char *buf;
  size_t ret;
  const char *delim = "\t\r\n ";
  int ROW_COUNT = 17;
  fp = fopen(path, "rb");
  buf = (char*)malloc(BUFFER_SIZE);
  ret = fread(buf, 1, BUFFER_SIZE, fp);
  // printf("%s\n", buf); // print "/proc/net/tcp" data
  fclose(fp);

  char* local_ip = (char*)malloc(10);
  char* local_port = (char*)malloc(10);
  char* rem_ip = (char*)malloc(10);
  char* rem_port = (char*)malloc(10);
  std::string inode;
  std::string pid;
  char* proc_name = (char*)malloc(50);
  int mode;

  if (strcmp(path, "/proc/net/tcp") == 0) 
    mode = AF_INET;
  else
    mode = AF_INET6;
  
  char *token = strtok(buf, delim);
  for (int i = 1; token != NULL; i++) {
    if (i > 12) {
      int index = normalize_tcp_index(i);
      // local_address
      if (index % ROW_COUNT == 2) { 
        local_ip = parse_address(token, mode);
        local_port = parse_port(token, mode);
      }
      // rem_address
      if (index % ROW_COUNT == 3) { 
        rem_ip = parse_address(token, mode);
        rem_port = parse_port(token, mode);
      }
      // inode
      if (index % ROW_COUNT == 10) { 
        inode = token;
        auto result = Hash_inode_table.find(inode);
        if (result != Hash_inode_table.end()) {
          pid = result->second.c_str();
          proc_name = find_proc_name(pid);
          if (is_filter_cmd(filter, std::string(proc_name))) {
            char* local_addr = (char*)malloc(20);
            char* rem_addr = (char*)malloc(20);
            char* pid_pname = (char*)malloc(50);
            sprintf(local_addr, "%s:%s", local_ip, local_port);
            sprintf(rem_addr, "%s:%s", rem_ip, rem_port);
            sprintf(pid_pname, "%s/%s", pid.c_str(), proc_name);
            if (mode == AF_INET)
              printf("%-8s%-20s%-20s%-30s\n", "tcp", local_addr, rem_addr, pid_pname);
            else
              printf("%-8s%-20s%-20s%-30s\n", "tcp6", local_addr, rem_addr, pid_pname);
          }
        } else {
          printf("Not found tcp\n");
        }
      }
    }
    token = strtok(NULL, delim);
  }
}

void show_udp_status (const char* path, std::unordered_map<std::string, std::string> &Hash_inode_table, std::vector <std::regex> &filter) {
  FILE *fp;
  char *buf;
  size_t ret;
  const char *delim = " \t\r\n ";
  int ROW_COUNT = 13;
  fp = fopen(path, "rb");
  buf = (char*)malloc(BUFFER_SIZE);
  ret = fread(buf, 1, BUFFER_SIZE, fp);
  // printf("%s\n", buf); // print "/proc/net/udp" data
  fclose(fp);

  char* local_ip = (char*)malloc(10);
  char* local_port = (char*)malloc(10);
  char* rem_ip = (char*)malloc(10);
  char* rem_port = (char*)malloc(10);
  std::string inode;
  std::string pid;
  char* proc_name = (char*)malloc(50);
  int mode;

  if (strcmp(path, "/proc/net/udp") == 0) 
    mode = AF_INET;
  else
    mode = AF_INET6;  
  
  char *token = strtok(buf, delim);
  for (int i = 1; token != NULL; i++) {
    if (i > 15) {
      int index = normalize_udp_index(i);
      // local_address
      if (index % ROW_COUNT == 2) { 
        local_ip = parse_address(token, mode);
        local_port = parse_port(token, mode);
      }
      // rem_address
      if (index % ROW_COUNT == 3) { 
        rem_ip = parse_address(token, mode);
        rem_port = parse_port(token, mode);
      }
      // inode
      if (index % ROW_COUNT == 10) { 
        inode = std::string(token);
        auto result = Hash_inode_table.find(inode);
        if (result != Hash_inode_table.end()) {
          pid = result->second.c_str();
          proc_name = find_proc_name(pid);
          if (is_filter_cmd(filter, std::string(proc_name))) {
            char* local_addr = (char*)malloc(20);
            char* rem_addr = (char*)malloc(20);
            char* pid_pname = (char*)malloc(50);
            sprintf(local_addr, "%s:%s", local_ip, local_port);
            sprintf(rem_addr, "%s:%s", rem_ip, rem_port);
            sprintf(pid_pname, "%s/%s", pid.c_str(), proc_name);
            if (mode == AF_INET)
              printf("%-8s%-20s%-20s%-30s\n", "udp", local_addr, rem_addr, pid_pname);
            else
              printf("%-8s%-20s%-20s%-30s\n", "udp6", local_addr, rem_addr, pid_pname);
          }
        } else {
          printf("Not found udp\n");
        }
      }
    }
    token = strtok(NULL, delim);
  }
}

bool is_filter_cmd(std::vector <std::regex> &filter, std::string proc_name) {
  if (filter.size() == 0)
    return true;
  for (auto &reg_filter_cmd: filter)
    if (std::regex_match(proc_name.begin(), proc_name.end(), reg_filter_cmd))
      return true;
  return false;
}

char* find_proc_name (std::string pid) {
  FILE *fp;
  char *file_path = (char*)malloc(100);
  char *proc_name;
  size_t ret;
  sprintf(file_path, "/proc/%s/comm", pid.c_str());
  fp = fopen(file_path, "rb");
  proc_name = (char*)malloc(100);
  ret = fread(proc_name, 1, 100, fp);

  // remove new line character
  int length = 0;
  while(proc_name[length] != '\0') {
    if (proc_name[length] == '\n') {
      proc_name[length] = '\0';
      break;
    }
    length++;
  }
  return proc_name;
}

std::string find_inode_by_symlink (char* symlink) {
  std::string inode;
  std::string s = std::string(symlink);
  unsigned int inode_tmp;
  std::regex reg("socket:\[[0-9]*]"); // ex: socket:[123]
 
  if (std::regex_match(s.begin(), s.end(), reg))
    sscanf(s.substr(8, s.length()-9).c_str(), "%u", &inode_tmp);
  inode = std::to_string(inode_tmp);
  return inode;
}

char* parse_port (char* token, int mode) {
  char* port = (char*)malloc(10);
  unsigned port_x;
  unsigned addr[4];
  if (mode == AF_INET)
    sscanf(token, "%x:%x", &addr[0], &port_x);
  else 
    sscanf(token, "%8x%8x%8x%8x:%x", &addr[0], &addr[1], &addr[2], &addr[3], &port_x);
  if(port_x == 0)
    sprintf(port, "*");
  else 
    sprintf(port, "%d", port_x);

  return port;
}

char* parse_address (char* token, int mode) {
  unsigned addr[4];
  char* addr_tmp = (char*)malloc(50);
  int mode_max_len;

  if (mode == AF_INET) {
    mode_max_len = INET_ADDRSTRLEN;
    sscanf(token, "%x:", &addr[0]);
  } else {
    mode_max_len = INET6_ADDRSTRLEN;
    sscanf(token, "%8x%8x%8x%8x:", &addr[0], &addr[1], &addr[2], &addr[3]);
  }
  inet_ntop(mode, &addr[0], addr_tmp, mode_max_len);

  return addr_tmp;
}

bool is_valid_directory (struct dirent *ptr) {
  if (ptr->d_type == DT_DIR 
        && strcmp(ptr->d_name, ".") != 0 
        && strcmp(ptr->d_name, "..") != 0
        && ptr->d_name[0] > 47 
        && ptr->d_name[0] < 58) {
      return true;
  }
  return false;
}

int normalize_udp_index (int index) {
  return index - 15 - (((index - 15) / 13) * 13);
}

int normalize_tcp_index (int index) {
  return index - 12 - (((index - 12) / 17) * 17);
}

void parse_cmdline(int argc, char *argv[], int *flag_tcp, int *flag_udp, std::vector <std::regex> &filter) {
  int result;
  int longindex = 0; // null
  const char *optstring = "tu";
  
  static struct option long_options[] = {
    {"tcp", no_argument, NULL, 't'},
    {"udp", no_argument, NULL, 'u'},
  };

  // parse filter cmd
  for(int i = 1; i < argc; i++) {
    if(argv[i][0]!='-') {
      std::string reg_filter_cmd;
      reg_filter_cmd = ".*";
      reg_filter_cmd = reg_filter_cmd + argv[i] + ".*";
      filter.push_back(std::regex(reg_filter_cmd.c_str()));
    }
  }

  while ((result = getopt_long(argc, argv, optstring, long_options, &longindex)) != -1) {
    switch (result) {
      case 't':
        *flag_tcp = 1;
        break;
      case 'u':
        *flag_udp = 1;
        break;
      default:
        break;
    }
  }
}

