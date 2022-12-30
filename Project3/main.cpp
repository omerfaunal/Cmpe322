/*
@Author: Omer Faruk Unal

You can run the program with the following command: 
make run input={input_path}
*/

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <sstream>

using namespace std;

int num_customers;

ofstream fileOut;
pthread_mutex_t writeLock;
pthread_mutex_t machineTransactionLock[11];
pthread_mutex_t machineInputLock[11];

vector<string> transactions(11);

//Global variables for the total amount of money for threads
int kevin = 0;
int bob = 0;
int stuart = 0;
int otto = 0;
int dave = 0;

int t_count = 0;

void *machine(void *param);
void *customer(void *param);

int main(int argc,  char **argv){

    string fileName = argv[1]; 
    vector<string> lines;

    //Read the file and store the lines in a vector
    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        getline(file, line);
        num_customers = stoi(line);
        for(int i = 0; i < num_customers; i++) {
            getline(file, line);
            //Add the customer number to the line
            line.append(",Customer" + to_string(i + 1));
            //Add the line to the vector
            lines.push_back(line);
        }
        file.close();
    } else {
        cout << "Unable to open file";
        return 1;
    }

    //Create the output file
    string outFileName = fileName.substr(0, fileName.size() - 4) + "_log.txt";
    fileOut.open(outFileName);

    //Create the threads for the machines and customers
    pthread_t machines[11];
    int nums[11];
    pthread_t customers[num_customers];

    //Initialize the mutexes
    pthread_mutex_init(&writeLock, NULL);
    for (int i = 1; i < 11; i++) {
        pthread_mutex_init(&machineTransactionLock[i], NULL);
        //Lock the machine transaction mutex so that the machines will wait for the customers
        pthread_mutex_lock(&machineTransactionLock[i]);
        pthread_mutex_init(&machineInputLock[i], NULL);
    }


    for (int i = 1; i < 11; i++) {
        nums[i] = i;
        pthread_create(&machines[i], 0, machine, &nums[i]);
    }
        
    for (int i = 0; i < num_customers; i++) 
        pthread_create(&customers[i], 0, customer, &lines[i]);

    for (int i = 0; i < num_customers; i++)
        pthread_join(customers[i], NULL);
    
    for (int i = 1; i < 11; i++) 
        pthread_join(machines[i], NULL);
    

    fileOut << "[Main]: All prepayments are completed." << endl;
    fileOut << "[Main]: Kevin: " << kevin << "TL" << endl;
    fileOut << "[Main]: Bob: " << bob << "TL" << endl;
    fileOut << "[Main]: Stuart: " << stuart << "TL" << endl;
    fileOut << "[Main]: Otto: " << otto << "TL" << endl;
    fileOut << "[Main]: Dave: " << dave << "TL";
    fileOut.close();
    
    return 0;
}


void* machine(void *param) {
    //Get the number of the atm machine
    int machineNo = *(int *)param;

    //Create an array to store the values of the bank accounts
    //Kevin, Bob, Stuart, Otto, Dave
    int values[5];
    
    while(true){
        /*
        Wait for the customer to write the transaction
        When the customer writes the transaction, the mutex will be unlocked
        and the machine will read the transaction
        After reading the transaction, the mutex will be locked again
        This mutex is for machines to work on just one customer (e.g. transaction) at a time
        */
        pthread_mutex_lock(&machineTransactionLock[machineNo]);
        //Read the transaction
        string line = transactions[machineNo];
        //Unlock the input mutex so that the customer can write the next transaction
        pthread_mutex_unlock(&machineInputLock[machineNo]);

        //If the transaction count is equal to the number of customers, then all the transactions are done
        if (t_count >= num_customers) {
            //Lock the write lock so that no more than one thread can write to variables at a time
            pthread_mutex_lock(&writeLock);
            kevin += values[0];
            bob += values[1];
            stuart += values[2];
            otto += values[3];
            dave += values[4];
            pthread_mutex_unlock(&writeLock);
            break;
        }

        //Parse the transaction
        vector<string> v;
        stringstream my_str(line);
        
        while(my_str.good())
        {
            string substr;
            getline(my_str, substr, ',');
            v.push_back(substr);
        }

        int amount = stoi(v[3]);

        //Create the log
        string log = "[VTM" + to_string(machineNo) + "]: " + v[4] + "," + v[3] + "TL" + "," + v[2];
        string name = v[2];

        //Add the amount to the correct bank account
        if (name == "Kevin")
            values[0] += amount;
        else if (name == "Bob")
            values[1] += amount;
        else if (name == "Stuart")
            values[2] += amount;
        else if (name == "Otto")
            values[3] += amount;
        else if (name == "Dave")
            values[4] += amount;

        //Lock the write lock so that no more than one thread can write to file at a time
        pthread_mutex_lock(&writeLock);
        fileOut << log << endl;
        t_count++;
        pthread_mutex_unlock(&writeLock);
        
        /*
        If all the transactions are done, unlock the machine transaction mutexes
        so that the machines can write the final values to the bank accounts
        */
        if (t_count == num_customers) {
            for (int i = 1; i < 11; i++){
                pthread_mutex_unlock(&machineTransactionLock[i]);
            }
        }
        
    }
    pthread_exit(0);
}


void* customer(void *param) {
    
    int sleep, machineNo;
    string line = *(string *)param;
    vector<string> v;
    stringstream my_str(line);
    while(my_str.good()){
        string substr;
        getline(my_str, substr, ',');
        v.push_back(substr);
    }

    sleep = stoi(v[0]);
    //Sleep for the given time
    usleep(sleep * 1000);
    machineNo = stoi(v[1]);

    //Lcok the input mutex so that no more than one customer can write to the transaction array at a time
    //This will be unlocked when the machine reads the transaction
    pthread_mutex_lock(&machineInputLock[machineNo]);
    transactions[machineNo] = line;
    //Unlock the transaction mutex so that the machine can process the transaction
    pthread_mutex_unlock(&machineTransactionLock[machineNo]);
    pthread_exit(0);
}
