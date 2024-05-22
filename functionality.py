import tkinter as tk
from tkinter import filedialog
import datetime

from build.sortingEngine import startupRequest

def getCurrentTime():
    currentDateTime = datetime.datetime.now()
    currentTimeString = currentDateTime.strftime("[%H:%M:%S:%f]")
    return currentTimeString

def submitAction(actionLogsText, fileTransferLogsText, currentDirEntry, groupingDropdown, priorityDropdown):
    currentDirectory = currentDirEntry.get()
    grouping = groupingDropdown.get()
    priority = priorityDropdown.get()

    if not(currentDirectory and grouping and priority):
        actionLogsText.insert(tk.END, "[ERROR]: Parameters missing!\n\n")
        actionLogsText.see(tk.END)
        return
    

    actionLogsText.insert(tk.END, f"{getCurrentTime()}: Requesting sortingEngine...\n")
    actionLogsText.see(tk.END)
    
    """
    Requesting engine to startup
    200: OK - successful
    400: Forking Failed
    404: Folder creation failed
    """
    returnStatus = startupRequest(currentDirectory, grouping, priority)
    
    actionLogsText.insert(tk.END, f"{getCurrentTime()}: Request Completed! [{returnStatus}]\n")
    actionLogsText.see(tk.END)

    actionLogsText.insert(tk.END, f"{getCurrentTime()}: Fetching FileTransferLogs...\n")
    actionLogsText.see(tk.END)

    with open("logs.txt", "r") as file:
        content = file.read()
        fileTransferLogsText.delete("1.0", tk.END)
        fileTransferLogsText.insert(tk.END, content)

    actionLogsText.insert(tk.END, f"{getCurrentTime()}: Terminated Successfully!\n\n")
    actionLogsText.see(tk.END)

def browseAction(currentDirEntry):
    directory = filedialog.askdirectory()
    currentDirEntry.delete(0, tk.END)
    currentDirEntry.insert(0, directory)