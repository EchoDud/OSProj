#include <iostream>
#include <string>
#include "process_monitor.h"
#include "process_control.h"
#include "utils.h"

void show_help() {
    std::cout << "Commands:\n"
              << "  list               - List all processes\n"
              << "  info <PID>         - Show detailed info about a process\n"
              << "  kill <PID>         - Kill a process\n"
              << "  stop <PID>         - Stop a process\n"
              << "  cont <PID>         - Continue a process\n"
              << "  filter <criterion> - Filter processes\n"
              << "  sort <criterion>   - Sort processes\n"
              << "  exit               - Exit the program\n";
}

int main() {
    std::string command;
    show_help();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        auto tokens = parse_input(command);
        if (tokens.empty()) continue;

        if (tokens[0] == "list") {
            list_processes();
        } else if (tokens[0] == "info" && tokens.size() == 2) {
            get_process_info(std::stoi(tokens[1]));
        } else if (tokens[0] == "kill" && tokens.size() == 2) {
            kill_process(std::stoi(tokens[1]));
        } else if (tokens[0] == "stop" && tokens.size() == 2) {
            stop_process(std::stoi(tokens[1]));
        } else if (tokens[0] == "cont" && tokens.size() == 2) {
            continue_process(std::stoi(tokens[1]));
        } else if (tokens[0] == "filter" && tokens.size() == 2) {
            filter_processes(tokens[1]);
        } else if (tokens[0] == "sort" && tokens.size() == 2) {
            sort_processes(tokens[1]);
        } else if (tokens[0] == "exit") {
            break;
        } else if (tokens[0] == "help"){
            show_help();
        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }

    return 0;
}
