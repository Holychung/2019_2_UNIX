#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include <unordered_map>
#include <regex>
#include <iostream>

#include "utils.h"

int main(int argc, char **argv)
{
  int flag_tcp = 0;
  int flag_udp = 0;
  const char *path_tcp = "/proc/net/tcp";
  const char *path_tcp_6 = "/proc/net/tcp6";
  const char *path_udp = "/proc/net/udp";
  const char *path_udp_6 = "/proc/net/udp6";
  std::unordered_map<std::string, std::string> Hash_inode_table;
  std::vector <std::regex> filter;
  
  parse_cmdline(argc, argv, &flag_tcp, &flag_udp, filter);
  create_inode_table(Hash_inode_table);

  // for( const auto& n : Hash_inode_table ) 
  //   std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";

  if (flag_tcp) {
    printf("List of TCP connections:\n");
    printf("%-8s%-20s%-20s%-30s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
    show_tcp_status(path_tcp, Hash_inode_table, filter);
    show_tcp_status(path_tcp_6, Hash_inode_table, filter);
  }
  if (flag_tcp && flag_udp)
    printf("\n");
  if (flag_udp) {
    printf("List of UDP connections:\n");
    printf("%-8s%-20s%-20s%-30s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
    show_udp_status(path_udp, Hash_inode_table, filter);
    show_udp_status(path_udp_6, Hash_inode_table, filter);
  }
  
  return 0;
}

