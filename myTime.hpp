#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

using std::to_string;

std::string getTime(){
    // Get the current system time
    auto currentTime = std::chrono::system_clock::now();

    // Convert the system time to a time_t object
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    // Convert the time_t object to a local time struct tm
    std::tm* localTime = std::localtime(&currentTime_t);

    // Extract the individual components: hours, minutes, seconds
    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;

    // Extract the individual components: day, month, year
    int day = localTime->tm_mday;
    int month = localTime->tm_mon + 1;  // Month starts from 0, so adding 1
    int year = localTime->tm_year + 1900;  // Year is since 1900

    return  to_string(year) + ":" + to_string(month) + ":" + to_string(day) + ":" + to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
    
}
