#pragma once
#include <queue>
#include <vector>
#include "RandomGenExpoMean.h"

enum class EventType
{
    ARRIVAL,
    DEPARTURE
};

struct Event
{
    double time;
    EventType type;
    size_t queue_id;
};

struct EventCompare
{
    bool operator()(const Event& a, const Event& b) const
    {
        return a.time > b.time;
    }
};

class QueueSimulation
{
public:
    QueueSimulation(
        double mean_interarrival,
        double mean_service,
        double sim_time_end,
        size_t num_servers = 1,
        size_t num_queues = 1,
        size_t max_queue_length = SIZE_MAX // unlimited by default
    );

    void run();
    void printStatistics() const;

    size_t getTotalArrivals() const;
    size_t getTotalDepartures() const;
    size_t getTotalRejected() const;
    double getAverageQueueLength() const;
    double getServerUtilization() const;
    size_t getMaxQueueLengthObserved() const;
    double getAverageWaitingTime() const;

private:
    // parameters for sim
    double time_end;
    size_t servers_total;
    size_t queues_total;
    size_t max_queue_length_param;

    // sir jojo's random function
    RandomExpoMean generate_interarrival;
    RandomExpoMean generate_service;

    double now;
    size_t arrivals;
    size_t departures;
    std::vector<size_t> servers_busy;
    std::vector<std::queue<double>> queue_lines;
    std::priority_queue<Event, std::vector<Event>, EventCompare> events;

    // stats
    double total_queue_time;
    double last_event_time;
    double total_busy_time;
    size_t max_queue_length_observed;
    double total_waiting_time;
    size_t total_rejected;

    void handleArrival(const Event& event);
    void handleDeparture(const Event& event);
};