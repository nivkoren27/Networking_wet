//
// Created by nivko on 31/12/2024.
//

#include "Server.h"

using namespace std;

Simulation_Server::Simulation_Server( double handling_rate,  int size) :
      queue(std::queue<double >()), handling_rate(handling_rate), max_size(size), gen(rd()){

}

double Simulation_Server::get_service_time() {
    exponential_distribution<> service_random(handling_rate);
    return service_random(gen);
}

