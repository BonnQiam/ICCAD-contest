#ifndef CHIP_H
#define CHIP_H
#include <iostream>
#include <fstream>
#include <vector>
#include "util.h"
#include "layer.h"
#include "polygon.h"

using namespace std;

class Chip{
public:
    Chip(){}
    ~Chip(){
        for(int i = 0, layer_num = layers.size(); i< layer_num; i++)delete layers[i];
    }
    bool read_config(string config_name ){ // read .config
        ifstream config;
        config.open(&config_name[0]);
        if(! config.is_open()){ cout<< "Error: Config File can't be opened!"<<endl; return false;}
        string line, design_name, rule_name, process_name;
        vector<string> lines;
        vector<string> strings;
        while(getline(config,line))lines.push_back(line);
        // set file path
        strings = parse_string(lines[0]); design_name = strings[1];
        strings = parse_string(lines[1]); output_name = strings[1];
        strings = parse_string(lines[2]); rule_name = strings[1];
        strings = parse_string(lines[3]); process_name = strings[1];
        
        //parse process file: .dat
        if(! read_process(process_name ))return false;

        //parse rule file: .dat
        if(! read_design( design_name, true))return false;
        if(! read_rule( rule_name ))return false;
        
        //parse design: .cut
        if( !read_design( design_name, false))return false;
        
        // set critical net IDs
        strings = parse_string(lines[4]);
        strings.erase(strings.begin());
        critical_IDs = get_int_vector(strings);

        // set power_net & ground_net IDs
        strings = parse_string(lines[5]); power_ID = string2int(strings[1]);
        strings = parse_string(lines[6]); ground_ID= string2int(strings[1]); 
        return true;
    }

    void set_boundary(int bl_x, int bl_y, int tr_x, int tr_y){
        left = bl_x; right = tr_x; bottom = bl_y; top = tr_y;  
    }
    
    // read rule.dat 
    bool read_rule( string rule_name){ 
        ifstream rule_file;
        rule_file.open( &rule_name[0] );
        if(!rule_file.is_open()){ cout<<"Error: Can't open rule file "<<rule_name<<endl; return false;}
        string line;
        vector<string> strings;
        vector<int>    ints;
        while(getline(rule_file, line)){
            strings = parse_string(line);
            if(strings[1] != "CONDUCTOR")continue;
            strings.erase(strings.begin()+1);
            
            Layer* lptr = new Layer(string2int(strings[0]), left, right, bottom, top, window_size);
            lptr -> set_property(string2int(strings[1]),string2int(strings[2]),string2int(strings[3])
                                ,string2double(strings[4]),string2double(strings[5]));
            
            layers.push_back(lptr);
        }return true;
    }

    //read circuit.cut
    bool read_design(string  design_name, bool initialize ){ 
        ifstream design_file;
        design_file.open( &design_name[0]);
        if(!design_file.is_open()){ cout<<"Error: Can't open design file "<<design_name<<endl; return false;}
        string line;
        vector<string> strings;
        vector<int>    ints;
        getline(design_file,line);

        if(initialize == true){
            strings = parse_string(line);
            strings.pop_back();
            ints = get_int_vector(strings);
            set_boundary(ints[0], ints[1] ,ints[2] ,ints[3] );
            return true;
        }
        
        while(getline(design_file,line) ){

            strings = parse_string(line);
            if(strings.back() != "normal")continue;
            strings.pop_back();
            ints = get_int_vector(strings);
            Polygon* pptr = 0;

            //coordinates of each polygon is normalized to the chip 
            int poly_bl_x = ints[1] - left;
            int poly_bl_y = ints[2] - bottom;
            int poly_tr_x = ints[3] - left;
            int poly_tr_y = ints[4] - bottom;
            int id = ints[5];

            if(id == power_ID || id == ground_ID){
                pptr = new PowerGround(poly_bl_x,poly_bl_y,poly_tr_x,poly_tr_y,id);
            }else if( is_member(id , critical_IDs)){
                pptr = new Critical(poly_bl_x,poly_bl_y,poly_tr_x,poly_tr_y,id);
            }else pptr = new NonCritic(poly_bl_x,poly_bl_y,poly_tr_x,poly_tr_y,id);

            layers[ints[6]-1] -> insert_polygon(pptr);
        }
        return true; 
    }
    bool read_process(string  process_name){
        
        ifstream process_file;
        process_file.open( &process_name[0]);
        if( ! process_file.is_open() ){cout<<"Error: Can't open "<<process_name<<endl; return false; }
        //
        string line;
        vector<string> strings;
        getline(process_file, line);
        getline(process_file, line);
        strings = parse_string(line);
        window_size = string2int(strings[1]);
        return true;
        
    }

    void write_fill(){
        ofstream Fill_file;
        Fill_file.open( &output_name[0]);
        int count = 1;

        for(int i =0, layer_num = layers.size(); i < layer_num; i++){//
            layers[i] ->insert_fill( count, Fill_file);
        }
        
        Fill_file.close();
        cout<<"\nFinished Fill-Insertion !!"<<endl;
    }
    void test(){
        cout<<"Window size: "<<window_size<<endl;
    }
private:
    string output_name;
    vector<int> critical_IDs;
    int power_ID, ground_ID;
    int window_size;
    int left,right,top,bottom;

    vector<Layer*> layers;
};


#endif