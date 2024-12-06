#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>

void list_processes();
void get_process_info(int pid);
void filter_processes(const std::string& criterion);
void sort_processes(const std::string& criterion);

#endif // PROCESS_MONITOR_H
