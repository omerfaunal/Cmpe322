#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <chrono>
#include <fstream>

int* arr;
int* mainArr;

double median(int n);
double interquartileRange(int n);

void* mode(void* arg);
void* sum(void* arg);
void* arithmeticMean(void* arg);
void* harmonicMean(void* arg);
void* standardDeviation(void* arg);



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
        file << arr[i] << std::endl;
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
    mainArr = (int *)malloc(n*sizeof(int));
    double out[11];
    generateRandomNInteger(n, 1000, 10000);

    auto start = high_resolution_clock::now();

    pthread_t t[5];

    if(pthread_create(&t[0], NULL, mode, &n)) 
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[1], NULL, sum, &n)) 
        fprintf(stderr, "Error creating thread");

    if(pthread_create(&t[2], NULL, arithmeticMean, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[3], NULL, harmonicMean, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[4], NULL, standardDeviation, &n))
        fprintf(stderr, "Error creating thread");

    std::copy(arr, arr + n, mainArr);
    std::sort(mainArr, mainArr + n);

    out[0] = mainArr[0]; //Min
    out[1] = mainArr[n-1]; //Max
    out[2] = out[1] - out[0]; //Range
    out[4] = median(n); //Median
    out[9] = interquartileRange(n);
    double threadOut[5];
    for(int i = 0; i < 5; i++) {
        double* r;
        if(pthread_join(t[i],(void**) &r)) {
            fprintf(stderr, "Error joining thread");
        }
        threadOut[i] = *r;
    }

    out[3] = threadOut[0]; //Mode
    out[5] = threadOut[1]; //Sum
    out[6] = threadOut[2]; //Arithmetic Mean 
    out[7] = threadOut[3]; //Harmonic Mean
    out[8] = threadOut[4]; //Standard Deviation/Interquartile Range

    auto end = high_resolution_clock::now();
    auto ms_int = duration_cast<microseconds>(end - start);
    out[10] = ms_int.count() / 1000000.0;

    writeToFile(out, 11, "output3.txt");
    
    return 0;
}

double median(int n) {
    if(n % 2 == 0) {
        return (mainArr[n / 2 - 1] + mainArr[n / 2]) / 2.0;
    } else {
        return mainArr[n / 2];
    }
}

double interquartileRange(int n) {
    int q1, q3;
    if(n % 2 == 0) {
        q1 = (mainArr[n / 4 - 1] + mainArr[n / 4]) / 2.0;
        q3 = (mainArr[n * 3 / 4 - 1] + mainArr[n * 3 / 4]) / 2.0;
    } else {
        q1 = mainArr[n / 4];
        q3 = mainArr[n * 3 / 4];
    }
    return q3 - q1;
}

void* mode(void* arg) {
    int n = *(int*)arg;
    int number = arr[0];
    int mode = number;
    int count = 1;
    int countMode = 1;

    for (int i=1; i<n; i++)
    {
        if (arr[i] == number) 
        {
            count++;
        }
        else
        {
            if (count > countMode) 
            {
                countMode = count;
                mode = number;
            }
            count = 1;
            number = arr[i];
        }
        if (count > countMode) { countMode = count; mode = number;}
    }
    double* result = (double* ) malloc(sizeof(double));
    *result = mode;
    pthread_exit((void*) result);
}

void* sum(void* arg) {
    int n = *(int*)arg;
    long int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }
    double* result = (double* ) malloc(sizeof(double));
    *result = sum;
    pthread_exit((void*) result);
}

void* harmonicMean(void* arg) {
    int n = *(int*)arg;
    double sum = 0;
    for(int i = 0; i < n; i++) {
        sum += 1.0 / arr[i];
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = n/sum;
    pthread_exit((void*) result);
}

void* standardDeviation(void* arg) {
    int n = *(int*)arg;
    double sum = 0;
    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }
    double mean = sum / n;
    double ssum = 0;
    for(int i = 0; i < n; i++) {
        ssum += (arr[i] - mean) * (arr[i] - mean);
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = sqrt(ssum / (n-1));
    pthread_exit((void*) result);
}

void* arithmeticMean(void* arg) {
    int n = *(int*)arg;
    double sum = 0;
    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = sum/n;
    pthread_exit((void*) result);
}

