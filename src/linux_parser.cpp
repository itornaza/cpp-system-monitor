#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string skip;
  string kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> skip >> skip >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Reads and returns the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string skip;
  string temp;
  string line;
  vector<string> memory;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 2; ++i) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> skip >> temp >> skip;
      memory.push_back(temp);
    }
  }
  float mem_total = std::stof(memory[0]);
  float mem_free = std::stof(memory[1]);
  return (mem_total - mem_free) / mem_total;
}

// Reads and returns the system uptime
long LinuxParser::UpTime() { 
  long uptime = 0.0;
  string temp = "0.0";
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp;
  }
  uptime = std::stol(temp);
  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Reads and returns CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> timers;
  string timer;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    linestream >> skip;
    for(int i = 0; i < 10; ++i) {
      linestream >> timer;
      timers.push_back(timer);
    }
  }
  return timers; 
}

// Parses the /proc/stat for a given token
int LinuxParser::SystemProcesses(string token) {
  int processes = 0;
  bool search = true;
  string line;
  string temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(search == true && stream.peek() != EOF) {
      std::getline(stream, line);
      std::istringstream linestream(line); 
      linestream >> temp;
      if(temp == token) {
        linestream >> temp;
        processes = std::stoi(temp);
        search = false;
      } // End inner if
    } // End while
  } // End outer if
  return processes; 
}

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses() { 
  return LinuxParser::SystemProcesses("processes");
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() { 
  return LinuxParser::SystemProcesses("procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }