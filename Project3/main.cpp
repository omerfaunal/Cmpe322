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
pthread_mutex_t machineLock[11];
pthread_mutex_t machineLock2[11];

vector<string> transactions(11);

//Kevin, Bob, Stuart, Otto, Dave
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
    vector<string> lines(num_customers);

    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        getline(file, line);
        num_customers = stoi(line);
        for(int i = 0; i < num_customers; i++) {
            getline(file, line);
            line.append(",Customer" + to_string(i + 1));
            lines.push_back(line);
        }
        file.close();
    }

    string outFileName = "output.txt";
    fileOut.open(outFileName);
    pthread_t machines[11];
    int nums[11];
    pthread_t customers[num_customers];

    pthread_mutex_init(&writeLock, NULL);
    for (int i = 1; i < 11; i++) {
        pthread_mutex_init(&machineLock[i], NULL);
        pthread_mutex_init(&machineLock2[i], NULL);
        pthread_mutex_lock(&machineLock[i]);
    }


    for (int i = 1; i < 11; i++) {
        nums[i] = i;
        pthread_create(&machines[i], 0, machine, &nums[i]);
    }
        
    for (int i = 0; i < num_customers; i++) 
        pthread_create(&customers[i], 0, customer, &lines[i]);
    
    for (int i = 1; i < 11; i++) 
        pthread_join(machines[i], NULL);
    
    for (int i = 0; i < num_customers; i++)
        pthread_join(customers[i], NULL);


    fileOut << "All prepayments are completed." << endl;
    fileOut << "Kevin: " << kevin << "TL" << endl;
    fileOut << "Bob: " << bob << "TL" << endl;
    fileOut << "Stuart: " << stuart << "TL" << endl;
    fileOut << "Otto: " << otto << "TL" << endl;
    fileOut << "Dave: " << dave << "TL";

    fileOut.close();
    
    return 0;
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
    machineNo = stoi(v[1]);
    usleep(sleep * 1000);
    
    pthread_mutex_lock(&machineLock2[machineNo]);
    transactions[machineNo] = line;
    pthread_mutex_unlock(&machineLock[machineNo]);
    pthread_exit(0);
}


void* machine(void *param) {
    int machineNo = *(int *)param;

    int values[5];
    
    while(true){
        pthread_mutex_lock(&machineLock[machineNo]);
        string line = transactions[machineNo];
        pthread_mutex_unlock(&machineLock2[machineNo]);

        if (t_count >= num_customers) {
            pthread_mutex_lock(&writeLock);
            kevin += values[0];
            bob += values[1];
            stuart += values[2];
            otto += values[3];
            dave += values[4];
            pthread_mutex_unlock(&writeLock);
            break;
        }

        vector<string> v;
        stringstream my_str(line);
        
        while(my_str.good())
        {
            string substr;
            getline(my_str, substr, ',');
            v.push_back(substr);
        }

        int amount = stoi(v[3]);
        string name = v[2];

        if (name == "Kevin")
            kevin += amount;
        else if (name == "Bob")
            bob += amount;
        else if (name == "Stuart")
            stuart += amount;
        else if (name == "Otto")
            otto += amount;
        else if (name == "Dave")
            dave += amount; ;

        string log = v[4] + "," + v[3] + "TL" + "," + v[2];
        
        pthread_mutex_lock(&writeLock);
        fileOut << log << endl;
        t_count++;
        pthread_mutex_unlock(&writeLock);
        
        if (t_count == num_customers) {
            for (int i = 1; i < 11; i++){
                pthread_mutex_unlock(&machineLock[i]);
            }
        }
        
    }
    pthread_exit(0);
}