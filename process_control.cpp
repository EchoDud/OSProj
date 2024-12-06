#include "process_control.h"
#include <iostream>
#include <csignal>

void kill_process(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        std::cout << "Process " << pid << " killed successfully.\n";
    } else {
        perror("Error killing process");
    }
}

void stop_process(int pid) {
    if (kill(pid, SIGSTOP) == 0) {
        std::cout << "Process " << pid << " stopped successfully.\n";
    } else {
        perror("Error stopping process");
    }
}

void continue_process(int pid) {
    if (kill(pid, SIGCONT) == 0) {
        std::cout << "Process " << pid << " continued successfully.\n";
    } else {
        perror("Error continuing process");
    }
}
