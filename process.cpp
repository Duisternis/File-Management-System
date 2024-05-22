#include <iostream>
#include <string>
#include <filesystem>
#include "manager.h"
using namespace std;
namespace fs = filesystem;

void Processor::settings(string type, string name)
{
    this->type = type;
    this->name = name;
}

bool Processor::matcher(string filename)
{
    if (type == "Type")
    {
        size_t dotPos = filename.find_last_of('.');
        string item = filename.substr(dotPos + 1);
        return (item == name);
    }
    else if (type == "Alphabets")
    {
        return (filename[0] == name[0]);
    }

    return -1;
}

void Processor::relocator(string curDir, string filename)
{
    string currentPath = curDir + "/" + filename;
    string newPath = curDir + "/" + name + "/" + filename;
    fs::rename(currentPath, newPath);
}