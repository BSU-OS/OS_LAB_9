#include <windows.h>
#include <vector>

int main() {
    SetEnvironmentVariable("PROC_TO_KILL", "Telegram.exe,browser.exe,Discord.exe,msedge.exe");
    STARTUPINFO startupinfo = {sizeof startupinfo};
    PROCESS_INFORMATION processInformation = {0};
    CreateProcess(
            "..\\..\\Killer\\cmake-build-debug\\Killer.exe",
            "/HELP /NAME Telegram.exe /PID 21868 /ENVV /EXIT", NULL, NULL, FALSE, NULL, NULL, NULL, &startupinfo,
            &processInformation
    );//TODO only from cmd, not from Cline
    char str[200];
    GetCurrentDirectory(sizeof str, str);
    putenv("PROC_TO_KILL");//TODO is it right way to remove environment variable?

}
