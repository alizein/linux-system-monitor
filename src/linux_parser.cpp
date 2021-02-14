#include <dirent.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;

  float MemTotal, MemFree;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = stoi(value);
        } else if (key == "MemFree:") {
          MemFree = stoi(value);
        }
      }
    }
  }

  return (MemTotal - MemFree) / MemTotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return stoi(uptime);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, jiffy;
  int jiffy_index{1};

  long utime, stime, cutime, cstime;

  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> jiffy) {
      switch (jiffy_index)
      {
        case 14:
          utime = stoi(jiffy);
          break;
        case 15:
          stime = stoi(jiffy);
          break;
        case 16:
          cutime = stoi(jiffy);
          break;
        case 17:
          cstime = stoi(jiffy);
          break;
          
        default:
          break;
      }
      
      jiffy_index++;
    }
  }

  return utime + stime + cutime + cstime;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  long activealltime = stoi(jiffies[kUser_]) + stoi(jiffies[kNice_]) +
                       stoi(jiffies[kSystem_]) + stoi(jiffies[kIRQ_]) +
                       stoi(jiffies[kSoftIRQ_]) + stoi(jiffies[kSteal_]);
  return activealltime;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  long idlealltime = stoi(jiffies[kIdle_]) + stoi(jiffies[kIOwait_]);
  return idlealltime;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_jiffies;
  string line, jiffy;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    // pop cpu out of linestream
    linestream >> jiffy;

    while (linestream >> jiffy) {
      cpu_jiffies.push_back(jiffy);
    }
  }

  return cpu_jiffies;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/" +
                           kCmdlineFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }

  return string();
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/" +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return value;
        }
      }
    }
  }
  return string();
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/" +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string user, x;
  string value;
  string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> value) {
        if (value == uid) {
          return user;
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, jiffy;
  int jiffy_index{1};

  long starttime;

  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> jiffy) {
      switch (jiffy_index)
      {
        case 22:
          starttime = stoi(jiffy);
          break;
          
        default:
          break;
      }
      
      jiffy_index++;
    }
  }

  return starttime;
}