#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <algorithm>

int* arr;
int* arr2;
double out[11];

void* minMax(void* arg);
void* medianIqr(void* arg);
void* modeRange(void* arg);
void* sumMean(void* arg);
void* stdHmean(void* arg);

int* generateRandomNInteger(int n, int min, int max) {
    //This function generates n random integers between min and max and returns them in an array
    srand((unsigned int)(time(NULL)));
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % (max - min + 1) + min;
    }
    return arr;
}

void writeToFile(double* arr, int n, std::string fileName) {
    std::ofstream file;
    file.open(fileName);
    for(int i = 0; i < n; i++) {
        file << std::fixed << std::setprecision(5)<< arr[i] << std::endl;
    }
    file.close();
}

int main(int argc, char const *argv[])
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::microseconds;
    long int n = atoi(argv[1]);
    arr = (int *)malloc(n*sizeof(int));
    arr2 = (int *)malloc(n*sizeof(int));
    generateRandomNInteger(n, 1000, 10000);

    auto start = high_resolution_clock::now();

    pthread_t t[5];

    if(pthread_create(&t[0], NULL, minMax, &n)) 
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[1], NULL, medianIqr, &n)) 
        fprintf(stderr, "Error creating thread");

    if(pthread_create(&t[2], NULL, modeRange, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[3], NULL, sumMean, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[4], NULL, stdHmean, &n))
        fprintf(stderr, "Error creating thread");


    double threadOut[5];
    for(int i = 0; i < 5; i++) {
        if(pthread_join(t[i], NULL)) {
            fprintf(stderr, "Error joining thread");
        }
    }

    auto end = high_resolution_clock::now();
    auto ms_int = duration_cast<microseconds>(end - start);
    out[10] = ms_int.count() / 1000000.0;

    writeToFile(out, 11, "output2.txt");
    
    return 0;
}

void* minMax(void* arg) {
    int n = *(int*)arg;
    int min = arr[0];
    int max = arr[0];
    for(int i = 0; i < n; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
        else if(arr[i] > max) {
            max = arr[i];
        }
    }
    out[0] = min;
    out[1] = max;
    pthread_exit(NULL);
}

void* medianIqr(void* arg) {
    int n = *(int*)arg;
    double median;
    std::copy(arr, arr + n, arr2);
    std::sort(arr2, arr2 + n);
    if(n % 2 == 0) {
        median = (arr2[n / 2 - 1] + arr2[n / 2]) / 2.0;
    } else {
        median = arr2[n / 2];
    }
    out[4] = median;
    int q1, q3;
    if(n % 2 == 0) {
        q1 = (arr2[n / 4 - 1] + arr2[n / 4]) / 2.0;
        q3 = (arr2[n * 3 / 4 - 1] + arr2[n * 3 / 4]) / 2.0;
    } else {
        q1 = arr2[n / 4];
        q3 = arr2[n * 3 / 4];
    }
    out[9] = q3 - q1;
    pthread_exit(NULL);
}

void* modeRange(void* arg) {
    int n = *(int*)arg;

    int min = arr[0];
    int max = arr[0];

    int* count = (int*)calloc(10000, sizeof(int));

    for (int i=0; i < n; i++) {
        count[arr[i]]++;

        if(arr[i] < min) {
            min = arr[i];
        }
        else if(arr[i] > max) {
            max = arr[i];
        }
    }

    int index = 9999;
    for (int i=9998; i >=0; i--) {
        if (count[i] >= count[index])
            index = i;
    }

    
    out[3] = index;
    out[2] = max - min;
    pthread_exit(NULL);
}

void* sumMean(void* arg) {
    int n = *(int*)arg;
    long int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }
    double mean = sum / (double)n;
    out[5] = sum;
    out[6] = mean;
    pthread_exit(NULL);
}

void* stdHmean(void* arg) {
    int n = *(int*)arg;
    double sum = 0;
    double hsum = 0;
    for(int i = 0; i < n; i++) {
        sum += arr[i];
        hsum += 1.0 / arr[i];
    }
    double hmean = n / hsum;
    double mean = sum / n;
    double ssum = 0;
    for(int i = 0; i < n; i++) {
        ssum += (arr[i] - mean) * (arr[i] - mean);
    }
    double std = sqrt(ssum / (n-1));
    out[7] = hmean;
    out[8] = std;
    pthread_exit(NULL);
}


