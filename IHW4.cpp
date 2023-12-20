#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


#define MAX_HONEY 30
#define NUM_BEES 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t honey_collected = PTHREAD_COND_INITIALIZER;

int honey = 0; // Honey in the hive
int num_bees_in_hive = 0; // Number of bees in the hive
int pooh_speed = 0; // Winnie the Pooh's speed
int max_pooh_capacity = 0; // Winnie the Pooh's max honey capacity
int pooh_heal_speed = 0; // Winnie the Pooh's heal speed
int num_bees_undef = 0; // Bees amount

void *bee_thread(void *id) {
    
    int bee_id = *((int *)id); // Bee's ID
    printf("Bee %d created\n", bee_id); // Bee's creation message
    while (max_pooh_capacity > 0) {
        
        num_bees_in_hive++;
        // Check if honey is full
        while (honey >= MAX_HONEY) {
            pthread_mutex_lock(&mutex);
            printf("Bee %a ended work and is going to sleep.\n", bee_id);
            num_bees_in_hive = 0;
            pthread_cond_wait(&honey_collected, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        // Bee's work
        sleep(rand() % 3 + 1);
        if (honey < MAX_HONEY) {
            honey++;
            printf("Bee %d collected honey. Total honey: %d\n", bee_id, honey);
        }
        

        // Simulate bee is waiting for other bees to come back
        while (num_bees_in_hive < 2 && max_pooh_capacity > 0) {
            printf("Bee %d is waiting for other bees to come back\n", bee_id);
            sleep(2);
        }
        // Check if bees count isn't negative number
        if (num_bees_in_hive > 0) {
            num_bees_in_hive--;
        }
        sleep(rand() % 9 + 1);
    }
}

void *winnie_thread(void* arg) {
    while (max_pooh_capacity > 0) {
        pthread_mutex_lock(&mutex);
        // Check if honey is enough for Pooh to wake up
        if (honey > MAX_HONEY/2) {
            printf("Winnie the Pooh woke up\n");
            if (num_bees_in_hive > 3) {
                printf("ouch \n");
                sleep(pooh_heal_speed);
            } else {
                printf("Winnie the Pooh woke up and took all the honey! Yummy!\n");
                honey = 0;
                pthread_cond_broadcast(&honey_collected);
                --max_pooh_capacity;
            }
        }
        pthread_mutex_unlock(&mutex);

        // Simulate Winnie the Pooh's sleep
        sleep(pooh_speed);
    }
}

void randomator() {
    pooh_speed = rand() % 5 + 1;
    max_pooh_capacity = rand() % 10 + 1;
    pooh_heal_speed = rand() % 5 + 1;
    num_bees_undef = rand() % 20 + 4;
    #undef NUM_BEES
    #define NUM_BEES num_bees_undef
}

void getData() {
    // Get Pooh's max speed
    std::cout << "Enter Pooh's speed in seconds(Recomended: 1 sec): ";    
    while (!(std::cin >> pooh_speed)){
        std::cout << "Ooops! Non integer value entered! Input integer: ";
        std::cin.clear();
        std::cin.ignore(40,'\n');
    }
    // Get Pooh's max honey capacity
    std::cout << "Enter Pooh's max honey capacity(Recomended: 10): ";
    while (!(std::cin >> max_pooh_capacity)){
        std::cout << "Ooops! Non integer value entered! Input integer: ";
        std::cin.clear();
        std::cin.ignore(40,'\n');
    }
    // Get Pooh's heal speed
    std::cout << "Enter Pooh's heal speed in seconds(Recomended: 1 sec): ";
    while (!(std::cin >> pooh_heal_speed)){
        std::cout << "Ooops! Non integer value entered! Input integer: ";
        std::cin.clear();
        std::cin.ignore(40,'\n');
    }
    // Get bees amount
    
    std::cout << "Enter bees amount(Recomended: 20; Min: 4): ";
    while (!(std::cin >> num_bees_undef) || num_bees_undef < 4){
        std::cout << "Ooops! Non integer value entered or amount is less than min! Input integer: ";
        std::cin.clear();
        std::cin.ignore(40,'\n');
    }
    #undef NUM_BEES
    #define NUM_BEES num_bees_undef
}

void commandLineData(int argc, char *argv[]) {
    if (argc == 5) {
        std::cout << "You've entered enough arguments! So program will run with command line arguments!\n";
        if (atoi(argv[1]) < 1) {
            std::cout << "You've entered less than 1 second for Pooh's speed! So program will run with default speed!\n";
            getData();
        }
        else if (atoi(argv[2]) < 1) {
            std::cout << "You've entered less than 1 for Pooh's max honey capacity! So program will run with default max honey capacity!\n";
            getData();
        }       
        else if (atoi(argv[3]) < 1) {
            std::cout << "You've entered less than 1 second for Pooh's heal speed! So program will run with default heal speed!\n";
            getData();
        }       
        else if (atoi(argv[4]) < 4) {
            std::cout << "You've entered less than 4 bees! So program will run with default bees amount!\n";
            getData();
        } else {
            pooh_speed = atoi(argv[1]);
            max_pooh_capacity = atoi(argv[2]);
            pooh_heal_speed = atoi(argv[3]);
            num_bees_undef = atoi(argv[4]);
            #undef NUM_BEES
            #define NUM_BEES num_bees_undef
        }
        
    } else {
        getData();
    }
}

int main(int argc, char *argv[]) {
    std::cout << "Welcome to the Winnie the Pooh's honey hunt!\n";
    std::cout << "If you want to enter data via command line arguments or via console - 1\n";
    std::cout << "If you want to enter data via console enter - 2\n";
    std::cout << "If you want to random data - 3\n";
    int choice;
    std::cout << "Enter your decision: ";
    while (!(std::cin >> choice) || choice < 1 || choice > 3){
        std::cout << "Ooops! Non integer value entered! Input integer(1 - 3): ";
        std::cin.clear();
        std::cin.ignore(40,'\n');
    }
    switch (choice) {
        case 1:
            commandLineData(argc, argv);
            break;
        case 2:
            getData();
            break;
        case 3:
            randomator();
            break;
        default:
            break;
    }
    
    pthread_t bees[NUM_BEES];
    pthread_t winnie;

    // Create bee threads
    int bee_ids[NUM_BEES];
    for (int i = 0; i < NUM_BEES; i++) {
        bee_ids[i] = i + 1;
        pthread_create(&bees[i], NULL, bee_thread, (void *)&bee_ids[i]);
    }

    // Create Winnie the Pooh thread
    pthread_create(&winnie, NULL, winnie_thread, NULL);

    // Join threads
    for (int i = 0; i < NUM_BEES; i++) {
        pthread_join(bees[i], NULL);
    }
    pthread_join(winnie, NULL);

    return 0;
}
