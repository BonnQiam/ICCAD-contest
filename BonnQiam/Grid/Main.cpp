#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>

#include <vector>
#include <iterator>
#include <set>

#include <limbo/parsers/gdsii/gdsdb/GdsIO.h>
#include <limbo/preprocessor/Msg.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/adapted/boost_polygon.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include "Decomposition.hpp"
#include "Grid.hpp"

typedef std::vector<GdsParser::GdsDB::GdsCell>                                                  Structure_s;
typedef std::vector<std::pair<GdsParser::GdsRecords::EnumType, GdsParser::GdsDB::GdsObject*> >  Element_s;

typedef GdsParser::GdsDB::GdsShape::coordinate_type                                             Coordinate_type;
typedef std::vector< boost::polygon::point_data<GdsParser::GdsDB::GdsShape::coordinate_type> >  Coordinate_s;
typedef std::vector<GdsParser::GdsDB::GdsPolygon*>                                              Ploygons_s;

//contest configure: the window size is 20 microns, the gdsii unit si 1 nm, i.e., 0.001 micron
#define x_grid_num 8
#define y_grid_num 20
#define grid_size  20000

void GDSII_Decomposition(char* gdsii_file);

int main(int argc, char** argv)
{   
    /**********************************
     * 
     * Read GDSII, decompose the polygon into rectangles, write the rectangles to file
     * 
     * *******************************/
#if 0
    char* gdsii_file = argv[1];
    GDSII_Decomposition(gdsii_file);
#endif

#if 1
    /**********************************
     * 
     * Read the rectangles from file, establish the grid system, write the grid to file
     * 
     * *******************************/

    std::vector < Rectangle<int> > rectangles;

    // read from rectangle.txt
    //std::ifstream file("rectangle_test.txt");
    std::ifstream file("rectangle.txt");

    std::regex r("\\((\\d+), (\\d+)\\)");
    std::smatch m;
    
    std::string line;
    while(std::getline(file, line)){
        std::regex_iterator<std::string::iterator> rit(line.begin(), line.end(), r);
        std::regex_iterator<std::string::iterator> rend;
        while (rit != rend) {
            Coor<int> Topleft(std::stoi((*rit)[1]), std::stoi((*rit)[2]));
            ++rit;
            Coor<int> Bottomright(std::stoi((*rit)[1]), std::stoi((*rit)[2]));
            ++rit;

            Rectangle<int> rect(Topleft, Bottomright);
            rectangles.push_back(rect);
        }
    }

    std::cout << "The number of rectangles is " << rectangles.size() << std::endl;

    // establish the grid system
    string grid_file = "grid.txt";
    //clear the file
    std::ofstream ofs;
    ofs.open(grid_file, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    std::vector< Grid<int> > grids;
    for(int i=0; i<x_grid_num; i++){
        for(int j=0; j<y_grid_num; j++){
            Coor<int> BasePoint(i*grid_size, j*grid_size);
            Grid<int> grid;
            grid.BasePoint = BasePoint;
            grid.size = grid_size;
            grid.Rectangles.clear();
            grids.push_back(grid);
        }
    }
    for(auto grid = grids.begin(); grid != grids.end(); grid++){
    //for(auto grid = grids.begin(); grid != grids.begin()+2; grid++){
        Coor<int> grid_Top_left = grid->BasePoint + Coor<int>(0, grid_size);
        Coor<int> grid_Bottom_right = grid->BasePoint + Coor<int>(grid_size, 0);
        Rectangle<int> grid_rect(grid_Top_left, grid_Bottom_right);

        for(auto rect = rectangles.begin(); rect != rectangles.end(); rect++){
            Rectangle<int> grid_intersection = Rectangle_intersection(grid_rect, *rect);
            // check the grid_intersection is not Rectangle<T>(Coor<T>(0, 0), Coor<T>(0, 0))
            if(grid_intersection.TopLeft != grid_intersection.BottomRight){
                grid->Rectangles.push_back(grid_intersection);
            }
        }

        grid->calculate_density();
        grid->output_file(grid_file);
        std::cout << "The grid at " << grid->BasePoint << " has " << grid->Rectangles.size() << " rectangles" << std::endl;
    }
#endif

    return 0;
}

void GDSII_Decomposition(char* gdsii_file){
    GdsParser::GdsDB::GdsDB db;
    GdsParser::GdsDB::GdsReader reader (db);
    //limboAssert(reader(argv[1]));
    limboAssert(reader(gdsii_file));

#if 0
    // read the gdsii and get unit
    double unit = db.unit();

    std::cout << "unit: " << unit << std::endl;

#endif

#if 1
    Ploygons_s ploygon_s;
    // read the gdsii and get ploygon vector
    for (Structure_s::const_iterator str = db.cells().begin(); str != db.cells().end(); ++str){

        std::cout << "cell: " << str->name() << std::endl;

        for(Element_s::const_iterator ele = str->objects().begin(); ele != str->objects().end(); ++ele){
            if(ele->first == GdsParser::GdsRecords::BOUNDARY){
                ploygon_s.push_back(dynamic_cast<GdsParser::GdsDB::GdsPolygon*>(ele->second));
            }
        }
    }
#endif

#if 0
    // find the min/max coordinate
    Coordinate_type min_x = 0;
    Coordinate_type min_y = 0;
    Coordinate_type max_x = 0;
    Coordinate_type max_y = 0;

    for(Ploygons_s::const_iterator poly = ploygon_s.begin(); poly != ploygon_s.end(); poly++){
        for(Coordinate_s::const_iterator coor = (*poly)->begin(); coor != (*poly)->end(); coor++){
            if(coor->x() < min_x) min_x = coor->x();
            if(coor->y() < min_y) min_y = coor->y();
            if(coor->x() > max_x) max_x = coor->x();
            if(coor->y() > max_y) max_y = coor->y();
        }
    }

    std::cout << "min_x: " << min_x << std::endl;
    std::cout << "min_y: " << min_y << std::endl;
    std::cout << "max_x: " << max_x << std::endl;
    std::cout << "max_y: " << max_y << std::endl;
    
#endif

#if 1
    // decompose the ploygon
    int length = ploygon_s.size();

    std::cout << "length: " << length << std::endl;

    for(int i=0; i < length; i++){
        std::cout << "Test " << i << "-th polygon" << std::endl;

        GdsParser::GdsDB::GdsPolygon* test = ploygon_s[i];

        Polygon<int> poly;

        for(Coordinate_s::const_iterator coor = test->begin(); coor != test->end(); coor++){
            poly.vertexes.push_back(Coor<int>(coor->x(), coor->y()));
        }

        poly.edges_init();

        std::vector< Polygon<int> > result;
        Decomposition(poly, result);
        std::cout << i << "-th polygon is decomposed successfully" << std::endl;

        //std::vector < Rectangle<int> > rectangle_result;
        std::ofstream file("rectangle.txt", std::ios::app);
        for(auto poly: result){
            Coor<int> Topleft = poly.vertexes[3];
            Coor<int> Bottomright = poly.vertexes[1];

            // output Topleft and Bottomright to file
            file << Topleft << "," << Bottomright << std::endl;
        }
        file.close();
    }
#endif
}