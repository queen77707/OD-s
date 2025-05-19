

# Operating System Simulator

## Overview

The Operating System Simulator is a comprehensive simulator designed to demonstrate various OS functionalities including process management, resource allocation, and CPU scheduling algorithms. This document provides an overview of the project, its features, setup instructions, and usage guidelines.

## Features

- **Process Management**: Create, monitor, and manage processes.
- **Resource Allocation**: Track and manage system resources such as RAM, HDD, and CPU cores.
- **CPU Scheduling Algorithms**: Implement and switch between different scheduling algorithms including:
  - First-Come-First-Serve (FCFS)
  - Round Robin
  - Priority Scheduling
- **User and Kernel Modes**: Switch between user mode for application execution and kernel mode for system management.
- **Applications**: Simulate various applications like Notepad, Calculator, Minesweeper, etc.
- **System Monitoring**: Monitor system resources and running processes.

## Setup Instructions

### Prerequisites

- **Operating System**: Linux (recommended for full functionality)
- **Compiler**: GCC (GNU Compiler Collection)
- **Libraries**: Standard C libraries

### Compilation

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Compile the source code using GCC:

   ```sh
   gcc -o os_simulator os_simulator.c -lpthread
   ```

### Execution

Run the compiled executable:

```sh
./os_simulator
```

## Usage

### Main Menu

Upon starting the simulator, you will be presented with a main menu offering various options:

- **User Mode**:
  - Notepad
  - Calculator
  - Time
  - Calendar
  - Create File
  - Move File
  - Copy File
  - Delete File
  - Minesweeper
  - Music Player
  - Snake Game
  - Help System
  - Show Tasks
  - End Task Now
  - Switch Mode
  - Shutdown

- **Kernel Mode**:
  - Memory Viewer
  - File Info
  - Process Manager
  - System Monitor
  - Close Task
  - Minimize Task
  - Restore Task
  - Show Scheduling Info
  - Switch to User Mode

### Switching Modes

- Use the `Switch Mode` option to toggle between User Mode and Kernel Mode.

### CPU Scheduling

- Use the `Set CPU Scheduling` option to change the CPU scheduling algorithm.

### System Monitoring

- Use the `System Monitor` option to view system resource usage.

## Code Structure

- **Main Functions**:
  - `boot_os()`: Initializes the operating system.
  - `show_main_menu()`: Displays the main menu.
  - `execute_task()`: Executes a selected task.
  - `create_process()`: Creates a new process.
  - `schedule_tasks()`: Schedules tasks based on the selected algorithm.
  - `set_scheduling_algorithm()`: Sets the CPU scheduling algorithm.
  - `show_scheduling_info()`: Displays scheduling information.

- **Utility Functions**:
  - `clear_screen()`: Clears the console screen.
  - `print_header()`: Prints the header with system information.
  - `print_error()`, `print_success()`, `print_warning()`, `print_info()`: Print formatted messages.
  - `loading_animation()`: Displays a loading animation.
  - `beep_sound()`: Plays a beep sound.

].

