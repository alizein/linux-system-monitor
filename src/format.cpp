#include <string>
#include <iomanip>

#include "format.h"

using std::string;

#define HOUR 3600
#define MINUTE 60

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hour, minute, second;
  string time_str;

  hour = seconds / HOUR;
  minute = (seconds % HOUR) / MINUTE;
  second = (seconds % HOUR) % MINUTE;

  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hour << ":" 
     << std::setw(2) << std::setfill('0') << minute << ":"
     << std::setw(2) << std::setfill('0') << second;
     
  return stream.str();
}