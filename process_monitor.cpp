#include "process_monitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> // Для сортировки
#include <dirent.h>
#include <iomanip>
#include <unistd.h>
#include <cstring>

// Цвета для терминала
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define BLUE "\033[34m"

struct Process {
    int pid;
    std::string name;
    int ppid;
    std::string state;
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
        proc.name.erase(proc.name.begin());
        proc.name.erase(proc.name.end() - 1);

        processes.push_back(proc);
    }
    closedir(dir);
    return processes;
}

void get_process_info(int pid) {
    std::string status_file = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream file(status_file);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << status_file << "\n";
        return;
    }

    std::cout << "Details for process PID " << pid << ":\n";
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
}

void list_processes() {
    auto processes = get_all_processes();
    std::cout << BLUE << std::left
              << std::setw(10) << "PID"
              << std::setw(25) << "Name"
              << std::setw(10) << "PPID"
              << "State" << RESET << "\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& proc : processes) {
        std::string color = proc.state == "R" ? GREEN : (proc.state == "S" ? YELLOW : RED);
        std::cout << std::left
                  << std::setw(10) << proc.pid
                  << std::setw(25) << proc.name
                  << std::setw(10) << proc.ppid
                  << color << proc.state << RESET << "\n";
    }
}

void filter_processes(const std::string& criterion) {
    auto processes = get_all_processes();
    std::cout << BLUE << "Filtering by: " << criterion << RESET << "\n";

    for (const auto& proc : processes) {
        if (proc.name.find(criterion) != std::string::npos) {
            std::cout << std::left
                      << std::setw(10) << proc.pid
                      << std::setw(25) << proc.name
                      << std::setw(10) << proc.ppid
                      << proc.state << "\n";
        }
    }
}

void sort_processes(const std::string& criterion) {
    auto processes = get_all_processes();

    if (criterion == "pid") {
        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.pid < b.pid;
        });
    } else if (criterion == "name") {
        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.name < b.name;
        });
    } else if (criterion == "ppid") {
        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.ppid < b.ppid;
        });
    } else {
        std::cout << RED << "Unknown sort criterion: " << criterion << RESET << "\n";
        return;
    }

    std::cout << BLUE << "Sorted by: " << criterion << RESET << "\n";
    list_processes();
}
