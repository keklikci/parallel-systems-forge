#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#define FREE 0
#define RESERVED 1

using namespace std; 

// shared resource of threads, initially all are 0
int seats[2][50] = {0};
// track remaining seats, lock for busy waiting
int rem = 100;
int spin_lock = 0;

/*
int minBound = 1;
int maxBound = 3;
*/

// get random row number 
int RandomRow() {
    return rand() % 2;
}

// get random col number 
int RandomCol() {
    return rand() % 50;
}

// 0 if FREE, 1 if RESERVED 
bool CheckReservation(int row, int col) {
    // book the seat 
    if(seats[row][col] == FREE) {
        return FREE;
    } 
    return RESERVED;
}

// prints matrix to the console when all seats are reserved 
void printMatrix() {
    cout << "Plane is full:\n\n"; 
    int colsize = 50, rowsize = 2;
    for(int row = 0; row < rowsize; row++) {
        for(int col = 0; col < colsize; col++) {
            cout << seats[row][col] << " ";
        }
        cout << "\n";
    }
}

// first thread besides main
void *travelAgency_1(void *param) {
    while(true && rem != 0) {
        // sleep(rand() % maxBound + minBound);
        // sleep(1);
        while(spin_lock != 0) {}
        cout << "Agency 1 Entered Critical Region\n";
        int row = RandomRow();
        int col = RandomCol();
        // if the seat is free, reserve it 
        if(!CheckReservation(row,col)) {
            int *ptr = (int *) param; 
            int seatNum;
            if(row == 1) {
                seatNum = col + 50;
            }
            else {
                seatNum = col;
            }
            seats[row][col] = *ptr;
            cout << "Seat Number " << seatNum << " is reserved by Agency 1\n";
            rem--;
        }
        // cout << "Remaining Seats: " << rem << endl;
        cout << "Agency 1 Exit Critical Region\n\n";
        spin_lock = 1;
    } 
}

// second thread besides main
void *travelAgency_2(void *param) {
    while(true && rem != 0) {
        // sleep(rand() % maxBound + minBound);
        // sleep(1);
        while(spin_lock != 1) {}
        cout << "Agency 2 Entered Critical Region\n";
        int row = RandomRow();
        int col = RandomCol();
        // if the seat is free, reserve it 
        if(!CheckReservation(row,col)) {
            int *ptr = (int *) param; 
            int seatNum;
            if(row == 1) {
                seatNum = col + 50;
            }
            else {
                seatNum = col;
            }
            seats[row][col] = *ptr;
            cout << "Seat Number " << seatNum << " is reserved by Agency 2\n";
            rem--;
        }
        // cout << "Remaining Seats: " << rem << endl;
        cout << "Agency 2 Exit Critical Region\n\n";
        spin_lock = 0;
    } 
}

int main() {

    // set different seeds at each execution
    srand(time(NULL));  

    pthread_t agency_1, agency_2;
    int id1 = 1, id2 = 2;

    pthread_create(&agency_1, NULL, travelAgency_1, (void*) &id1);
    pthread_create(&agency_2, NULL, travelAgency_2, (void*) &id2);

    pthread_join(agency_1, NULL);
    pthread_join(agency_2, NULL);

    cout << "No seats left\n";
    printMatrix();

    return 0;
}
