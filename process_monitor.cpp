#include "process_monitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <iomanip> 

struct Process {
    int pid;
    std::string name;
    int ppid;
    std::string state;
    float cpu_usage;
    float memory_usage;
};

std::vector<Process> get_all_processes() {
    std::vector<Process> processes;
    DIR* dir = opendir("/proc");
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        if (!isdigit(entry->d_name[0])) continue;

        int pid = std::stoi(entry->d_name);
        std::ifstream stat_file("/proc/" + std::to_string(pid) + "/stat");
        if (!stat_file.is_open()) continue;

        Process proc{};
        stat_file >> proc.pid >> proc.name >> proc.state >> proc.ppid;
        proc.name.erase(proc.name.begin()); // Удаляем первую скобку (
        proc.name.erase(proc.name.end() - 1); // Удаляем последнюю скобку )
        
        processes.push_back(proc);
    }
    closedir(dir);
    return processes;
}

void list_processes() {
    auto processes = get_all_processes();
    std::cout << std::left // Выравнивание по левому краю
              << std::setw(10) << "PID"
              << std::setw(25) << "Name"
              << std::setw(10) << "PPID"
              << "State" << "\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& proc : processes) {
        std::cout << std::left
                  << std::setw(10) << proc.pid
                  << std::setw(25) << proc.name
                  << std::setw(10) << proc.ppid
                  << proc.state << "\n";
    }
}

void get_process_info(int pid) {
    std::ifstream stat_file("/proc/" + std::to_string(pid) + "/status");
    if (!stat_file.is_open()) {
        std::cout << "Process with PID " << pid << " not found.\n";
        return;
    }

    std::string line;
    std::cout << "Details for PID " << pid << ":\n";
    while (std::getline(stat_file, line)) {
        std::cout << line << "\n";
    }
}

void filter_processes(const std::string& criterion) {
    std::cout << "Filtering by " << criterion << " is not implemented yet.\n";
}

void sort_processes(const std::string& criterion) {
    std::cout << "Sorting by " << criterion << " is not implemented yet.\n";
}
