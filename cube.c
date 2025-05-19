#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>
#include <termios.h>
#include <sys/select.h>
#include <errno.h>

// Define constants for maximum tasks, name length, path length, and time quantum for Round Robin scheduling
#define MAX_TASKS 50
#define MAX_NAME_LENGTH 50
#define MAX_PATH_LENGTH 256
#define TIME_QUANTUM 2  // For Round Robin scheduling

// Define color codes for console output
#define COLOR_RESET   "\033[0m"
#define COLOR_AQUA    "\033[36m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_PURPLE  "\033[35m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"

// Define an enum for scheduling algorithms
typedef enum {
    FCFS,           // First-Come-First-Serve
    ROUND_ROBIN,    // Round Robin
    PRIORITY        // Priority Scheduling
} SchedulingAlgorithm;

// Define a struct for tasks
typedef struct {
    pid_t pid;                     // Process ID
    char name[MAX_NAME_LENGTH];    // Name of the task
    int ram_usage;                 // RAM usage in MB
    int hdd_usage;                 // Hard Drive usage in MB
    int cpu_usage;                 // CPU usage in cores
    int is_running;                // Flag to check if task is running
    int is_minimized;              // Flag to check if task is minimized
    time_t start_time;             // Start time of the task
    int priority;                  // Priority for scheduling
    int remaining_time;           // Remaining time for Round Robin scheduling
} Task;

// Define a struct for system resources
typedef struct {
    int total_ram;         // Total RAM in MB
    int total_hdd;         // Total Hard Drive space in MB
    int total_cores;       // Total CPU cores
    int available_ram;     // Available RAM in MB
    int available_hdd;     // Available Hard Drive space in MB
    int available_cores;   // Available CPU cores
} SystemResources;

// Declare global variables
Task tasks[MAX_TASKS];            // Array to store tasks
SystemResources system_res;       // System resources
int task_count = 0;               // Number of tasks currently running
int current_mode = 0;             // Current mode (0 for User, 1 for Kernel)
sem_t resource_sem;               // Semaphore for resource management
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for task queue
SchedulingAlgorithm current_scheduler = FCFS;  // Current scheduling algorithm

// Function prototypes
void boot_os();
void show_main_menu();
void execute_task(char* task_name);
void create_process(char* task_name, int ram, int hdd, int cpu);
void show_running_tasks();
void close_task(int task_index);
void minimize_task(int task_index);
void restore_task(int task_index);
void switch_mode();
void shutdown_os();
void manage_resources(int ram, int hdd, int cpu, int allocate);
int check_resources(int ram, int hdd, int cpu);
void schedule_tasks();
void set_scheduling_algorithm();
void show_scheduling_info();

void notepad();
void calculator();
void show_time();
void create_file();
void move_file();
void copy_file();
void delete_file();
void file_info();
void minesweeper();
void calendar();
void music_player();
void system_monitor();
void process_manager();
void memory_viewer();
void help_system();
void snake_game();
void end_task_immediately();

void clear_screen();
void print_header();
void print_error(char* message);
void print_success(char* message);
void print_warning(char* message);
void print_info(char* message);
void loading_animation(char* message, int seconds);
void beep_sound(int duration_ms, int frequency);
int kbhit();

int main() {
    // Initialize semaphore
    if (sem_init(&resource_sem, 0, 1) != 0) {
        perror("Failed to initialize semaphore");
        exit(EXIT_FAILURE);
    }

    // Get system resources from user
    printf("Enter total RAM (MB): ");
    if (scanf("%d", &system_res.total_ram) != 1) {
        print_error("Invalid input for RAM!");
        return 1;
    }

    printf("Enter total Hard Drive space (MB): ");
    if (scanf("%d", &system_res.total_hdd) != 1) {
        print_error("Invalid input for HDD!");
        return 1;
    }

    printf("Enter number of CPU cores: ");
    if (scanf("%d", &system_res.total_cores) != 1) {
        print_error("Invalid input for CPU cores!");
        return 1;
    }

    system_res.available_ram = system_res.total_ram;
    system_res.available_hdd = system_res.total_hdd;
    system_res.available_cores = system_res.total_cores;

    boot_os();

    // Ask for the mode
    printf("\nSelect Mode:\n");
    printf("1. User Mode\n");
    printf("2. Kernel Mode\n");
    printf("Enter your choice: ");
    int mode_choice;
    if (scanf("%d", &mode_choice) != 1) {
        print_error("Invalid input!");
        return 1;
    }

    current_mode = (mode_choice == 2) ? 1 : 0;

    int choice;
    do {
        clear_screen();
        print_header();
        show_main_menu();

        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            print_error("Invalid input!");
            choice = 0; // Reset choice to prevent invalid behavior
            while (getchar() != '\n'); // Clear input buffer
        }

        switch (choice) {
        case 1:
            if (current_mode == 0) {
                execute_task("Notepad");
            }
            else {
                memory_viewer();
            }
            break;
        case 2:
            if (current_mode == 0) {
                execute_task("Calculator");
            }
            else {
                file_info();
            }
            break;
        case 3:
            if (current_mode == 0) {
                execute_task("Time");
            }
            else {
                process_manager();
            }
            break;
        case 4:
            if (current_mode == 0) {
                execute_task("Calendar");
            }
            else {
                system_monitor();
            }
            break;
        case 5:
            if (current_mode == 0) {
                execute_task("Create File");
            }
            else {
                close_task(choice);
            }
            break;
        case 6:
            if (current_mode == 0) {
                execute_task("Move File");
            }
            else {
                minimize_task(choice);
            }
            break;
        case 7:
            if (current_mode == 0) {
                execute_task("Copy File");
            }
            else {
                restore_task(choice);
            }
            break;
        case 8:
            if (current_mode == 0) {
                execute_task("Delete File");
            }
            else {
                show_scheduling_info();
            }
            break;
        case 9:
            if (current_mode == 0) {
                set_scheduling_algorithm();
            }
            else {
                switch_mode(); // Switch back to User Mode
            }
            break;
        case 10: execute_task("Minesweeper"); break;
        case 11: execute_task("Music Player"); break;
        case 12: execute_task("Snake Game"); break;
        case 13: execute_task("Help System"); break;
        case 14: show_running_tasks(); break;
        case 15: end_task_immediately(); break;
        case 16: switch_mode(); break;
        case 17: shutdown_os(); break;
        default: print_error("Invalid choice!"); sleep(1); break;
        }

        // Schedule tasks after each operation that might affect the task queue
        if (choice != 17 && choice != 14 && choice != 15 && choice != 16) {
            schedule_tasks();
        }
    } while (choice != 17);

    sem_destroy(&resource_sem);
    pthread_mutex_destroy(&queue_mutex);

    return 0;
}

void boot_os() {
    clear_screen();
    printf("\n\n");
    printf("            OD's\n");
    printf("            Operating System Simulator\n");

    loading_animation("Booting OS", 3);

    // Removed the calendar prompt to prevent asking to open the calendar at the beginning
}

void show_main_menu() {
    printf("\n%s+=======================================================+%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s|                                MAIN MENU              |%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s+=======================================================+%s\n", COLOR_AQUA, COLOR_RESET);

    if (current_mode == 0) {
        printf("%s|  1. Notepad            | 10. Minesweeper        |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  2. Calculator         | 11. Music Player       |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  3. Time               | 12. Snake Game         |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  4. Calendar           | 13. Help System        |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  5. Create File        | 14. Show Tasks         |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  6. Move File          | 15. End Task Now       |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  7. Copy File          | 16. Switch Mode        |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  8. Delete File        | 17. Shutdown           |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s|  9. Set CPU Scheduling |                        |%s\n", COLOR_YELLOW, COLOR_RESET);
    }
    else {
        printf("%s| 1. Memory Viewer%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 2. File Info%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 3. Process Manager%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 4. System Monitor%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 5. Close Task%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 6. Minimize Task%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 7. Restore Task%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 8. Show Scheduling Info%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| 9. Switch to User Mode%s\n", COLOR_YELLOW, COLOR_RESET); // Option to switch back to User Mode
    }

    printf("%s+-----------------------------------------------+%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s| Current Mode: %s%s\n", COLOR_AQUA, current_mode ? "Kernel" : "User", COLOR_RESET);
    printf("%s+================================================+%s\n", COLOR_AQUA, COLOR_RESET);
}

void execute_task(char* task_name) {
    int ram = 0, hdd = 0, cpu = 0;

    // Set resource requirements based on task name
    if (strcmp(task_name, "Notepad") == 0) {
        ram = 50; hdd = 5; cpu = 1;
    }
    else if (strcmp(task_name, "Calculator") == 0) {
        ram = 20; hdd = 1; cpu = 1;
    }
    else if (strcmp(task_name, "Time") == 0) {
        ram = 10; hdd = 1; cpu = 1;
    }
    else if (strcmp(task_name, "Calendar") == 0) {
        ram = 15; hdd = 2; cpu = 1;
    }
    else if (strcmp(task_name, "Create File") == 0) {
        ram = 30; hdd = 10; cpu = 1;
    }
    else if (strcmp(task_name, "Move File") == 0) {
        ram = 40; hdd = 10; cpu = 1;
    }
    else if (strcmp(task_name, "Copy File") == 0) {
        ram = 40; hdd = 10; cpu = 1;
    }
    else if (strcmp(task_name, "Delete File") == 0) {
        ram = 30; hdd = 1; cpu = 1;
    }
    else if (strcmp(task_name, "File Info") == 0) {
        ram = 25; hdd = 1; cpu = 1;
    }
    else if (strcmp(task_name, "Minesweeper") == 0) {
        ram = 60; hdd = 10; cpu = 2;
    }
    else if (strcmp(task_name, "Music Player") == 0) {
        ram = 40; hdd = 20; cpu = 1;
    }
    else if (strcmp(task_name, "System Monitor") == 0) {
        ram = 50; hdd = 5; cpu = 2;
    }
    else if (strcmp(task_name, "Process Manager") == 0) {
        ram = 45; hdd = 5; cpu = 2;
    }
    else if (strcmp(task_name, "Memory Viewer") == 0) {
        ram = 35; hdd = 5; cpu = 1;
    }
    else if (strcmp(task_name, "Snake Game") == 0) {
        ram = 55; hdd = 10; cpu = 2;
    }
    else if (strcmp(task_name, "Help System") == 0) {
        ram = 30; hdd = 5; cpu = 1;
    }

    create_process(task_name, ram, hdd, cpu);
}

void create_process(char* task_name, int ram, int hdd, int cpu) {
    if (!check_resources(ram, hdd, cpu)) {
        print_error("Not enough resources to start this task!");
        sleep(1);
        return;
    }

    printf("Run in background? (y/n): ");
    char bg;
    scanf(" %c", &bg);
    int run_in_background = (bg == 'y' || bg == 'Y');

    if (run_in_background) {
        // Use system call to open a new terminal window and run the task
        char command[256];
        snprintf(command, sizeof(command), "gnome-terminal -- bash -c './your_program_name %s; exec bash'", task_name);
        system(command);

        pthread_mutex_lock(&queue_mutex);

        if (task_count < MAX_TASKS) {
            // Simulate process creation for tracking purposes
            tasks[task_count].pid = -1; // Using -1 as a placeholder since we're not actually forking
            strcpy(tasks[task_count].name, task_name);
            tasks[task_count].ram_usage = ram;
            tasks[task_count].hdd_usage = hdd;
            tasks[task_count].cpu_usage = cpu;
            tasks[task_count].is_running = 1;
            tasks[task_count].is_minimized = 0;
            tasks[task_count].start_time = time(NULL);
            tasks[task_count].priority = rand() % 5 + 1;
            tasks[task_count].remaining_time = (rand() % 10) + 1;

            manage_resources(ram, hdd, cpu, 1);
            task_count++;

            print_success("Task started in background in a new terminal!");
        }
        else {
            print_error("Maximum number of tasks reached!");
        }

        pthread_mutex_unlock(&queue_mutex);
        sleep(1);
    }
    else {
        // Existing code for foreground execution
        if (strcmp(task_name, "Notepad") == 0) {
            notepad();
        }
        else if (strcmp(task_name, "Calculator") == 0) {
            calculator();
        }
        else if (strcmp(task_name, "Time") == 0) {
            show_time();
        }
        else if (strcmp(task_name, "Calendar") == 0) {
            calendar();
        }
        else if (strcmp(task_name, "Create File") == 0) {
            create_file();
        }
        else if (strcmp(task_name, "Move File") == 0) {
            move_file();
        }
        else if (strcmp(task_name, "Copy File") == 0) {
            copy_file();
        }
        else if (strcmp(task_name, "Delete File") == 0) {
            delete_file();
        }
        else if (strcmp(task_name, "File Info") == 0) {
            file_info();
        }
        else if (strcmp(task_name, "Minesweeper") == 0) {
            minesweeper();
        }
        else if (strcmp(task_name, "Music Player") == 0) {
            music_player();
        }
        else if (strcmp(task_name, "System Monitor") == 0) {
            system_monitor();
        }
        else if (strcmp(task_name, "Process Manager") == 0) {
            process_manager();
        }
        else if (strcmp(task_name, "Memory Viewer") == 0) {
            memory_viewer();
        }
        else if (strcmp(task_name, "Snake Game") == 0) {
            snake_game();
        }
        else if (strcmp(task_name, "Help System") == 0) {
            help_system();
        }
    }
}

void schedule_tasks() {
    if (task_count == 0) return;

    pthread_mutex_lock(&queue_mutex);

    switch (current_scheduler) {
    case FCFS:
        // First-Come-First-Serve - no reordering needed
        break;

    case ROUND_ROBIN: {
        // Move the first task to the end of the queue
        Task temp = tasks[0];
        for (int i = 0; i < task_count - 1; i++) {
            tasks[i] = tasks[i + 1];
        }
        tasks[task_count - 1] = temp;

        // Decrease remaining time for current task
        tasks[task_count - 1].remaining_time -= TIME_QUANTUM;

        // If task is done, remove it
        if (tasks[task_count - 1].remaining_time <= 0) {
            manage_resources(tasks[task_count - 1].ram_usage,
                tasks[task_count - 1].hdd_usage,
                tasks[task_count - 1].cpu_usage, 0);
            kill(tasks[task_count - 1].pid, SIGTERM);
            waitpid(tasks[task_count - 1].pid, NULL, 0);
            task_count--;
        }
        break;
    }

    case PRIORITY: {
        // Sort tasks by priority (higher priority first)
        for (int i = 0; i < task_count - 1; i++) {
            for (int j = 0; j < task_count - i - 1; j++) {
                if (tasks[j].priority < tasks[j + 1].priority) {
                    Task temp = tasks[j];
                    tasks[j] = tasks[j + 1];
                    tasks[j + 1] = temp;
                }
            }
        }
        break;
    }
    }

    pthread_mutex_unlock(&queue_mutex);
}

void set_scheduling_algorithm() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s        CPU Scheduling Algorithm%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%sCurrent algorithm: %s%s\n", COLOR_AQUA,
        current_scheduler == FCFS ? "First-Come-First-Serve" :
        current_scheduler == ROUND_ROBIN ? "Round Robin" : "Priority", COLOR_RESET);

    printf("\nSelect new algorithm:\n");
    printf("1. First-Come-First-Serve (FCFS)\n");
    printf("2. Round Robin\n");
    printf("3. Priority Scheduling\n");
    printf("4. Back to Main Menu\n");

    int choice;
    printf("\nEnter your choice: ");
    if (scanf("%d", &choice) != 1) {
        print_error("Invalid input!");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    switch (choice) {
    case 1: current_scheduler = FCFS; break;
    case 2: current_scheduler = ROUND_ROBIN; break;
    case 3: current_scheduler = PRIORITY; break;
    case 4: return;
    default: print_error("Invalid choice!"); sleep(1); return;
    }

    print_success("Scheduling algorithm changed!");
    sleep(1);
}

void show_scheduling_info() {
    printf("\n%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s        CPU Scheduling Information%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%sCurrent algorithm: %s%s\n", COLOR_AQUA,
        current_scheduler == FCFS ? "First-Come-First-Serve" :
        current_scheduler == ROUND_ROBIN ? "Round Robin" : "Priority", COLOR_RESET);

    if (current_scheduler == ROUND_ROBIN) {
        printf("%sTime Quantum: %d seconds%s\n", COLOR_AQUA, TIME_QUANTUM, COLOR_RESET);
    }

    printf("\nTask Queue:\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%-5s %-20s %-10s %-10s %-10s\n",
        "ID", "Name", "Priority", "Rem Time", "Status");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    for (int i = 0; i < task_count; i++) {
        printf("%-5d %-20s %-10d %-10d %-10s\n",
            i,
            tasks[i].name,
            tasks[i].priority,
            tasks[i].remaining_time,
            tasks[i].is_minimized ? "Minimized" : "Running");
    }
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void end_task_immediately() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s        End Task Immediately%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    if (task_count == 0) {
        printf("No tasks are currently running.\n");
        sleep(1);
        return;
    }

    printf("Running Tasks:\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
    for (int i = 0; i < task_count; i++) {
        printf("%d. %s (PID: %d)\n", i, tasks[i].name, tasks[i].pid);
    }
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nEnter task number to end (or -1 to cancel): ");
    int task_num;
    if (scanf("%d", &task_num) != 1) {
        print_error("Invalid input!");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    if (task_num >= 0 && task_num < task_count) {
        close_task(task_num);
    }
}

void show_running_tasks() {
    clear_screen();
    print_header();
    printf("\n%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            RUNNING TASKS%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    if (task_count == 0) {
        printf("No tasks are currently running.\n");
    }
    else {
        printf("%-5s %-20s %-10s %-10s %-10s %-10s %-15s\n",
            "ID", "Name", "RAM(MB)", "HDD(MB)", "CPU", "Status", "Running Time");
        printf("%s------------------------------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

        for (int i = 0; i < task_count; i++) {
            if (tasks[i].is_running) {
                time_t now = time(NULL);
                double running_time = difftime(now, tasks[i].start_time);

                printf("%-5d %-20s %-10d %-10d %-10d %-10s %.0f seconds\n",
                    i,
                    tasks[i].name,
                    tasks[i].ram_usage,
                    tasks[i].hdd_usage,
                    tasks[i].cpu_usage,
                    tasks[i].is_minimized ? "Minimized" : "Running",
                    running_time);
            }
        }
    }

    if (current_mode == 1) {
        printf("\nKernel Mode Options:\n");
        printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
        printf("1. Memory Viewer\n");
        printf("2. File Info\n");
        printf("3. Process Manager\n");
        printf("4. System Monitor\n");
        printf("5. Close Task\n");
        printf("6. Minimize Task\n");
        printf("7. Restore Task\n");
        printf("8. Show Scheduling Info\n");
        printf("9. Back to Main Menu\n");
        printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

        int choice, task_id;
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            print_error("Invalid input!");
            while (getchar() != '\n'); // Clear input buffer
            return;
        }

        if (choice >= 1 && choice <= 4) {
            switch (choice) {
            case 1: memory_viewer(); break;
            case 2: file_info(); break;
            case 3: process_manager(); break;
            case 4: system_monitor(); break;
            }
        }
        else if (choice >= 5 && choice <= 7) {
            printf("Enter Task ID: ");
            if (scanf("%d", &task_id) != 1) {
                print_error("Invalid input!");
                while (getchar() != '\n'); // Clear input buffer
                return;
            }

            if (task_id >= 0 && task_id < task_count && tasks[task_id].is_running) {
                switch (choice) {
                case 5: close_task(task_id); break;
                case 6: minimize_task(task_id); break;
                case 7: restore_task(task_id); break;
                }
            }
            else {
                print_error("Invalid Task ID!");
                sleep(1);
            }
        }
        else if (choice == 8) {
            show_scheduling_info();
        }
    }
    else {
        printf("\nPress any key to continue...");
        getchar(); getchar();
    }
}

void close_task(int task_index) {
    if (task_index < 0 || task_index >= task_count || !tasks[task_index].is_running) {
        print_error("Invalid task index!");
        return;
    }

    if (kill(tasks[task_index].pid, SIGTERM) == -1) {
        perror("Failed to terminate task");
        return;
    }

    if (waitpid(tasks[task_index].pid, NULL, 0) == -1) {
        perror("Failed to wait for task");
        return;
    }

    manage_resources(tasks[task_index].ram_usage,
        tasks[task_index].hdd_usage,
        tasks[task_index].cpu_usage, 0);

    pthread_mutex_lock(&queue_mutex);

    for (int i = task_index; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    task_count--;

    pthread_mutex_unlock(&queue_mutex);

    print_success("Task closed successfully!");
    sleep(1);
}

void minimize_task(int task_index) {
    if (task_index < 0 || task_index >= task_count || !tasks[task_index].is_running) {
        print_error("Invalid task index!");
        return;
    }

    tasks[task_index].is_minimized = 1;
    print_success("Task minimized successfully!");
    sleep(1);
}

void restore_task(int task_index) {
    if (task_index < 0 || task_index >= task_count || !tasks[task_index].is_running) {
        print_error("Invalid task index!");
        return;
    }

    tasks[task_index].is_minimized = 0;
    print_success("Task restored successfully!");
    sleep(1);
}

void switch_mode() {
    current_mode = !current_mode;
    print_success(current_mode ? "Switched to Kernel Mode" : "Switched to User Mode");
    sleep(1);
}

void shutdown_os() {
    clear_screen();
    printf("\n\n");
    printf("SHUTTING DOWN SYSTEM..........");

    for (int i = 0; i < task_count; i++) {
        if (tasks[i].is_running) {
            kill(tasks[i].pid, SIGTERM);
            waitpid(tasks[i].pid, NULL, 0);
        }
    }

    loading_animation("Shutting down", 3);
    exit(0);
}

void manage_resources(int ram, int hdd, int cpu, int allocate) {
    sem_wait(&resource_sem);

    if (allocate) {
        system_res.available_ram -= ram;
        system_res.available_hdd -= hdd;
        system_res.available_cores -= cpu;
    }
    else {
        system_res.available_ram += ram;
        system_res.available_hdd += hdd;
        system_res.available_cores += cpu;
    }

    sem_post(&resource_sem);
}

int check_resources(int ram, int hdd, int cpu) {
    sem_wait(&resource_sem);

    int result = (system_res.available_ram >= ram) &&
        (system_res.available_hdd >= hdd) &&
        (system_res.available_cores >= cpu);

    sem_post(&resource_sem);
    return result;
}

void notepad() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s              NOTEPAD%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("Type your text (enter 'SAVE' on a new line to save and exit):\n\n");

    char filename[MAX_PATH_LENGTH];
    printf("Enter filename to save: ");
    scanf("%s", filename);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file!\n");
        return;
    }

    char buffer[256];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "SAVE\n") == 0) {
            break;
        }

        fputs(buffer, file);
    }

    fclose(file);
    printf("File saved successfully as %s\n", filename);
    sleep(2);
}

void calculator() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s             CALCULATOR%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("Operations: +, -, *, /, %% (enter 'q' to quit)\n\n");

    while (1) {
        double num1, num2;
        char op;

        printf("Enter expression (e.g., 5 + 3): ");
        if (scanf("%lf %c %lf", &num1, &op, &num2) != 3) {
            break;
        }

        double result;
        switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                printf("Error: Division by zero!\n");
                continue;
            }
            result = num1 / num2;
            break;
        case '%':
            result = (int)num1 % (int)num2;
            break;
        default:
            printf("Invalid operator!\n");
            continue;
        }

        printf("Result: %.2lf\n", result);

        while ((getchar()) != '\n');

        printf("Press q to quit or any other key to continue...");
        if (getchar() == 'q') {
            break;
        }
    }
}

void show_time() {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            CURRENT TIME%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    printf("%02d/%02d/%04d\n", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void calendar() {
    while (1) {
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);

        clear_screen();
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s              CALENDAR%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

        printf("     %02d/%04d\n", tm_info->tm_mon + 1, tm_info->tm_year + 1900);
        printf("Su Mo Tu We Th Fr Sa\n");

        struct tm first_day = *tm_info;
        first_day.tm_mday = 1;
        mktime(&first_day);

        int day_of_week = first_day.tm_wday;
        int days_in_month;

        switch (tm_info->tm_mon + 1) {
        case 4: case 6: case 9: case 11: days_in_month = 30; break;
        case 2:
            days_in_month = ((tm_info->tm_year + 1900) % 4 == 0) ? 29 : 28;
            break;
        default: days_in_month = 31;
        }

        for (int i = 0; i < day_of_week; i++) {
            printf("   ");
        }

        for (int day = 1; day <= days_in_month; day++) {
            printf("%2d ", day);
            if ((day + day_of_week) % 7 == 0 || day == days_in_month) {
                printf("\n");
            }
        }

        printf("\nPress 'q' to quit...");
        if (kbhit()) {
            char ch = getchar();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
        }

        sleep(1);
    }
}

void create_file() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            CREATE FILE%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    char filename[MAX_PATH_LENGTH];
    printf("Enter filename: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file!\n");
    }
    else {
        printf("File created successfully: %s\n", filename);
        fclose(file);
    }

    sleep(2);
}

void move_file() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s             MOVE FILE%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    char source[MAX_PATH_LENGTH], dest[MAX_PATH_LENGTH];
    printf("Enter source file path: ");
    scanf("%s", source);
    printf("Enter destination path: ");
    scanf("%s", dest);

    if (rename(source, dest) == 0) {
        printf("File moved successfully from %s to %s\n", source, dest);
    }
    else {
        printf("Error moving file!\n");
    }

    sleep(2);
}

void copy_file() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s             COPY FILE%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    char source[MAX_PATH_LENGTH], dest[MAX_PATH_LENGTH];
    printf("Enter source file path: ");
    scanf("%s", source);
    printf("Enter destination path: ");
    scanf("%s", dest);

    FILE* src_file = fopen(source, "rb");
    if (src_file == NULL) {
        printf("Error opening source file!\n");
        sleep(2);
        return;
    }

    FILE* dest_file = fopen(dest, "wb");
    if (dest_file == NULL) {
        printf("Error creating destination file!\n");
        fclose(src_file);
        sleep(2);
        return;
    }

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);

    printf("File copied successfully from %s to %s\n", source, dest);
    sleep(2);
}

void delete_file() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            DELETE FILE%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    char filename[MAX_PATH_LENGTH];
    printf("Enter filename to delete: ");
    scanf("%s", filename);

    if (remove(filename) == 0) {
        printf("File deleted successfully: %s\n", filename);
    }
    else {
        printf("Error deleting file!\n");
    }

    sleep(2);
}

void file_info() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            FILE INFO%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    char filename[MAX_PATH_LENGTH];
    printf("Enter filename: ");
    scanf("%s", filename);

    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        printf("Error getting file info!\n");
        sleep(2);
        return;
    }

    printf("\nFile Information for: %s\n", filename);
    printf("Size: %ld bytes\n", file_stat.st_size);
    printf("Permissions: %o\n", file_stat.st_mode & 0777);
    printf("Last accessed: %s", ctime(&file_stat.st_atime));
    printf("Last modified: %s", ctime(&file_stat.st_mtime));

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void minesweeper() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            MINESWEEPER%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("A simplified version of Minesweeper\n\n");

    const int SIZE = 5;
    const int MINES = 5;

    char board[SIZE][SIZE];
    char visible[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '0';
            visible[i][j] = '.';
        }
    }

    srand(time(NULL));
    for (int m = 0; m < MINES; ) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        if (board[x][y] != '*') {
            board[x][y] = '*';
            m++;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && board[nx][ny] != '*') {
                        board[nx][ny]++;
                    }
                }
            }
        }
    }

    int game_over = 0;
    int cells_revealed = 0;
    int total_safe = SIZE * SIZE - MINES;

    while (!game_over && cells_revealed < total_safe) {
        printf("   ");
        for (int i = 0; i < SIZE; i++) printf("%d ", i);
        printf("\n");

        for (int i = 0; i < SIZE; i++) {
            printf("%d |", i);
            for (int j = 0; j < SIZE; j++) {
                printf("%c ", visible[i][j]);
            }
            printf("\n");
        }

        int x, y;
        printf("\nEnter row and column (0-%d): ", SIZE - 1);
        scanf("%d %d", &x, &y);

        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            printf("Invalid coordinates!\n");
            continue;
        }

        if (visible[x][y] != '.') {
            printf("Cell already revealed!\n");
            continue;
        }

        if (board[x][y] == '*') {
            game_over = 1;
            visible[x][y] = '*';
        }
        else {
            visible[x][y] = board[x][y];
            cells_revealed++;
        }
    }

    if (game_over) {
        printf("\nBOOM! You hit a mine!\n");
    }
    else {
        printf("\nCongratulations! You cleared the minefield!\n");
    }

    printf("\nFinal Board:\n");
    printf("   ");
    for (int i = 0; i < SIZE; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d |", i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void music_player() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s            MUSIC PLAYER%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("Playing background music...\n");

    for (int i = 0; i < 5; i++) {
        if (task_count == 0 || !tasks[task_count - 1].is_running) break;

        printf("Playing note %d/5...\n", i + 1);
        beep_sound(500, 440 + i * 100);
        sleep(1);
    }

    printf("Music finished playing.\n");
    sleep(2);
}

void system_monitor() {
    while (1) {
        clear_screen();
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s          SYSTEM MONITOR%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

        printf("\nSystem Resources:\n");
        printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
        printf("RAM: %d/%d MB (%.1f%% used)\n",
            system_res.total_ram - system_res.available_ram,
            system_res.total_ram,
            ((float)(system_res.total_ram - system_res.available_ram) / system_res.total_ram * 100));
        printf("HDD: %d/%d MB (%.1f%% used)\n",
            system_res.total_hdd - system_res.available_hdd,
            system_res.total_hdd,
            ((float)(system_res.total_hdd - system_res.available_hdd) / system_res.total_hdd * 100));
        printf("CPU Cores: %d/%d in use\n",
            system_res.total_cores - system_res.available_cores,
            system_res.total_cores);
        printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

        printf("\nPress q to quit or any other key to refresh...");
        char ch = getchar();
        if (ch == 'q') {
            break;
        }
        while ((getchar()) != '\n'); // Clear input buffer
    }
}

void process_manager() {
    while (1) {
        clear_screen();
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s          PROCESS MANAGER%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

        if (task_count == 0) {
            printf("No processes running.\n");
        }
        else {
            printf("%-5s %-20s %-10s %-10s %-10s %-10s\n",
                "ID", "Name", "RAM(MB)", "HDD(MB)", "CPU", "Status");
            printf("%s------------------------------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

            for (int i = 0; i < task_count; i++) {
                printf("%-5d %-20s %-10d %-10d %-10d %-10s\n",
                    i,
                    tasks[i].name,
                    tasks[i].ram_usage,
                    tasks[i].hdd_usage,
                    tasks[i].cpu_usage,
                    tasks[i].is_minimized ? "Minimized" : "Running");
            }
        }

        printf("\nPress q to quit or any other key to refresh...");
        char ch = getchar();
        if (ch == 'q') {
            break;
        }
        while ((getchar()) != '\n'); // Clear input buffer
    }
}

void memory_viewer() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s          MEMORY VIEWER%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nMemory Allocation Map:\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
    printf("Total RAM: %d MB\n", system_res.total_ram);
    printf("Used RAM: %d MB\n", system_res.total_ram - system_res.available_ram);
    printf("Free RAM: %d MB\n", system_res.available_ram);
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nProcess Memory Usage:\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
    for (int i = 0; i < task_count; i++) {
        printf("%-20s: %4d MB\n", tasks[i].name, tasks[i].ram_usage);
    }
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

void snake_game() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s             SNAKE GAME%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("Use WASD keys to move. Press q to quit.\n");

    const int WIDTH = 20;
    const int HEIGHT = 10;

    int snakeX[100], snakeY[100];
    int snakeLength = 1;
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;

    int foodX = rand() % (WIDTH - 2) + 1;
    int foodY = rand() % (HEIGHT - 2) + 1;

    char direction = 'd';
    int game_over = 0;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (!game_over) {
        clear_screen();
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s             SNAKE GAME%s\n", COLOR_AQUA, COLOR_RESET);
        printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                    printf("#");
                }
                else if (i == foodY && j == foodX) {
                    printf("F");
                }
                else {
                    int isSnake = 0;
                    for (int k = 0; k < snakeLength; k++) {
                        if (snakeX[k] == j && snakeY[k] == i) {
                            printf("O");
                            isSnake = 1;
                            break;
                        }
                    }
                    if (!isSnake) printf(" ");
                }
            }
            printf("\n");
        }

        if (kbhit()) {
            char ch = getchar();
            switch (ch) {
            case 'w': if (direction != 's') direction = 'w'; break;
            case 'a': if (direction != 'd') direction = 'a'; break;
            case 's': if (direction != 'w') direction = 's'; break;
            case 'd': if (direction != 'a') direction = 'd'; break;
            case 'q': game_over = 1; continue;
            }
        }

        for (int i = snakeLength - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        switch (direction) {
        case 'w': snakeY[0]--; break;
        case 'a': snakeX[0]--; break;
        case 's': snakeY[0]++; break;
        case 'd': snakeX[0]++; break;
        }

        if (snakeX[0] <= 0 || snakeX[0] >= WIDTH - 1 ||
            snakeY[0] <= 0 || snakeY[0] >= HEIGHT - 1) {
            game_over = 1;
        }

        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
                game_over = 1;
            }
        }

        if (snakeX[0] == foodX && snakeY[0] == foodY) {
            snakeLength++;
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
        }

        usleep(200000);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("\nGame Over! Your score: %d\n", snakeLength - 1);
    printf("Press any key to continue...");
    getchar();
}

void help_system() {
    clear_screen();
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s             HELP SYSTEM%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%s======================================%s\n", COLOR_AQUA, COLOR_RESET);

    printf("Available Commands:\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
    printf("1. Notepad - Simple text editor\n");
    printf("2. Calculator - Basic arithmetic operations\n");
    printf("3. Time - Shows current time and date\n");
    printf("4. Calendar - Shows current month calendar\n");
    printf("5. Create File - Creates a new empty file\n");
    printf("6. Move File - Moves a file to new location\n");
    printf("7. Copy File - Copies a file to new location\n");
    printf("8. Delete File - Deletes a file\n");
    printf("9. File Info - Shows information about a file\n");
    printf("10. Minesweeper - Simple minesweeper game\n");
    printf("11. Music Player - Plays simple background music\n");
    printf("12. System Monitor - Shows system resource usage\n");
    printf("13. Process Manager - Shows running processes\n");
    printf("14. Memory Viewer - Shows memory allocation\n");
    printf("15. Snake Game - Classic snake game\n");
    printf("16. Help System - Shows this help message\n");
    printf("17. Show Running Tasks - List all running tasks\n");
    printf("18. Switch Mode - Toggle between User and Kernel mode\n");
    printf("19. Shutdown - Shuts down the OS\n");
    printf("20. Set CPU Scheduling - Change CPU scheduling algorithm\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nIn Kernel Mode, you can:\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);
    printf("- Close running tasks\n");
    printf("- Minimize tasks\n");
    printf("- Restore minimized tasks\n");
    printf("- View scheduling information\n");
    printf("%s--------------------------------------%s\n", COLOR_AQUA, COLOR_RESET);

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_header() {
    printf("%s=============================================================================================%s\n", COLOR_AQUA, COLOR_RESET);
    printf("%sOS Simulator - RAM: %d/%d MB | HDD: %d/%d MB | Cores: %d/%d | Mode: %s | Scheduler: %s%s\n", COLOR_AQUA,
        system_res.total_ram - system_res.available_ram, system_res.total_ram,
        system_res.total_hdd - system_res.available_hdd, system_res.total_hdd,
        system_res.total_cores - system_res.available_cores, system_res.total_cores,
        current_mode ? "Kernel" : "User",
        current_scheduler == FCFS ? "FCFS" :
        current_scheduler == ROUND_ROBIN ? "Round Robin" : "Priority", COLOR_RESET);
    printf("%s=============================================================================================%s\n", COLOR_AQUA, COLOR_RESET);
}

void print_error(char* message) {
    printf("%s[ERROR] Attention: %s%s\n", COLOR_RED, message, COLOR_RESET);
}

void print_success(char* message) {
    printf("%s[SUCCESS] Operation Successful: %s%s\n", COLOR_GREEN, message, COLOR_RESET);
}

void print_warning(char* message) {
    printf("%s[WARNING] Caution: %s%s\n", COLOR_YELLOW, message, COLOR_RESET);
}

void print_info(char* message) {
    printf("%s[INFO] Note: %s%s\n", COLOR_AQUA, message, COLOR_RESET);
}

void loading_animation(char* message, int seconds) {
    printf("\n%s%s%s ", COLOR_AQUA, message, COLOR_RESET);
    fflush(stdout);

    for (int i = 0; i < seconds; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

void beep_sound(int duration_ms, int frequency) {
#ifdef _WIN32
    Beep(frequency, duration_ms);
#else
    char command[50];
    sprintf(command, "beep -f %d -l %d", frequency, duration_ms);
    system(command);
#endif
}