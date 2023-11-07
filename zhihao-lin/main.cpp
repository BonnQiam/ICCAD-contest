#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "util.h"
#include "chip.h"
#include "layer.h"
#include "polygon.h"
#include <sstream>

using namespace std;

void test(){
    
}

int main(int argc, char* argv[]){
    
    if(argc != 2){                                      
        cout<<"Error: Wrong number of input! (1 configure file)"<<endl; 
        return 0;
    }
    Chip chip;
    if(!chip.read_config(argv[1]))return 0;
    chip.write_fill();
    
}

