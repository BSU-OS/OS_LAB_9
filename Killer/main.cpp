#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <regex>
#include <sstream>

std::istringstream input;

DWORD TerminateProcessByPID(DWORD PID) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
    bool isTerminated = TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);
    return isTerminated;//TODO return
}

DWORD TerminateProcessByName(std::string processName) {
    PROCESSENTRY32 pe32;
    DWORD numTerminatedProcess = 0;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, false);
    pe32.dwSize = sizeof(PROCESSENTRY32);
    do {
        std::string str(pe32.szExeFile);
        if (str == processName) {//TODO lower upper equal
            numTerminatedProcess += TerminateProcessByPID(pe32.th32ProcessID);
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return numTerminatedProcess;//TODO return
}

DWORD TerminateProcessByEnvV() {
    char cstr[1000];
    GetEnvironmentVariable("PROC_TO_KILL", cstr, sizeof(cstr));
    std::string str(cstr);
    std::regex regex{R"([,]+)"}; // split on space and comma
    std::sregex_token_iterator it{str.begin(), str.end(), regex, -1};
    std::vector<std::string> names{it, {}};
    DWORD numTerminatedProcess = 0;
    for (auto &str: names) {
        numTerminatedProcess += TerminateProcessByName(str);
    }
    return numTerminatedProcess;

}

DWORD KillerTerminateProcess(std::string flag) {


    DWORD numTerminatedProcess = 0;
    if (flag == "/PID") {
        DWORD PID;
        input >> PID;
        numTerminatedProcess += TerminateProcessByPID(PID);
    } else if (flag == "/NAME") {
        std::string NAME;
        input >> NAME;
        numTerminatedProcess += TerminateProcessByName(NAME);
    } else if (flag == "/ENVV") {
        numTerminatedProcess += TerminateProcessByEnvV();
    } else {
        std::cout << flag << " is not recognized.\n";
        return -1;
    }
    std::cout << "Number of terminated processes : " << numTerminatedProcess << "\n";
    return numTerminatedProcess;
}

void KillerRecursion() {
    std::string flag = "";
    while (flag != "/EXIT") {
        input >> flag;
        if (flag == "/EXIT") {
            return;
        } else if (flag == "/HELP") {
            std::cout << "Help\n"
                      << "/PID DWORD_PID          Set terminate process by PID flag.\n"
                      << "/NAME STRING_NAME       Set terminate process by NAME flag\n"
                      << "/ENVV                   Set terminate process by Environment Variable PROC_TO_KILL\n"
                      << "/EXIT                   Exit program\n";
        } else {
            KillerTerminateProcess(flag);
        }
    }
}

std::string inputToString(int argc, char **argv) {
    std::string str = "";
    for (size_t i = 1; i < argc; ++i) {
        str += argv[i];
        str += ' ';
    }
    return str;
}

int main(int argc, char **argv) {
    input = std::istringstream(inputToString(argc, argv));
    KillerRecursion();
    system("pause");
}
