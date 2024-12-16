#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <iomanip>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
};

void GenerateProcesses(std::vector<Process>& processes, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distArrival(0, 10);
    std::uniform_int_distribution<> distBurst(1, 10);
    std::uniform_int_distribution<> distPriority(1, 5);

    for (int i = 0; i < count; ++i) {
        processes.push_back({ i + 1, distArrival(gen), distBurst(gen), distPriority(gen), 0 });
        processes.back().remainingTime = processes.back().burstTime;
    }
}

void PrintProcesses(const std::vector<Process>& processes) {
    std::cout << "ID\tArrival\tBurst Time\tPriority\n";
    for (const auto& process : processes) {
        std::cout << process.id << "\t" << process.arrivalTime << "\t\t" << process.burstTime << "\t\t" << process.priority << "\n";
    }
}

void RoundRobin(std::vector<Process> processes, int quantum) {
    std::cout << "\n--- Round Robin Algorithm ---\n";
    int time = 0;
    std::queue<Process*> readyQueue;

    while (!processes.empty() || !readyQueue.empty()) {
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrivalTime <= time) {
                readyQueue.push(&(*it));
                it = processes.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            Process* current = readyQueue.front();
            readyQueue.pop();

            int executeTime = std::min(current->remainingTime, quantum);
            current->remainingTime -= executeTime;
            time += executeTime;

            std::cout << "Process " << current->id << " is executing: remaining time " << current->remainingTime << "\n";

            if (current->remainingTime > 0) {
                readyQueue.push(current);
            }
        }
        else {
            ++time;
        }
    }
}

void FCFS(std::vector<Process> processes) {
    std::cout << "\n--- FCFS Algorithm ---\n";
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
        });

    int currentTime = 0;
    int totalWaitTime = 0;

    for (const auto& process : processes) {
        int waitTime = std::max(0, currentTime - process.arrivalTime);
        totalWaitTime += waitTime;
        currentTime = std::max(currentTime, process.arrivalTime) + process.burstTime;

        std::cout << "Process " << process.id << " start: " << currentTime - process.burstTime << ", end: " << currentTime << ", waiting: " << waitTime << "\n";
    }

    std::cout << "Average waiting time: " << (double)totalWaitTime / processes.size() << "\n";
}

void PriorityScheduling(std::vector<Process> processes) {
    std::cout << "\n--- Priority Scheduling ---\n";
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.priority < b.priority || (a.priority == b.priority && a.arrivalTime < b.arrivalTime);
        });

    int currentTime = 0;

    for (const auto& process : processes) {
        currentTime = std::max(currentTime, process.arrivalTime) + process.burstTime;

        std::cout << "Process " << process.id << " priority: " << process.priority << ", end: " << currentTime << "\n";
    }
}

int main() {
    std::vector<Process> processes;
    GenerateProcesses(processes, 5);

    std::cout << "--- Generated Processes ---\n";
    PrintProcesses(processes);

    RoundRobin(processes, 3);

    GenerateProcesses(processes, 5);
    FCFS(processes);

    GenerateProcesses(processes, 5);
    PriorityScheduling(processes);

    return 0;
}
