#ifndef Grid_hpp
#define Grid_hpp

#include "Polygon.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/*******************************************************************
 * Gird Definition
*/
template <typename T>
struct Grid
{
    Coor<T> BasePoint;
    double density;
    double size;

    std::vector< Rectangle<T> > Rectangles;

    void calculate_density(){
        for(auto rect = Rectangles.begin(); rect != Rectangles.end(); rect++){
            density += rect->Area()/(size*size);
        }
    }

    void output_file(string filename){
        // open filename in std::ios::app
        std::ofstream file;
        file.open(filename, std::ios::app);

        file << "<grid>" << std::endl;
        file << density << std::endl;
        for(auto rect = Rectangles.begin(); rect != Rectangles.end(); rect++){
            file << rect->TopLeft << "," << rect->BottomRight << std::endl;
        }
        file << "</grid>" << std::endl;

        file.close();
    }
};

#endif