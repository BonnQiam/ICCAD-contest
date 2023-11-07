#ifndef LAYER_H
#define LAYER_H
#include <iostream>
#include <vector>
#include <iomanip>
#include "polygon.h"
#include "window.h"

using namespace std;


class Layer{
public:
    Layer(int i,int l,int r, int b, int t,int win){
        left = l; right = r; bottom = b; top = t; 
        id = i; width = right - left; hight = top - bottom; block_size = win/2; //block is a 1/4 window for checking density
        w_block_num = width/block_size;
        h_block_num = hight/block_size;
        total_block_num = w_block_num * h_block_num;
        blocks = new vector<Polygon* > [total_block_num];
        
    }
    ~Layer(){
        delete [] blocks;
    }
    void set_property(int x1, int x2, int x3, double x4, double x5){
        w_min = x1; space_min = x2; w_max = x3;
        density_min = x4; density_max = x5; 
    }
    // Determine what blocks the polygon belongs to

    void insert_polygon(Polygon* poly){ // Need to find a clever way to store the nets
        
        int win_left_id = poly -> get_left()/ block_size;
        int win_right_id = poly -> get_right() / block_size; 
        if(poly -> get_right() % block_size == 0) win_right_id -= 1;
        int win_bottom_id = poly -> get_bottom()/block_size;
        int win_top_id = poly -> get_top()/block_size;
        if(poly -> get_top() % block_size == 0) win_top_id -= 1;
            
        for(int w = win_left_id; w <= win_right_id; w++){
            for(int h = win_bottom_id; h <= win_top_id; h++){
                
                blocks[w_block_num*h + w].push_back(poly);
            }
        }
        
    }
    
    void insert_fill(int& count,ofstream& file){
        
        cout<<endl;
        for(int j = 0; j < h_block_num; j++){
            for(int i = 0; i < w_block_num; i++){ 
                
                vector<Polygon*>& polygons = blocks[ j* w_block_num + i ];
                bool fill_complete = false; 
                int unit_size = 10; // -> Important value !!

                while(fill_complete == false){    
                    Window* window = new Window(left,bottom,i*block_size, j*block_size, block_size, unit_size);
                    window -> set_property(w_min, space_min, w_max, density_min, density_max);

                    for(int k = 0, size = polygons.size(); k<size; k++){

                        window -> mark_occupied( polygons[k] -> get_left(), polygons[k] ->get_right()
                                                ,polygons[k] -> get_bottom(), polygons[k] -> get_top());
                    }
                    fill_complete = window -> insert_fill(count, id);

                    if(fill_complete == true)window  -> write_file(file);
                    else if(unit_size == 20)unit_size = 1;
                    else { 
                        window -> write_file(file);
                        fill_complete = true;
                    }

                    delete window;
                }
                
                for(int x = 0; x<34; x++)cout<<"\b";
                cout<<"Layer "<<id<<" finished "<<setw(7)<<j*w_block_num +i +1<<"  /"<<setw(7)<<total_block_num<<flush;
            }
        }
    }

private:
    int left,right,bottom,top;
    int id,width, hight;
    int block_size, w_block_num, h_block_num, total_block_num; //total_block_num = 12960
    int w_min, space_min, w_max;
    double density_min,density_max;
    vector<Polygon*>* blocks;
 
};

#endif