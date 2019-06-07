// Copyright (c) 2019, Matthew Chalifoux.
#include <iostream>
#include <cstdlib>
#include "queue.cpp"
#include <cassert>
using namespace std;

class averager{
public:
    // CONSTRUCTOR
    averager();
    // MODIFICATION MEMBER FUNCTION
    void next_number(double value);
    // CONSTANT MEMBER FUNCTIONS
    std::size_t how_many_numbers() const {return count;}
    double average() const;
    void addtoCrash();
    unsigned int getCrashCount() {return crash;}
private:
    std::size_t count;
    double sum;
    unsigned int crash;
};

class bool_source{
public:
    // CONSTRUCTOR
    bool_source(double pl);
    // CONSTANT MEMBER FUNCTION
    bool query() const;
private:
    double probability; // Probability of query returning true
};
class runway{
public:
    // CONSTRUCTOR
    runway(unsigned int,unsigned int);
    // MODIFICATION MEMBER FUNCTIONS
    void one_minute();
    void start_runwayLanding();
    void start_runwayTakeoff();
    // CONSTANT MEMBER FUNCTION
    bool is_busy() const {return (runway_time_left) > 0;}
private:
    unsigned int seconds_for_runwayLanding;   // Minutes for a single runway Land
    unsigned int seconds_for_runwayTakeoff;   // Minutes for a single runwau Takeoff
    unsigned int runway_time_left;            // Minutes left till runway is clear
};
void airport_runway_simulate(unsigned int time_to_land, unsigned int time_to_take_off,
                             double landing_probability, double takeoff_probability,
                             unsigned int fuel_limit, unsigned int simulation_time);
int main()
{
    airport_runway_simulate(5,15,.1,.08,20,1440);
    return 0;
}

void airport_runway_simulate(unsigned int time_to_land, unsigned int time_to_take_off,
                             double landing_probability, double takeoff_probability,
                             unsigned int fuel_limit, unsigned int simulation_time)
{

    Queue<unsigned int> arrival_timesLanding;       // Time stamps of the planes waiting to land
    Queue<unsigned int> arrival_timesTakeoff;       // Time stamps of the planes waiting to takeoff
    unsigned int nextLanding;                       // A value taken from the queue
    unsigned int nextTakeoff;                       // A calue taken from the queu
    bool_source arrivalLanding(landing_probability);// Probability of plane arriving at landing queue
    bool_source arrivalTakeoff(takeoff_probability);// Probability of plane arriving at takeoff queue
    runway RunWay(time_to_land, time_to_take_off);  // Runway constructor
    averager wait_timesLanding;                     // Averager for wait times of planes in landing queue
    averager wait_timesTakeoff;                     // Averager for wait times of planes in takeoff queue
    unsigned int current_minute;                    // current minute of time
    unsigned int cntLandingplanesleft = 0;          // Count of planes left in landing queue when sim over
    unsigned int cntTakeoffplanesleft = 0;          // Count of planes left in takeoff queue when sim over
    // Write the parameters to cout
    cout << "Minutes for one plane to land : " << time_to_land << endl;
    cout << "Minutes for one plane to takeoff : " << time_to_take_off << endl;
    cout << "Probability of plane arriving to Land : " << landing_probability << endl;
    cout << "Probability of plane arriving to Takeoff: " << takeoff_probability << endl;
    cout << "Minutes a plane can stay in Air without Crashing : " << fuel_limit << endl;
    cout << "Total simulation minutes : " << simulation_time << endl;

    for(current_minute = 1; current_minute <= simulation_time; ++current_minute)
    // Simulate the passage of one minute
    {        
        if(arrivalLanding.query())
        // Check whether a new plane has arrived to Land
        {
            arrival_timesLanding.Push(current_minute);
        }
        if(arrivalTakeoff.query())
        // Check whether a new plane has arrived to Takeoff
        {
            arrival_timesTakeoff.Push(current_minute);
        }        
        if((!RunWay.is_busy()) && ((!arrival_timesLanding.Empty()) || (!arrival_timesTakeoff.Empty())))
        // Check whether a plane can use the runway.
        {
            bool over = true;                          // Variable to know if takeoff can take place            
            if(!arrival_timesLanding.Empty())
            // Check to see if a plane is in queue to land
            {
                    nextLanding = arrival_timesLanding.Top();                    
                    if(fuel_limit < (current_minute - nextLanding))
                    // Check to see if plane was over limit till crashing
                    {                        
                        while(fuel_limit < (current_minute - nextLanding))
                        // Loop to get the next plane that has not crashed
                        {
                            wait_timesLanding.addtoCrash();
                            arrival_timesLanding.Pop();                            
                            if(!arrival_timesLanding.Empty())
                            // If there is a plane in landing check, it is loaded in
                            {
                                nextLanding = arrival_timesLanding.Top();
                            }
                            else if(arrival_timesLanding.Empty())
                            // If there is no planes to land, this exits while loop so it still
                            //  does a takeoff
                            {
                                nextLanding = current_minute - 1;
                            }
                        }
                        if(!arrival_timesLanding.Empty())
                        // Plane lands here.
                        {
                            arrival_timesLanding.Pop();
                            wait_timesLanding.next_number(current_minute - nextLanding);
                            RunWay.start_runwayLanding();
                            over = false;
                        }
                    }                    
                    else if(fuel_limit >= (current_minute - nextLanding))
                    // Plane lands here if the first plane in queue did not crash
                    {
                        arrival_timesLanding.Pop();
                        wait_timesLanding.next_number(current_minute - nextLanding);
                        RunWay.start_runwayLanding();
                        over = false;
                    }
            }            
            if(!arrival_timesTakeoff.Empty() && over == true)
            // Plane uses runway to takeoff if no plane needs to land
            {
                nextTakeoff = arrival_timesTakeoff.Top();
                arrival_timesTakeoff.Pop();
                wait_timesTakeoff.next_number(current_minute - nextTakeoff);
                RunWay.start_runwayTakeoff();
            }
        }        
        if(current_minute == simulation_time)
        // Count the planes left in the queue when simulatio is over
        {
            unsigned int sub = 0;       // var to count planes left in landing queue
            unsigned int sub2 = 0;      // var to count planes left in takeoff queue            
            while(!arrival_timesLanding.Empty())
            // Loop to count planes left in landing queue and those that crashed
            {
                sub = arrival_timesLanding.Top();
                if(fuel_limit < (current_minute - sub))
                {
                    wait_timesLanding.addtoCrash();
                }
                arrival_timesLanding.Pop();
                cntLandingplanesleft += 1;
            }            
            while(!arrival_timesTakeoff.Empty())
            // Loop to count planes left in takeoff queue
            {
                sub2 = arrival_timesTakeoff.Top();
                arrival_timesTakeoff.Pop();
                cntTakeoffplanesleft += 1;
            }
        }
        // Tell the machine to simulate the passage of one minute.
        RunWay.one_minute();
    }
    // Writes out the summary information about the simulation.
    cout << "Planes that landed: " << wait_timesLanding.how_many_numbers() << endl;
    cout << "Planes that Took off: " << wait_timesTakeoff.how_many_numbers() << endl;
    if(wait_timesLanding.how_many_numbers() > 0)
    {
        cout << "Average wait to Land: " << wait_timesLanding.average() << " min" << endl;
    }
    if(wait_timesTakeoff.how_many_numbers() > 0)
    {
        cout << "Average wait to Takeoff: " << wait_timesTakeoff.average() << " min" << endl;
    }
    cout << "planes in landing queue : " << cntLandingplanesleft << endl;
    cout << "planes in takeoff queue : " << cntTakeoffplanesleft << endl;
    cout << "The number of planes that crashed : " << wait_timesLanding.getCrashCount() << endl;
}


runway::runway(unsigned int l, unsigned int t)
{
    seconds_for_runwayLanding = l;
    seconds_for_runwayTakeoff = t;
    runway_time_left = 0;
}
void runway::one_minute()
{
    if(is_busy())
    {
        --runway_time_left;
    }
}
void runway::start_runwayLanding()
//Library facilities used : cassert
{
    assert(!is_busy());
    runway_time_left = seconds_for_runwayLanding;
}
void runway::start_runwayTakeoff()
//Library facilities used : cassert
{
    assert(!is_busy());
    runway_time_left = seconds_for_runwayTakeoff;
}

bool bool_source::query() const
//Library facilities used : cstdlib
{
    return (rand() < probability*RAND_MAX);
}
bool_source::bool_source(double pl)
//Library facilities used : cassert
{
    assert(pl >= 0);
    assert(pl <= 1);
    probability = pl;
}
averager::averager()
{
    count = 0;
    sum = 0;
    crash = 0;
}
void averager::addtoCrash()
{
    crash += 1;
}
void averager::next_number(double value)
{
    ++count;
    sum += value;
}
double averager::average() const
//Library facilities used : cassert
{
    assert(how_many_numbers() > 0);
    return sum/count;
}
