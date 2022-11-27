#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <algorithm>

int* arr;
//int arr[] = {1, 2, 5, 6, 7, 9, 12, 15, 18, 19, 27};

int min(int n);
int max(int n);
int mode(int n);
double median(int n);
long int sum(int n);
double harmonicMean(int n);
double standardDeviation(int n, double mean);
double interquartileRange(int n);


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
    double out[11];
    generateRandomNInteger(n, 1000, 10000);

    auto start = high_resolution_clock::now();

    out[3] = mode(n);
    std::sort(arr, arr + n);
    out[0] = arr[0];
    out[1] = arr[n-1];
    out[2] = out[1] - out[0];
    out[4] = median(n);
    out[5] = sum(n);
    out[6] = out[5] / n;
    out[7] = harmonicMean(n);
    out[8] = standardDeviation(n, out[6]);
    out[9] = interquartileRange(n);

    auto end = high_resolution_clock::now();
    auto ms_int = duration_cast<microseconds>(end - start);
    out[10] = ms_int.count() / 1000000.0;

    writeToFile(out, 11, "output1.txt");
    
    return 0;
}

int mode(int n) {
    int* count = (int*)calloc(10000, sizeof(int));

    for (int i=0; i < n; i++) {
        count[arr[i]]++;
    }

    int index = 9999;
    for (int i=9998; i >=0; i--) {
        if (count[i] >= count[index])
            index = i;
    }

    return index;
}

double median(int n) {
    if(n % 2 == 0) {
        return (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
    } else {
        return arr[n / 2];
    }
}

long int sum(int n) {
    long int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

double harmonicMean(int n) {
    double sum = 0;
    for(int i = 0; i < n; i++) {
        sum += 1.0 / arr[i];
    }
    return n / sum;
}

double standardDeviation(int n, double mean) {
    double sum = 0;
    for(int i = 0; i < n; i++) {
        sum += (arr[i] - mean) * (arr[i] - mean);
    }
    return sqrt(sum / (n-1));
}

double interquartileRange(int n) {
    int q1, q3;
    if(n % 2 == 0) {
        q1 = (arr[n / 4 - 1] + arr[n / 4]) / 2.0;
        q3 = (arr[n * 3 / 4 - 1] + arr[n * 3 / 4]) / 2.0;
    } else {
        q1 = arr[n / 4];
        q3 = arr[n * 3 / 4];
    }
    return q3 - q1;
}
