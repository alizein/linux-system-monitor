#include <string>

#include "format.h"

using std::string;

#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hour, minute, second;
  string time_str;

  hour = seconds / SECONDS_PER_HOUR;
  minute = (seconds % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
  second = (seconds % SECONDS_PER_HOUR) % SECONDS_PER_MINUTE;

  time_str = std::to_string(hour) + ":" + std::to_string(minute) + ":" +
             std::to_string(second);

  return time_str;
}