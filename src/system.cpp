#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/**
 * Return the system's CPU. 
 * Reserved for future additional features
 */
Processor& System::Cpu() { 
  return cpu_; 
}

// Returns a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for(int pid: pids) {
    Process p;
    p.Pid(pid);
    p.User(pid);
    p.Command(pid);
    p.CpuUtilization(pid);
    p.Ram(pid);
    p.UpTime(pid);
    processes_.push_back(p);
  }
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_; 
}

// Returns the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel();
}

// Returns the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Returns the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// Returns the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

// Returns the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

// Returns the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}