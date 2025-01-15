#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <format>
#include <utility>
#include <type_traits>

struct TimeTracker{
   
    bool hasInfo = false;

    int taskAmount = 0;

    std::chrono::seconds timeElapsed;

    std::string intention = "None added";
    std::vector<std::string> tasks;

 std::vector<std::string> search(std::string searchQuery){
    std::vector<std::string> lines;

    std::ifstream inFile("DataLog.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error opening file..." << std::endl;
        return lines;
    }
    std::string currentWord;
    std::string line;

    std::string compoundLine;

    bool isDateAppended = false;

        while(std::getline(inFile, line)){
            if(line.find(searchQuery) != std::string::npos){
                std::cout << "Repeated";
                compoundLine.append(line);

                for(int i = 0; i < 7; ++i) {
                    std::getline(inFile, line);
                    compoundLine.append("\n");
                    compoundLine.append(line);
                }
        while(getline(inFile, line)){
            if(line != ""){
            compoundLine.append("\n");
            compoundLine.append(line);
    } else{
        break;
    }
}
lines.push_back(compoundLine);
}
}

inFile.close();
return lines;
    }  

const size_t systemStartTimeDataLocation = 0;
const size_t pauseStartTimeDataLocation = 50;
const size_t systemStartDateDataLocation = 100;
const size_t pauseStartDateDataLocation = 150;
const size_t isPausedDataLocation = 200;
const size_t startToPauseDurationLocation = 250;



void StartPause(){
    systemPauseStart = std::chrono::system_clock::now();

    std::ofstream outFile("PauseData.txt", std::ios::trunc);
    if(!outFile.is_open()){
        std::cerr << "Error opening file...";
        return;
    }

    auto startToPauseDurationCast = std::chrono::duration_cast<std::chrono::seconds>(systemPauseStart - systemStartTime);
    int startToPauseDuration = static_cast<int>(startToPauseDurationCast.count());

    outFile << "                                                                                                                                                                                                                                                         ";

    outFile.seekp(systemStartTimeDataLocation);
    outFile << ConvertTimeToString(systemStartTime, false);

    outFile.seekp(pauseStartTimeDataLocation);
    outFile << ConvertTimeToString(systemPauseStart, false);

    outFile.seekp(systemStartDateDataLocation);
    outFile << ConvertTimeToString(systemStartTime, true);

    outFile.seekp(pauseStartDateDataLocation);
    outFile << ConvertTimeToString(systemPauseStart, true);

    outFile.seekp(isPausedDataLocation);
    outFile << "1";

    outFile.seekp(startToPauseDurationLocation);
    outFile << startToPauseDuration;

}


//[0]: System start time [1]: pause start time [2]: system start date [3]: pause start date
std::array<std::string, 5>  startInfo;
void EndPause(){
    wasPaused = true;
    systemPauseEnd = std::chrono::system_clock::now();

    std::ifstream inFile("PauseData.txt");
    if(!inFile.is_open()){
        std::cerr << "Error opening file...";
        return;
    }

    inFile.seekg(systemStartTimeDataLocation);
    inFile >> startInfo[0];

    inFile.seekg(pauseStartTimeDataLocation);
    inFile >> startInfo[1];

    inFile.seekg(systemStartDateDataLocation);
    inFile >> startInfo[2];

    inFile.seekg(pauseStartDateDataLocation);
    inFile >> startInfo[3];

    inFile.seekg(startToPauseDurationLocation);
    inFile >> startInfo[4];

inFile.close();

std::ofstream outFile("PauseData.txt", std::ios::trunc);

    if(!outFile.is_open()){
        std::cerr << "Error opening file\n";
        return;
    }
    outFile.seekp(isPausedDataLocation);
    outFile << "0";

outFile.close();
}



void StartClock(){    
    systemStartTime = std::chrono::system_clock::now();

    std::cout << "\nWork session started at " << ConvertTimeToString(systemStartTime, false) << "\n";
}



void EndClock(){
    systemEndTime = std::chrono::system_clock::now();

    if(wasPaused){
        std::chrono::seconds startToPauseDurationSeconds(std::stoi(startInfo[4]));
        timeElapsed = std::chrono::duration_cast<std::chrono::seconds>((systemEndTime - systemPauseEnd) + startToPauseDurationSeconds);
    } else{
        timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(systemEndTime - systemStartTime);
    }
    std::cout << "Work session ended\n\n";

    std::cout << "\n\nWasPAUSED: " << wasPaused << "\n\n";
    wasPaused ? savePauseData(true) : SaveToFile(true);
    SaveInfo(intention, tasks);
    std::cout << std::endl;
    exit(0);
}



void AddInfo(){

    std::cout << "Input your intentions: ";
    getline(std::cin, intention);

    std::cout << "Would you like to add tasks?(type yes if you want to):  ";
    std::string addTasks = "NULL";
    getline(std::cin, addTasks);

    if(addTasks == "yes" || addTasks == "Yes"){
        std::string task;
        int delAmount = 0;
       
        while(true){
            std::cout << "\nTask: ";
            getline(std::cin, task);
            if(task == "end" || task == "End"){
                break;
            }

            if(task == "delete" || task == "Delete"){
                int index = 0;
               
                std::cout << "Delete at what index(starting at 1 and to clear ALL type -314)";
                std::cin >> index;

                if(index == -314){
                    tasks.clear();
                } else{
                    tasks.erase(tasks.begin() + (index-1));              
                }
                delAmount++;
                std::cout << "Delete: " << delAmount << "\n\n";
                continue;
            }
            tasks.push_back(task);
            taskAmount = tasks.size();
        }

    }
hasInfo = true;
}


private:   
void SaveInfo(std::string intention, std::vector<std::string> tasks){
    std::ofstream outFile("DataLog.txt", std::ios::app);
    if(!outFile.is_open()){
        std::cout << "ERROR OPENING FILE...";
        return;
    }
    outFile << "\n     Session Information: ";
    outFile <<  "\n         Intention: " << intention;
    outFile <<  "\n              Completed Tasks: " << tasks.size();
    outFile <<  "\n                  Tasks: ";
    for(int i = 0; i < tasks.size(); i++){
        outFile << "\n                    -" << tasks[i];
    }
}



long long int totalSecondsSpent;
    
std::string ConvertTimeToString(std::chrono::system_clock::time_point clock, bool getDate = false){
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(clock);

    std::tm tm = *std::localtime(&now_time_t);

    std::ostringstream oss;
    if(getDate){
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    } else{
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }
}



void SaveToFile(bool displayOutput = false){
    auto hours = std::chrono::duration_cast<std::chrono::hours>(timeElapsed);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(timeElapsed) % 60;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) % 60;

    std::ofstream outFile("DataLog.txt", std::ios::app);
    outFile << "\n\n" << ConvertTimeToString(systemStartTime, true) << " - " << ConvertTimeToString(systemStartTime, true) << " | "; //Puts in date
    outFile << ConvertTimeToString(systemStartTime, false) << " - " << ConvertTimeToString(systemEndTime, false) << "\n    "; //Puts in time
    outFile << "Session duration: " << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s\n"; //Puts in the amount of time spent in session

    if(displayOutput){
    std::cout << ConvertTimeToString(systemStartTime, true) << " - " << ConvertTimeToString(systemStartTime, true) << " | ";
    std::cout << ConvertTimeToString(systemStartTime, false) << " - " << ConvertTimeToString(systemEndTime, false) << "\n    ";
    std::cout << "Session duration: " << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s\n";
    }
}



void savePauseData(bool displayOutput = false){
    auto hours = std::chrono::duration_cast<std::chrono::hours>(timeElapsed);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(timeElapsed) % 60;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) % 60;


    std::ofstream outFile("DataLog.txt", std::ios::app);
    outFile << "\n\n" << startInfo[2] << " - " << startInfo[3] << " : " << ConvertTimeToString(systemPauseEnd, true) << " - " << ConvertTimeToString(systemEndTime, true) << " | "; //Puts in date
    outFile << startInfo[0] << " - " <<  startInfo[1] << " : "<< ConvertTimeToString(systemPauseEnd, false) << " - " <<  ConvertTimeToString(systemEndTime, false) << "\n    "; //Puts in time
    outFile << "Session duration: " << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s\n"; //Puts in the amount of time spent in session

    if(displayOutput){
        std::cout << startInfo[2] << " - " << startInfo[3] << " : " << ConvertTimeToString(systemPauseEnd, true) << " - " << ConvertTimeToString(systemEndTime, true) << " | "; //Puts in date
        std::cout << startInfo[0] << " - " <<  startInfo[1] << " : "<< ConvertTimeToString(systemPauseEnd, false) << " - " <<  ConvertTimeToString(systemEndTime, false) << "\n    "; //Puts in time
        std::cout << "Session duration: " << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s\n"; //Puts in the amount of time spent in session
    }
}
    
    std::chrono::system_clock::time_point systemStartTime;
    std::chrono::system_clock::time_point systemEndTime;

    std::chrono::system_clock::time_point systemPauseStart;
    std::chrono::system_clock::time_point systemPauseEnd;

    bool isPaused = false;
    bool wasPaused = false;

};

struct Command{
    TimeTracker Time;

    std::string GetInput(std::string prompt){
        std::cout << prompt;
       
        std::string input;

        getline(std::cin, input);

        return input;
    }



void DisplayHome(){
    std::cout << "\033cLists of commands:\n\nStart: Starts a new session\n";
    std::cout << "End: Ends current session\n";
    std::cout << "Find/Search: Finds and lists all logs containing a given date/time\n";
    std::cout << "Pause: Pauses the current session for a later time, stores data even after program shut off\n";
    std::cout << "Continue: Continues a previously pauses session";
}



void GetCommand(){
    std::string command = GetInput("\n\nEnter a command: ");

    if(command == "Start" || command == "start"){
        StartTracker();
    }
    else if(command == "End" || command == "end"){
        EndTracker();
    }
    else if(command == "Find" || command == "find" || command == "Search" || command == "search"){
        Find();
    }
    else if(command == "Pause" || command == "pause"){
        Pause();
    }
    else if(command == "Continue" || command == "continue"){
        Continue();
    }

}



void StartTracker(){
    std::ifstream inFile("PauseData.txt");

    if(!inFile.is_open()){
        std::cout << "Error opening file...";
        return;
    }

    inFile.seekg(Time.isPausedDataLocation);
    std::string isPaused;
    inFile >> isPaused;

    if(isPaused == "1"){
        std::string input = GetInput("\n\nYou have an infinished/paused session would you like to continue that? ");
        if(input == "No" || input == "no"){
            DisplayHome();
            std::cout << "New session continue, pause deleted\n";
            Time.EndPause();
            Time.StartClock();
        } else{
            DisplayHome();
            Time.EndPause();
            }
        return; 
        }
    DisplayHome();
    Time.StartClock();
    }



void EndTracker(){
    std::ifstream inFile("PauseData.txt");

    if(!inFile.is_open()){
        std::cout << "Error opening file...";
        return;
    }

    inFile.seekg(Time.isPausedDataLocation);
    std::string isPaused;
    inFile >> isPaused;
    if(isPaused == "1"){
        std::string input = GetInput("\n\nYou have an infinished/paused session would you like to continue that? ");
        if(input == "No" || input == "no"){
            DisplayHome();
            std::cout << "New session continue, pause deleted";
            Time.EndPause();
            Time.EndClock();
        } else{
            DisplayHome();
            Time.EndPause();
            }
        return; 
        }

    if(Time.hasInfo == false){
        std::string command = GetInput("\nPlease add info: ");
            
        if(command == "no" || command == "No"){
            
            command = GetInput("\nAre you sure?");
            
            if(command == "Yes" || command == "yes"){
            
                std::cout << "\nOkay no info added\n";
                Time.EndClock();
            
            } else{
                Time.AddInfo();
            }
            
        } else{
            Time.AddInfo();
        }
    }
Time.EndClock();
}



void Find(){
    std::string input = GetInput("\nPlease input the date/time you want to search for: ");
    std::vector<std::string> results = Time.search(input);
    
    for(int i = 0; i < results.size(); i++){
        std::cout << "\n\n" << results[i] << "\n\n";
    }
    std::cout << "\n\n\n";
}

void Add(){
    DisplayHome();
    Time.AddInfo();
}



void Pause(){
    DisplayHome();
    Time.StartPause();
    std::cout << "\nSession paused for later";
}



void Continue(){
    DisplayHome();
    Time.EndPause();
    std::cout << "\nSession continued";
}
};

int main(){
    TimeTracker Time;
    Command command;
   
    command.DisplayHome();

    while(true){
    command.GetCommand();  
}
}