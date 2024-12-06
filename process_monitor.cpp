#include "process_monitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <iomanip>
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

// Получение списка всех процессов
std::vector<Process> get_all_processes() {
    std::vector<Process> processes;
    DIR* dir = opendir("/proc");
    if (!dir) {
        std::cerr << RED << "Error: Unable to open /proc directory" << RESET << "\n";
        return processes;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (!isdigit(entry->d_name[0])) continue;

        int pid = std::stoi(entry->d_name);
        std::ifstream stat_file("/proc/" + std::to_string(pid) + "/stat");
        if (!stat_file.is_open()) {
            std::cerr << YELLOW << "Warning: Unable to open /proc/" << pid << "/stat" << RESET << "\n";
            continue;
        }

        Process proc{};
        stat_file >> proc.pid >> proc.name >> proc.state >> proc.ppid;

        if (!proc.name.empty() && proc.name.front() == '(' && proc.name.back() == ')') {
            proc.name = proc.name.substr(1, proc.name.size() - 2); // Удаление скобок
        }

        processes.push_back(proc);
    }

    closedir(dir);
    return processes;
}

// Вывод всех процессов
void list_processes() {
    auto processes = get_all_processes();
    if (processes.empty()) {
        std::cout << YELLOW << "No processes found." << RESET << "\n";
        return;
    }

    std::cout << BLUE << std::left
              << std::setw(10) << "PID"
              << std::setw(25) << "Name"
              << std::setw(10) << "PPID"
              << "State" << RESET << "\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& proc : processes) {
        std::string color = proc.state == "R" ? GREEN : (proc.state == "S" ? YELLOW : RED);
        std::string truncated_name = proc.name.size() > 24 ? proc.name.substr(0, 24) + "..." : proc.name;

        std::cout << std::left
                  << std::setw(10) << proc.pid
                  << std::setw(25) << truncated_name
                  << std::setw(10) << proc.ppid
                  << color << proc.state << RESET << "\n";
    }
}

// Получение подробной информации о процессе
void get_process_info(int pid) {
    std::ifstream file("/proc/" + std::to_string(pid) + "/status");
    if (!file.is_open()) {
        std::cerr << RED << "Error: Could not open /proc/" << pid << "/status" << RESET << "\n";
        return;
    }

    std::cout << "Details for process PID " << pid << ":\n";
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
}

std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void filter_processes(const std::string& criterion) {
    auto processes = get_all_processes();
    std::vector<Process> filtered;

    std::string lower_criterion = to_lower(criterion);
    for (const auto& proc : processes) {
        if (to_lower(proc.name).find(lower_criterion) != std::string::npos) {
            filtered.push_back(proc);
        }
    }

    if (filtered.empty()) {
        std::cout << YELLOW << "No processes found matching: " << criterion << RESET << "\n";
        return;
    }

    std::cout << BLUE << "Filtering by: " << criterion << RESET << "\n";
    std::cout << std::left
              << std::setw(10) << "PID"
              << std::setw(25) << "Name"
              << std::setw(10) << "PPID"
              << "State" << "\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& proc : filtered) {
        std::string color = proc.state == "R" ? GREEN : (proc.state == "S" ? YELLOW : RED);
        std::cout << std::left
                  << std::setw(10) << proc.pid
                  << std::setw(25) << proc.name
                  << std::setw(10) << proc.ppid
                  << color << proc.state << RESET << "\n";
    }
}


// Сортировка процессов
void sort_processes(const std::string& criterion) {
    auto processes = get_all_processes();
    if (processes.empty()) {
        std::cout << YELLOW << "No processes to sort." << RESET << "\n";
        return;
    }

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
        std::cerr << RED << "Unknown sort criterion: " << criterion << RESET << "\n";
        return;
    }

    std::cout << BLUE << "Sorted by: " << criterion << RESET << "\n";
    std::cout << std::left
              << std::setw(10) << "PID"
              << std::setw(25) << "Name"
              << std::setw(10) << "PPID"
              << "State" << "\n";
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

