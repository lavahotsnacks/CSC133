# CSC133 (TtUu)
Modeling and Simulation
## files:
- `main.cpp`
- `QueueSimulation.cpp`
- `QueueSimulation.h`
(sir jojo's files)
- `RandomGenExpoMean.cpp`
- `RandomGenExpoMean.h`

### `main.cpp`
this file will run simulation scenarios where each has varying number of servers, queues, and arrival/service rates (iA/sD). 9 scenarios will be tested with iA being equal, or greater, or less than sD. it will then output a summary stats for each scenario to a CSV file for further analysis. 
### `QueueSimulation.cpp` / `QueueSimulation.h`
main features that i did here are:
- arrivals always join the shortest queue (if there are multiple queue)
- any free server always serve the customer that arrived the earliest in any queue
- supports multiple scenarios (varying number of servers, queues, and arrival/services rates)
- queues and arrival/services rates are generated using sir Jojo's random function with a given mean.
### `RandomGenExpoMean.cpp` / `RandomGenExpoMean.h`
code is from sir Jojo. original file is a single file but separated into _header_ and _implementation_ files due to compile error in MacOS.
