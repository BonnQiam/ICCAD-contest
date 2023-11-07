#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <vector>
#include <sstream>
#include "polygon.h"


class Window{
public:
    Window(int left,int bottom,int x,int y, int s, int u){
        layer_left = left; layer_bottom = bottom;
        o_x = x; o_y = y; window_size = s;
        unit_size = u;
        
        unit_num = window_size/unit_size;

        o = new bool* [unit_num];
        for(int i=0; i<unit_num ; i++)o[i] = new bool [ unit_num ];
        for(int i=0; i<unit_num ; i++){
            for(int j=0; j<unit_num; j++)o[i][j] = true;
        }
    }
    ~Window(){
        for(int i = 0; i<unit_num; i++)delete [] o[i];
        delete [] o;
    }

    void set_property(int w_min, int space_min, int w_max, double x4, double x5){
        unit_wid_min = (w_min % unit_size == 0)? (w_min/unit_size) : (w_min/unit_size +1);
        unit_wid_max = w_max/unit_size;
        unit_space_min = (space_min % unit_size == 0)? (space_min/unit_size) : (space_min/unit_size +1);
        density_min = x4+0.1; //To supplement some windows which can't reach minimum density
        density_max = x5;
        double space_temp = (double)window_size * (double)window_size * density_min;
        space_limit = (int)space_temp;
    }

    bool* operator [](int x){ return o[x];}

    void mark_occupied(int left, int right, int bottom , int top){  
        //input is normalized to chip boundary, now need to be normalized to the window 
        left = ( left - o_x > 0)? (left - o_x) : 0;
        right = (right - o_x < window_size)? (right - o_x) : window_size;
        bottom = ( bottom - o_y > 0 )? (bottom - o_y ) : 0;
        top = ( top - o_y  < window_size)? (top - o_y ) : window_size;
        space_limit -= (top - bottom) * ( right - left);

        int u_left, u_right, u_bottom, u_top;
        u_left = left/unit_size ;
        u_right= (right % unit_size == 0)? (right/unit_size ) : (right/unit_size +1);
        u_bottom= bottom/unit_size;
        u_top = (top % unit_size == 0)? (top/unit_size ) : (top/unit_size +1);

        for(int i = u_left; i < u_right; i++){
            for(int j = u_bottom; j < u_top; j++)o[i][j] = false;
        }
    }

    bool insert_fill(int& count, int layer_id){
        
        int left, right, bottom, top;
        
        for(int i = unit_space_min; i <= (unit_num-unit_space_min-unit_wid_min) ; i++){
            for(int j =unit_space_min; j <= (unit_num-unit_space_min-unit_wid_min); j++){
                
                if(space_limit < 0)return true;
                if(!valid_starting_unit(i,j))continue;
                
                left = i; right = i+ unit_wid_min;
                bottom = j; top = j+ unit_wid_min;
                
                // grow fill horizontally
                while( (right-left < unit_wid_max) && (right + unit_space_min < unit_num)){
                    if(valid(right+unit_space_min, right+unit_space_min+1, bottom, top))right += 1;
                    else break;
                }
                // grow fill vertically
                while( (top-bottom < unit_wid_max) && (top + unit_space_min < unit_num)){
                    if(valid(left, right, top+unit_space_min, top+unit_space_min+1))top += 1;
                    else break;
                }

                for(int x = left; x < right; x++){
                    for(int y = bottom; y<top; y++)o[x][y] = false;
                }
                space_limit -= (right - left)*(top - bottom)*unit_size*unit_size;

                stringstream ss;
                ss<<count<<" "<<layer_left+o_x+left*unit_size<<" "<<layer_bottom+o_y+bottom*unit_size
                    <<" "<<layer_left+o_x+right*unit_size<<" "<<layer_bottom+o_y+top*unit_size<<" 0 "<<layer_id<<" Fill";
                fills.push_back(ss.str());
                count ++;
                
            }
        }
        return false;
    }
    void write_file(ofstream& file){
        for(int i=0,fill_num = fills.size(); i<fill_num; i++){
            file<<fills[i]<<endl;
        }
    }
    double get_limit_left(){
        return space_limit/(double)(unit_size*unit_num*unit_size*unit_num);
    }

private:
    bool**  o;        //bottom left point of window o[i][j] means the unit in row i and colomn j
    int layer_left, layer_bottom;
    int o_x, o_y, window_size;
    int w_min, space_min, w_max;
    int unit_size, unit_num;    //length of each grid in the window, number of unit along the window edge
    int unit_wid_min, unit_space_min, unit_wid_max;

    double density_min, density_max;
    int space_limit;//  blocks' number we should insert at least
    vector<string> fills;

    bool valid_starting_unit(int x , int y){
        //determine whether o[x][y] is a valid starting point for a fill
        if(o[x][y] == false)return false;
        for(int i = x - unit_space_min; i < x+unit_wid_min+unit_space_min; i++){
            for(int j = y - unit_space_min; j < y+unit_wid_min+unit_space_min; j++){
                if(o[i][j] == false)return false;
            }
        }return true;
    }

    bool valid(int left, int right, int bottom, int top){
        //determine whether a region bounded by left,right,top,bottom is valid for insertion
        for(int i = left; i < right; i++){
            for(int j = bottom; j < top; j++){
                if(o[i][j] == false)return false;
            }
        }return true;
    }
};

#endif
