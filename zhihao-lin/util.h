#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// Convert string into integer
int string2int(string s){  
    int len = s.size();
    int integer  = 0;
    for(int i = 0;i <len; i++){
        if     (s[i] == '0')integer = integer*10;
        else if(s[i] == '1')integer = integer*10 + 1;
        else if(s[i] == '2')integer = integer*10 + 2;
        else if(s[i] == '3')integer = integer*10 + 3;
        else if(s[i] == '4')integer = integer*10 + 4;
        else if(s[i] == '5')integer = integer*10 + 5;
        else if(s[i] == '6')integer = integer*10 + 6;
        else if(s[i] == '7')integer = integer*10 + 7;
        else if(s[i] == '8')integer = integer*10 + 8;
        else if(s[i] == '9')integer = integer*10 + 9;
    }
    return integer;
}

//Convert string to double
double string2double( string s){
    int I = string2int(s);
    double d = (double) I;

    for(int pos = s.size()-1, count = 0; pos >= 0; pos --, count++){
        if(s[pos] == '.'){
            d = d * pow(0.1,count);
            break;
        }
    }return d;
}

//parse variabels from a string
vector<string> parse_string(string str){  
    vector<string> vec;
    int start = 0, end = 0;
    int len = str.size();
    while(true){
        start = str.find_first_not_of(' ',end);
        if(start == -1)break;
        end = str.find_first_of(' ',start);
        if(end == -1) end = len;
        vec.push_back( str.substr(start, end-start));
    }
    return vec;
}

// Transofrm string vector into int vector
vector<int> get_int_vector(vector<string> strings){  
    vector<int> integers;
    for(int i =0,size = strings.size(); i<size; i++){
        integers.push_back(string2int(strings[i]));
    }return integers;
}

//Check if target is in a int vector
bool is_member(int target, vector<int>& ints){
    for(int i = 0,size = ints.size(); i<size; i++){
        if(ints[i]  == target)return true;
    }return false;
}

#endif