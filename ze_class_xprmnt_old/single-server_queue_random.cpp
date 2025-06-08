/*
March 20, 2025 (start)
May 6, 2025 (last revision)

notes to future me:
- still need to add statistics (use the given from the PDF)
- try other data structures for events 
(instead of PQ, maybe bst or sorted linked list? consider time complexities)
- try class (OOP) instead of struct for event?
- is the time in second or ms? idk
*/

#include <iostream>
#include <queue>
#include <vector>
#include "RandomGenExpoMean.h"

using namespace std;

enum class EventType { ARRIVAL, DEPARTURE };

struct Event
{
	double time;
	EventType type;
};

struct EventCompare
{
	bool operator()(Event const& a, Event const& b) const
	{
		return a.time > b.time;
	}
};

int main()
{
	// random interarrival time and service time using sir Jojo's rando func.
	RandomExpoMean generate_interarrival(4.0);
	RandomExpoMean generate_service(5.0);
	
	const double time_end = 1000; // end of simulation or DEAD END!
	
	double now = 0.0;
	bool busy = false;
	int departures = 0;
	int arrivals = 0;
	queue<double> queue_line;
	
	priority_queue<Event, vector<Event>, EventCompare> events;
	double time_arrival_next = now + generate_interarrival.getValue();
	events.push({time_arrival_next, EventType::ARRIVAL});
	
	while (!events.empty())
	{
		Event event_current = events.top();
		if (event_current.time > time_end)
			break;
		events.pop();
		now = event_current.time;
		
		if (event_current.type == EventType::ARRIVAL)
		{
			arrivals++;
			double arrival_next = now + generate_interarrival.getValue();
			if (arrival_next <= time_end) events.push({ arrival_next, EventType::ARRIVAL });
		
			if (!busy)
			{
				busy = true;
				double departure_next = now + generate_service.getValue();
				events.push({ departure_next, EventType::DEPARTURE});
			}
			else queue_line.push(now);

		}
		else
		{
			departures++;
			if (!queue_line.empty())
			{
				queue_line.pop();
				events.push({ now + generate_service.getValue(), EventType::DEPARTURE});
			}
			else busy = false;
		}
		
		cout 
		<< "time=" << now << ", \t\t"
		<< (event_current.type == EventType::ARRIVAL ? "ARRIVAL" : "DEPARTURE") << ", \t\t"
		<< "busy=" << (busy ? "yes" : "no") << ", \t\t"
		<< "queue_length=" << queue_line.size() << "\n";
	}
	
	cout 
	<< "\n"
	<< now << " <- Time of the last served\n"
	<< time_end << " <- Dead end, stop simulation\n"
	<< departures << " <- Number of departures or served\n"
	<< arrivals << " <- Number of arrivals\n";
	
	return 0;
}