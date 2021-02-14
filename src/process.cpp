#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Process Constructor
Process::Process(int pid) : pid_(pid) { 
  user_   = LinuxParser::User(pid);
  pcpu_   = (float) LinuxParser::ActiveJiffies(pid) / (float) (LinuxParser::Jiffies() - LinuxParser::UpTime(pid));
  uptime_ = LinuxParser::UpTime(pid) / sysconf(_SC_CLK_TCK);
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return pcpu_; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
  return a.pcpu_ < this->pcpu_ ? true:false;
}