#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "manager.h"
using namespace std;
namespace fs = filesystem;
mutex g_mutex;

string getCurrentTime()
{
    auto now = chrono::system_clock::now();
    auto ms = chrono::time_point_cast<chrono::milliseconds>(now);

    auto epoch = ms.time_since_epoch();
    auto current_ms = chrono::duration_cast<chrono::milliseconds>(epoch).count();

    auto seconds = chrono::duration_cast<chrono::seconds>(epoch);
    auto current_seconds = seconds.count();

    auto milliseconds = current_ms - (current_seconds * 1000);

    time_t now_c = chrono::system_clock::to_time_t(now);
    tm now_tm = *localtime(&now_c);

    stringstream ss;
    ss << "[" << put_time(&now_tm, "%H:%M:%S") << ":" << setfill('0') << setw(3) << milliseconds << "]";

    return ss.str();
}

streampos getFileSize(const std::string &filePath)
{
    ifstream file(filePath, ios::binary | ios::ate);
    return file.tellg();
}

string folderGenerator(string filename, string grouping)
{
    string result;
    if (grouping == "Type")
    {
        size_t dotPos = filename.find_last_of('.');
        result = filename.substr(dotPos + 1);
    }
    else if (grouping == "Alphabets")
    {
        result = filename[0];
    }

    return result;
}

int Engine::generateFileList(string curDir, string filesList[])
{
    int i = 0;
    for (const auto &entry : fs::directory_iterator(curDir))
    {
        if (fs::is_regular_file(entry.path()))
        {
            filesList[i] = entry.path().filename();
            i++;
        }
    }
    return i;
}

int Engine::generateFolderList(string filesList[], string foldersList[], int size, string grouping)
{
    int j = 0, k;
    for (int i = 0; i < size; ++i)
    {
        string item = folderGenerator(filesList[i], grouping);
        for (k = 0; k < j; ++k)
            if (foldersList[k] == item)
                break;
        if (k == j)
        {
            foldersList[j] = item;
            j++;
        }
    }

    return j;
}

int Engine::createFolders(string curDir, string foldersList[], int size)
{
    string directoryPath;
    for (int i = 0; i < size; ++i)
    {
        directoryPath = curDir + "/" + foldersList[i];
        if (!fs::exists(directoryPath))
        {
            fs::create_directory(directoryPath);
        }
        else
        {
            // rollback
            for (int j = i - 1; j >= 0; --j)
                fs::remove_all(curDir + "/" + foldersList[j]);
            return 404;
        }
    }
    return 200;
}

int Engine::createProcessors(string curDir, string grouping, string filesList[], string foldersList[], int fileSize, int folderSize)
{
    ofstream outfile("logs.txt", ofstream::out | ofstream::trunc);
    outfile.close();
    for (int i = 0; i < folderSize; ++i)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            ofstream outfile("logs.txt", ofstream::out | ofstream::app);
            Processor processor;
            processor.settings(grouping, foldersList[i]);
            for (int j = 0; j < fileSize; ++j)
            {
                if (processor.matcher(filesList[j]))
                {
                    lock_guard<mutex> lock(g_mutex);
                    processor.relocator(curDir, filesList[j]);
                    outfile << getCurrentTime() << ": " << filesList[j] << " -> " << foldersList[i] << "\n";
                }
            }
            outfile.close();
            exit(0);
        }
        else if (pid < 0)
            return 400;
    }

    int status;
    pid_t childPid;
    while ((childPid = wait(&status)) != -1)
    {
        cout << "Child process " << childPid << " exited with status " << status << endl;
    }

    return 200;
}

int bootstrap(string curDir, string grouping, string priority)
{
    Engine e;
    string foldersList[1024], filesList[1024];

    int size = e.generateFileList(curDir, filesList);
    int folderListSize = e.generateFolderList(filesList, foldersList, size, grouping);

    if (priority == "Priority [SIZE]")
    {
        std::sort(filesList, filesList + size, [&](const string &a, const string &b)
                  { return getFileSize(curDir + "/" + a) < getFileSize(curDir + "/" + b); });
    }

    for (int i = 0; i < size; ++i)
        cout << filesList[i] << endl;

    int creatorStatus = e.createFolders(curDir, foldersList, folderListSize);
    if (creatorStatus != 200)
        return creatorStatus;

    int processStatus = e.createProcessors(curDir, grouping, filesList, foldersList, size, folderListSize);
    if (processStatus != 200)
        return processStatus;

    return 200;
}