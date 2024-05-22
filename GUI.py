import tkinter as tk
from tkinter import ttk
from functionality import submitAction, browseAction

class GuiApp:
    def __init__(self, master, title, geometry):
        self.master = master
        master.title(title)
        master.geometry(geometry)

        self.create_left_frame()
        self.create_right_frame()

    def create_left_frame(self):
        self.left_frame = ttk.LabelFrame(self.master, text="Action Control Panel")
        self.left_frame.place(relx=0.02, rely=0.02, relwidth=0.49, relheight=0.96)

        self.create_current_directory_section()
        self.create_settings_section()
        self.create_action_logs_section()

    def create_current_directory_section(self):
        self.currentDirLabel = ttk.Label(self.left_frame, text="Current Directory:")
        self.currentDirLabel.grid(row=0, column=0, padx=5, pady=5, sticky="w")

        self.currentDirEntry = ttk.Entry(self.left_frame, width=15)
        self.currentDirEntry.grid(row=0, column=1, padx=5, pady=5)

        self.browseButton = ttk.Button(self.left_frame, text="Browse", width=5, command=lambda: browseAction(self.currentDirEntry))
        self.browseButton.grid(row=0, column=2, padx=5, pady=5)

    def create_settings_section(self):
        self.settingsFrame = ttk.LabelFrame(self.left_frame, text="Settings")
        self.settingsFrame.grid(row=1, column=0, columnspan=3, padx=5, pady=5, sticky="ew")

        self.groupingLabel = ttk.Label(self.settingsFrame, text="Grouping Based On:")
        self.groupingLabel.grid(row=0, column=0, padx=5, pady=5, sticky="w")

        self.groupingDropdown = ttk.Combobox(self.settingsFrame, values=["Type", "Alphabets"], width=10)
        self.groupingDropdown.grid(row=0, column=1, padx=5, pady=5, sticky="w")

        self.priorityLabel = ttk.Label(self.settingsFrame, text="Priority Sorting:")
        self.priorityLabel.grid(row=1, column=0, padx=5, pady=5, sticky="w")

        self.priorityDropdown = ttk.Combobox(self.settingsFrame, values=["FCFS", "Priority [SIZE]"], width=10)
        self.priorityDropdown.grid(row=1, column=1, padx=5, pady=5, sticky="w")


        self.submitButton = ttk.Button(self.left_frame, text="Submit/Run", command=lambda: submitAction(self.actionLogsText, self.fileTransferLogsText, self.currentDirEntry, self.groupingDropdown, self.priorityDropdown))

        self.submitButton.grid(row=2, column=0, columnspan=3, padx=5, pady=5, sticky="ew")

    def create_action_logs_section(self):
        self.actionLogsLabel = ttk.Label(self.left_frame, text="Action Logs:")
        self.actionLogsLabel.grid(row=3, column=0, padx=5, pady=5, sticky="w")

        self.actionLogsText = tk.Text(self.left_frame, width=10, height=10)
        self.actionLogsText.grid(row=4, column=0, columnspan=3, padx=5, pady=5, sticky="ew")

    def create_right_frame(self):
        self.rightFrame = ttk.LabelFrame(self.master, text="File Management System", labelanchor="ne")
        self.rightFrame.place(relx=0.52, rely=0.02, relwidth=0.46, relheight=0.96)

        self.create_file_transfer_logs_section()

    def create_file_transfer_logs_section(self):
        self.fileTransferLogsLabel = ttk.Label(self.rightFrame, text="File Transfer Logs:")
        self.fileTransferLogsLabel.pack(padx=5, pady=5, anchor="w")

        self.fileTransferLogsText = tk.Text(self.rightFrame, width=90, height=30)
        self.fileTransferLogsText.pack(padx=5, pady=5)