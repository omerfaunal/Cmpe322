#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <chrono>
#include <fstream>

int* arr;
int* arr2;
int* arr3;

void* min(void* arg);
void* max(void* arg);
void* range(void* arg);
void* mode(void* arg);
void* median(void* arg);
void* sum(void* arg);
void* arithmeticMean(void* arg);
void* harmonicMean(void* arg);
void* standardDeviation(void* arg);
void* interquartileRange(void* arg);

int* generateRandomNInteger(int n, int min, int max) {
    //This function generates n random integers between min and max and returns them in an array
    //srand((unsigned int)(time(NULL)));
    srand((unsigned int)10);
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
    arr2 = (int *)malloc(n*sizeof(int));
    arr3 = (int *)malloc(n*sizeof(int));
    double out[11];
    generateRandomNInteger(n, 1000, 10000);

    auto start = high_resolution_clock::now();
    pthread_t t[10];

    if(pthread_create(&t[0], NULL, min, &n)) 
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[1], NULL, max, &n)) 
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[2], NULL, range, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[3], NULL, mode, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[4], NULL, median, &n))
        fprintf(stderr, "Error creating thread");

    if(pthread_create(&t[5], NULL, sum, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[6], NULL, arithmeticMean, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[7], NULL, harmonicMean, &n))
        fprintf(stderr, "Error creating thread");

    if(pthread_create(&t[8], NULL, standardDeviation, &n))
        fprintf(stderr, "Error creating thread");
    
    if(pthread_create(&t[9], NULL, interquartileRange, &n))
        fprintf(stderr, "Error creating thread");

    
    for(int i = 0; i < 10; i++) {
        double* r;
        if(pthread_join(t[i],(void**) &r)) {
            fprintf(stderr, "Error joining thread");
        }
        out[i] = *r;
    }

    auto end = high_resolution_clock::now();
    auto ms_int = duration_cast<microseconds>(end - start);
    out[10] = ms_int.count() / 1000000.0;

    writeToFile(out, 11, "output3.txt");
    
    return 0;
}

void* min(void* arg) {
    int n = *(int*)arg;
    int min = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = min;
    pthread_exit((void*) result);
}

void* max(void* arg) {
    int n = *(int*)arg;
    int max = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = max;
    pthread_exit((void*) result);
}

void* range(void* arg) {
    int n = *(int*)arg;
    int max = arr[0];
    int min = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
        else if(arr[i] < min) {
            min = arr[i];
        }
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = max - min;
    pthread_exit((void*) result);
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

void* median(void* arg) {
    int n = *(int*)arg;
    double median;
    std::copy(arr, arr + n, arr2);
    std::sort(arr2, arr2 + n);
    if(n % 2 == 0) {
        median = (arr2[n / 2 - 1] + arr2[n / 2]) / 2.0;
    } else {
        median = arr2[n / 2];
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = median;
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

void* interquartileRange(void* arg) {
    int n = *(int*)arg;
    std::copy(arr, arr + n, arr3);
    std::sort(arr3, arr3 + n);
    int q1, q3;
    if(n % 2 == 0) {
        q1 = (arr3[n / 4 - 1] + arr3[n / 4]) / 2.0;
        q3 = (arr3[n * 3 / 4 - 1] + arr3[n * 3 / 4]) / 2.0;
    } else {
        q1 = arr3[n / 4];
        q3 = arr3[n * 3 / 4];
    }
    double* result =(double* ) malloc(sizeof(double));
    *result = q3 - q1;
    pthread_exit((void*) result);
}
