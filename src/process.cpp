#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  return 0.0;
}

// TODO: Return the command that generated this process
string Process::Command() { 
  return string(); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  return string(); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return string(); 
}

// TODO: Return the age of this process (in seconds)
long Process::UpTime() { 
  return up_time_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
  return true; 
}

//--------------
// Mutators
//--------------

void Process::Pid(int pid) {
  pid_ = pid;
}

void Process::User(string user) {
  user_ = user;
}

void Process::Command(string command) {
  command_ = command;
}

void Process::CpuUtilization(float cpu_utilization) {
  cpu_utilization_ = cpu_utilization;
}

void Process::Ram(string ram) {
  ram_ = ram;
}

void Process::UpTime(long up_time){
  up_time_ = up_time;
}
