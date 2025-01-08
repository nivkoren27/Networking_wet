//
// Created by nivko on 31/12/2024.


#ifndef NETWORKING_QUEUE_H
#define NETWORKING_QUEUE_H

#include <queue>
#include <random>

class Simulation_Server {

private:


    std::random_device rd;  // Seed generator
    std::mt19937 gen;

public:
    double handling_rate;
    unsigned int max_size;
    bool is_busy = false;
    std::queue<double> queue;
    Simulation_Server(double handling_rate,  int size);

    double get_service_time();

};


#endif //NETWORKING_QUEUE_H
