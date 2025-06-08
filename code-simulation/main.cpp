#include <iostream>
#include <fstream>
#include "QueueSimulation.h"

int main()
{
    double sim_time = 10000.0;
    size_t max_queue_length = 5;

    // test values for mean interarrival (iA) and mean service (sD)
    struct IASD
    {
        double iA; // mean interarrival
        double sD; // mean service
        std::string label;
    } cases[] = {
        {2.0, 1.0, "iA> sD"}, // underloaded
        {0.5, 1.0, "iA< sD"}, // overloaded
        {1.0, 1.0, "iA==sD"} // balanced
    };

    std::ofstream csv("simulation_results.csv");
    csv << "Scenario,Servers,Queues,MeanInterarrival,MeanService,Label,TotalArrivals,TotalDepartures,TotalRejected,AverageQueueLength,ServerUtilization,MaxQueueLengthObserved,AverageWaitingTime\n";

    int scenario_num = 1;
    for (size_t servers = 1; servers <= 3; ++servers)
    {
        for (size_t queues = 1; queues <= 3; ++queues)
        {
            for (const auto& c : cases)
            {
                QueueSimulation sim(
                    c.iA, // mean interarrival
                    c.sD, // mean service
                    sim_time,
                    servers, // number of servers
                    queues, // number of queues
                    max_queue_length // max number of waiting arrivals per queue
                );
                sim.run();

                // stats
                csv << "Scenario " << scenario_num << ","
                    << servers << ","
                    << queues << ","
                    << c.iA << ","
                    << c.sD << ","
                    << c.label << ","
                    << sim.getTotalArrivals() << ","
                    << sim.getTotalDepartures() << ","
                    << sim.getTotalRejected() << ","
                    << sim.getAverageQueueLength() << ","
                    << sim.getServerUtilization() << ","
                    << sim.getMaxQueueLengthObserved() << ","
                    << sim.getAverageWaitingTime() << "\n";

                ++scenario_num;
            }
        }
    }

    csv.close();
    std::cout << "batch sim completed. results exported to simulation_results.csv\n";
    return 0;
}