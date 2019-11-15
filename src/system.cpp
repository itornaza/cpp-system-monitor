#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

// TODO: Remove after testing
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  Process temp;

  // TODO: Remove after testing
  // temp.Pid(pids[0]);
  // int pid = pids[0];
  // std::cout << "*****" << LinuxParser::Command(pid) << "*****" << std::endl;
  // std::cout << "*****" << LinuxParser::Ram(pid) << "*****" << std::endl;
  // std::cout << "*****" << LinuxParser::Uid(pid) << "*****" << std::endl;
  // std::cout << "*****" << LinuxParser::User(pid) << "*****" << std::endl;
  // std::cout << "*****" << LinuxParser::UpTime(pid) << "*****" << std::endl;
    
  // TODO: The number of processes (n in ncurses file) is not consistent 
  // to the number of our processes
  
  for (auto pid : pids) {
    // TODO: Remove after testing
    // std::cout << "***" << pid << "***" << std::endl;
    temp.Pid(pid);
    temp.User(LinuxParser::User(pid));
    temp.Command(LinuxParser::Command(pid));
    // TODO: temp.CpuUtilization(pid);
    temp.Ram(LinuxParser::Ram(pid));
    temp.UpTime(LinuxParser::UpTime(pid));
    processes_.push_back(temp);
  }
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