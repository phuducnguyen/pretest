#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

struct Flight {
    string src; // source 
    string des; // destination
    string airliner; // name of airliner
    int eco; // economy seats
    int bus; // business seats
    int hours; 
    int mins;
};


void todo(string flightdata_filename, string data_filename);
void main_debug();


int main(int argc, char **argv){
    if (argc == 1) {
        main_debug();
    }
    else {
        int idx_argv = 1;
        string flightdata_filename = argv[idx_argv++];
        string data_filename = argv[idx_argv++];
        todo(flightdata_filename, data_filename);
    }
    cout << "\n";
    return 0;
}