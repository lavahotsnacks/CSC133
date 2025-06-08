#include "QueueSimulation.h"
#include <iostream>
#include <algorithm>
#include <limits>

size_t QueueSimulation::getTotalArrivals() const { return arrivals; }
size_t QueueSimulation::getTotalDepartures() const { return departures; }
size_t QueueSimulation::getTotalRejected() const { return total_rejected; }
double QueueSimulation::getAverageQueueLength() const { return (time_end > 0 ? total_queue_time / time_end : 0); }
double QueueSimulation::getServerUtilization() const { return (servers_total * time_end > 0 ? total_busy_time / (servers_total * time_end) : 0); }
size_t QueueSimulation::getMaxQueueLengthObserved() const { return max_queue_length_observed; }
double QueueSimulation::getAverageWaitingTime() const { return (departures > 0 ? total_waiting_time / departures : 0); }

QueueSimulation::QueueSimulation(
    double mean_interarrival,
    double mean_service,
    double sim_time_end,
    size_t num_servers,
    size_t num_queues,
    size_t max_queue_length
)
    : time_end(sim_time_end),
      servers_total(num_servers),
      queues_total(num_queues),
      max_queue_length_param(max_queue_length),
      generate_interarrival(mean_interarrival),
      generate_service(mean_service),
      now(0.0),
      arrivals(0),
      departures(0),
      servers_busy(1, 0),
      queue_lines(num_queues),
      total_queue_time(0.0),
      last_event_time(0.0),
      total_busy_time(0.0),
      max_queue_length_observed(0),
      total_waiting_time(0.0),
      total_rejected(0)
{
    // schedule the first arrival for queue 0
    double first_arrival = now + generate_interarrival.getValue();
    events.push({first_arrival, EventType::ARRIVAL, 0});
}

void QueueSimulation::run()
{
    while (!events.empty())
    {
        Event event_current = events.top();
        if (event_current.time > time_end)
        {
            break;
        }
        events.pop();
        double time_since_last = event_current.time - last_event_time;
        size_t total_queue = 0;
        for (const auto& q : queue_lines)
        {
            total_queue += q.size();
        }
        total_queue_time += total_queue * time_since_last;
        total_busy_time += servers_busy[0] * time_since_last;
        max_queue_length_observed = std::max(max_queue_length_observed, total_queue);

        now = event_current.time;
        last_event_time = now;

        if (event_current.type == EventType::ARRIVAL)
        {
            handleArrival(event_current);
        }
        else
        {
            handleDeparture(event_current);
        }
    }
}

void QueueSimulation::handleArrival(const Event& event)
{
    arrivals++;
    size_t qid = event.queue_id;

    // schedule next arrival (assign to the shortest queue)
    double next_arrival_time = now + generate_interarrival.getValue();
    if (next_arrival_time <= time_end)
    {
        size_t next_qid = 0;
        size_t min_size = queue_lines[0].size();
        for (size_t i = 1; i < queues_total; ++i)
        {
            if (queue_lines[i].size() < min_size)
            {
                min_size = queue_lines[i].size();
                next_qid = i;
            }
        }
        events.push({next_arrival_time, EventType::ARRIVAL, next_qid});
    }

    // shared server pool, any free server serves earliest in any queue
    if (servers_busy[0] < servers_total)
    {
        // server available, serve immediately
        servers_busy[0]++;
        double departure_time = now + generate_service.getValue();
        events.push({departure_time, EventType::DEPARTURE, qid});
    }
    else
    {
        // check for queue capacity
        if (queue_lines[qid].size() < max_queue_length_param)
        {
            queue_lines[qid].push(now);
        }
        else
        {
            total_rejected++;
        }
    }
}

void QueueSimulation::handleDeparture(const Event& event)
{
    departures++;

    // find the earliest customer in any queue
    size_t min_qid = queues_total;
    double min_arrival = std::numeric_limits<double>::max();
    for (size_t i = 0; i < queues_total; ++i)
    {
        if (!queue_lines[i].empty() && queue_lines[i].front() < min_arrival)
        {
            min_arrival = queue_lines[i].front();
            min_qid = i;
        }
    }

    if (min_qid < queues_total)
    {
        // serve the earliest customer from any queue
        queue_lines[min_qid].pop();
        total_waiting_time += (now - min_arrival);

        double departure_time = now + generate_service.getValue();
        events.push({departure_time, EventType::DEPARTURE, min_qid});
        // servers_busy[0] remains unchanged (server stays busy)
    }
    else
    {
        // no one is waiting in any queue, so a server becomes idle
        servers_busy[0]--;
    }
}

void QueueSimulation::printStatistics() const
{
    std::cout << "\nsimulation ended at time: " << now << "\n";
    std::cout << "total arrivals: " << arrivals << "\n";
    std::cout << "total departures: " << departures << "\n";
    std::cout << "total rejected: " << total_rejected << "\n";
    std::cout << "average queue length: "
              << (time_end > 0 ? total_queue_time / time_end : 0) << "\n";
    std::cout << "server utilization: "
              << (servers_total * time_end > 0 ? total_busy_time / (servers_total * time_end) : 0) << "\n";
    std::cout << "max queue length observed: " << max_queue_length_observed << "\n";
    std::cout << "average waiting time: "
              << (departures > 0 ? total_waiting_time / departures : 0) << "\n";
}