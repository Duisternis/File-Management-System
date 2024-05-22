#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
using namespace std;

class Engine
{
public:
    int generateFileList(string, string[]);
    int generateFolderList(string[], string[], int, string);
    int createFolders(string, string[], int);
    int createProcessors(string, string, string[], string[], int, int);
};

class Processor
{
private:
    string type, name;

public:
    void settings(string, string);
    bool matcher(string);
    void relocator(string, string);
};

#endif