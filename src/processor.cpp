#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// Returns the aggregate CPU utilization
float Processor::Utilization() { 
  float percentage =(float)LinuxParser::ActiveJiffies()/LinuxParser::Jiffies();
  return percentage;
}
