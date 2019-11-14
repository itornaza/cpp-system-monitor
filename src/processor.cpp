#include <string>
#include <vector>

#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<string> timers = LinuxParser::CpuUtilization();
  int user  = std::stoi(timers[0]);
  int nice = std::stoi(timers[1]);
  int system = std::stoi(timers[2]);
  int idle = std::stoi(timers[3]);
  int iowait = std::stoi(timers[4]);
  int irq = std::stoi(timers[5]);
  int softirq = std::stoi(timers[6]);
  int steal = std::stoi(timers[7]);  
  int guest = std::stoi(timers[8]);
  int guest_nice = std::stoi(timers[9]);
  
  float t_total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
  float t_idle = idle + iowait;
  float percentage = (t_total - t_idle) / t_total;
  
  return percentage; 
}