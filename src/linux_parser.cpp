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

//-----------------------------------------------------------------------------
// Utils
//-----------------------------------------------------------------------------

// Parses the file to find a value for a given key
string LinuxParser::KeyValParser(string key, string path) {
  string value = "n/a";
  bool search = true;
  string line;
  string temp;
  std::ifstream stream(path);
  if(stream.is_open()) {
    while(search == true && stream.peek() != EOF) {
      std::getline(stream, line);
      std::istringstream linestream(line); 
      linestream >> temp;
      if(temp == key) {
        linestream >> temp;
        value = temp;
        search = false;
      } // End inner if
    } // End while
  } // End outer if
  return value; 
}

//-----------------------------------------------------------------------------
// System
//-----------------------------------------------------------------------------

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value = "n/a";
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
  string kernel = "n/a";
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

// Reads and returns the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string skip;
  string temp;
  string line;
  float mem = 0.0;
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
  mem = (mem_total - mem_free) / mem_total;
  return mem;
}

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses() { 
  int t_processes = 0;
  string path = kProcDirectory + kStatFilename;
  string result = LinuxParser::KeyValParser("processes", path);
  t_processes = std::stoi(result);
  return t_processes;
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() { 
  int a_processes = 0;
  string path = kProcDirectory + kStatFilename;
  string result = LinuxParser::KeyValParser("procs_running", path);
  a_processes = std::stoi(result);
  return a_processes;
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
  uptime = std::atoi(temp.c_str());
  return uptime; 
}

//-----------------------------------------------------------------------------
// Processor
//-----------------------------------------------------------------------------

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> jiffies = CpuUtilization();
  long t_jiffies = 0;
  for(string jiffie : jiffies) {
    t_jiffies += std::stoi(jiffie);
  }
  return t_jiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long a_jiffies = 0;
  a_jiffies = Jiffies() - IdleJiffies();
  return a_jiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  long i_jiffies = 0;
  long idle = std::stoi(jiffies[3]);
  long iowait = std::stoi(jiffies[4]);
  i_jiffies = idle + iowait;
  return i_jiffies;
}

// Reads and returns the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long a_jiffies = 0;
  string utime;
  string stime;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 1; i < 14; ++i) {
      linestream >> skip;
    }
    linestream >> utime >> stime;
  }
  a_jiffies = std::atol(utime.c_str()) + std::atol(stime.c_str());
  return a_jiffies;
}

//-----------------------------------------------------------------------------
// Process
//-----------------------------------------------------------------------------

// Reads and returns the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
  return LinuxParser::KeyValParser("Uid:", path);
}

// Reads and returns the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string test_user;
  string test_uid;
  string skip;
  string user = "n/a";
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  bool search = true;
  if (stream.is_open()) {
    while(search && stream.peek() != EOF ) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line); 
      linestream >> test_user >> skip >> test_uid;
      if(uid == test_uid) {
        user = test_user;
        search = false;
      }
    }
  }
  return user; 
}

// Reads and returns the memory used by a process
string LinuxParser::Ram(int pid) { 
  string path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
  return LinuxParser::KeyValParser("VmSize:", path);
}

// Reads and returns the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long ticks = 0;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 1; i < 22; ++i) {
      linestream >> skip;
    }
    linestream >> ticks;
  }
  return ticks / sysconf(_SC_CLK_TCK);
}

// Reads and returns the command associated with a process
string LinuxParser::Command(int pid) { 
  string line = "n/a";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}
