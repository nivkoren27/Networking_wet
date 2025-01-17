//
// Created by nivko on 31/12/2024.
//
#include "Server.h"
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include <exception>

enum Event_Type{Arrival, Departure};
struct Event{
    Event_Type eventType;
    double time;
    unsigned int server_num ;

    Event (Event_Type eventType, double time, unsigned int num): eventType(eventType), time(time), server_num(num){}
    bool operator<(const Event& event)const{
        return  this->time > event.time;
    }


};
using namespace std;

double string_to_double(char* string){
    char *end;
    double res = strtod(string, &end);
    if(end == string){
        cerr << "Invalid parameters" << endl;
        return 1;
    }
    return res;

}

int main (int argc, char** argv){

    random_device rd;
    mt19937 gen(rd());

    int requests_serviced = 0;
    int requests_thrown = 0;
    double finish_time = 0;
    double total_service_time = 0;
    double total_waiting_time = 0;

    double sim_time = string_to_double(argv[1]);
    int num_of_servers = 0;
    try {
        num_of_servers = stoi(argv[2]);
    } catch (const invalid_argument &e) {
        cerr << "Invalid argument: " << e.what() << endl;
        return 1;
    } catch (const out_of_range &e) {
        cerr << "Out of range: " << e.what() << endl;
        return 1;
    }
    if (argc - 1 != 3 + 3 * num_of_servers) {
        cerr << "Invalid argument: " << endl;
        return 1;
    }
    vector<double> probabilities;
    for (int i = 0; i < num_of_servers; i++) {
        probabilities.push_back(string_to_double(argv[3 + i]));
    }
    double sum = 0;
    for (int i = 0; i < num_of_servers; i++) {

        sum += probabilities[i];
    }
    if (sum != 1) {
        cerr << "Invalid argument: " << endl;
        return 1;
    }
    double arrival_rate = string_to_double(argv[3 + num_of_servers]);
    vector<unique_ptr<Simulation_Server>> servers;
    for (int i = 0; i < num_of_servers; i++) {
        try {
            servers.push_back(make_unique<Simulation_Server>(string_to_double(argv[4 + 2 * num_of_servers + i]),
                                                             stoi(argv[4 + num_of_servers + i])));
        } catch (const invalid_argument &e) {
            cerr << "Invalid argument: " << e.what() << endl;
            return 1;
        } catch (const out_of_range &e) {
            cerr << "Out of range: " << e.what() << endl;
            return 1;
        }
    }

    std::discrete_distribution<> server_random(probabilities.begin(), probabilities.end());
    exponential_distribution<> arrival_random(arrival_rate);


    double cur_time = 0;
    std::priority_queue<Event, std::vector<Event>> event_queue;
    cur_time = arrival_random(gen);
    int server_number = server_random(gen);
    event_queue.emplace(Arrival, cur_time, server_number);
    while (!event_queue.empty()) {
        Event cur_event = event_queue.top();
        cur_time = cur_event.time;
        event_queue.pop();
        Simulation_Server *server = servers[cur_event.server_num].get();
        if (cur_event.eventType == Arrival) {
            if (!server->is_busy) {
                double service_time = server->get_service_time();
                total_service_time += service_time;
                event_queue.emplace(Departure, cur_time + service_time, cur_event.server_num);
                server->is_busy = true;
            } else {
                if (server->queue.size() < server->max_size) {
                    server->queue.push(cur_time);
                } else {
                    requests_thrown++;
                }
            }
            double arrival_time = arrival_random(gen);
            server_number = server_random(gen);
            if (cur_time + arrival_time < sim_time) {
                event_queue.emplace(Arrival, cur_time + arrival_time, server_number);
            }
        } else {
            finish_time = cur_time; //not sure;
            requests_serviced++;
            if (!server->queue.empty()) {
                double arrive_time = server->queue.front();
                total_waiting_time += cur_time - arrive_time;
                server->queue.pop();
                double service_time = server->get_service_time();
                total_service_time += service_time;
                event_queue.emplace(Departure, cur_time + service_time, cur_event.server_num);
            } else {
                server->is_busy = false;
            }
        }

    }
    double avg_service_time = 0;
    double avg_waiting_time = 0;
    if (requests_serviced) {
        avg_service_time = total_service_time / requests_serviced;
        avg_waiting_time = total_waiting_time / requests_serviced;
    }

    printf("%d %d %.4f %.4f %.4f\n", requests_serviced, requests_thrown, finish_time, avg_waiting_time,
           avg_service_time);


}