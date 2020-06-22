#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE 65536

extern char *optarg;  // option pointer
extern int optind;   // option index 

void create_inode_table (std::unordered_map<std::string, std::string> &);
void show_tcp_status (const char* path, std::unordered_map<std::string, std::string> &, std::vector <std::regex> &);
void show_udp_status (const char* path, std::unordered_map<std::string, std::string> &, std::vector <std::regex> &);
bool is_filter_cmd(std::vector <std::regex> &filter, std::string proc_name);
char* find_proc_name (std::string pid);
std::string find_inode_by_symlink (char* symlink);
char* parse_port (char* token, int);
char* parse_address (char* token, int);
bool is_valid_directory (struct dirent *ptr);
int normalize_udp_index (int index);
int normalize_tcp_index (int index);
void parse_cmdline(int argc, char *argv[], int*, int*, std::vector <std::regex> &);

#endif