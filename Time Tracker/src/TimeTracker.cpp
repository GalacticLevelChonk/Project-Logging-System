#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <format>
struct TimeTracker{
    std::chrono::system_clock::time_point systemStartTime;
    std::chrono::system_clock::time_point systemEndTime;

    std::chrono::steady_clock::time_point steadyStart;
    std::chrono::steady_clock::time_point steadyEnd;

    std::string systemStartTimeString;
    std::string systemEndTimeString;

    std::string systemStartDate;
    std::string systemEndDate;

    std::chrono::seconds timeElapsed;

void StartClock(){
    steadyStart = std::chrono::steady_clock::now();
    systemStartTime = std::chrono::system_clock::now();

    std::cout << "Work session started!\n";
}

std::string ConvertTimeToString(std::chrono::system_clock::time_point clock, bool getDate = false){
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(clock);

    std::tm tm = *std::localtime(&now_time_t);

    std::string formattedTime;

    std::ostringstream oss;
    if(getDate){
        oss << std::put_time(&tm, "%Y-%m-%d");
        formattedTime = oss.str();
        return formattedTime;
    } else{
        oss << std::put_time(&tm, "%H:%M:%S");
        formattedTime = oss.str();
        return formattedTime;
    }
}



void EndClock(){
    using namespace std::chrono;
    steadyEnd = steady_clock::now();
    systemEndTime = system_clock::now();

    timeElapsed = duration_cast<std::chrono::seconds>(steadyEnd - steadyStart);

    totalSecondsSpent += timeElapsed.count();

    systemStartTimeString = ConvertTimeToString(systemStartTime, false);
    std::cout << "Conversion: " << ConvertTimeToString(systemStartTime, false) << "\n";
    systemEndTimeString = ConvertTimeToString(systemEndTime, false);

    systemStartDate = ConvertTimeToString(systemStartTime, true);
    systemEndDate = ConvertTimeToString(systemEndTime, true);
}

void SaveToFile(){
    auto hours = std::chrono::duration_cast<std::chrono::hours>(timeElapsed);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(timeElapsed) % 60;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) % 60;


    std::ofstream outFile("DataLog.txt", std::ios::app);
    outFile << systemStartDate << " - " << systemEndDate << " | ";

    outFile << systemStartTimeString << " - " << systemEndTimeString << "\n    ";

    outFile << "Session duration: " << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s\n";
}
void SimulateTimePassage(int hoursToSimulate) {
    timeElapsed += std::chrono::hours(hoursToSimulate);  // Simulate time passage by adding hours
}

private:    
    long long int totalSecondsSpent ;
};


int main(int argc, char *argv[]){
    using namespace std::chrono;
    TimeTracker Time;

    std::string sussy;

    Time.StartClock();

    getline(std::cin, sussy);
    if(sussy == "end"){
    Time.EndClock();
    }

    Time.SaveToFile();

}