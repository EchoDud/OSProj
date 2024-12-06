#include "process_control.h"
#include <iostream>
#include <csignal>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void kill_process(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        std::cout << GREEN << "Process " << pid << " killed successfully." << RESET << "\n";
    } else {
        perror(RED "Error killing process" RESET);
    }
}

void stop_process(int pid) {
    if (kill(pid, SIGSTOP) == 0) {
        std::cout << GREEN << "Process " << pid << " stopped successfully." << RESET << "\n";
    } else {
        perror(RED "Error stopping process" RESET);
    }
}

void continue_process(int pid) {
    if (kill(pid, SIGCONT) == 0) {
        std::cout << GREEN << "Process " << pid << " continued successfully." << RESET << "\n";
    } else {
        perror(RED "Error continuing process" RESET);
    }
}
