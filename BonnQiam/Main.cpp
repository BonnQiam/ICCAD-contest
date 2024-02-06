#include <iostream>
#include <vector>
#include <iterator>
#include <set>

#include <limbo/parsers/gdsii/gdsdb/GdsIO.h>
#include <limbo/preprocessor/Msg.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/adapted/boost_polygon.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include "PTR.hpp"

typedef std::vector<GdsParser::GdsDB::GdsCell>                                                  Structure_s;
typedef std::vector<std::pair<GdsParser::GdsRecords::EnumType, GdsParser::GdsDB::GdsObject*> >  Element_s;

typedef GdsParser::GdsDB::GdsShape::coordinate_type                                             Coordinate_type;
typedef std::vector< boost::polygon::point_data<GdsParser::GdsDB::GdsShape::coordinate_type> >  Coordinate_s;
typedef std::vector<GdsParser::GdsDB::GdsPolygon*>                                              Ploygons_s;


int main(int argc, char** argv)
{
    GdsParser::GdsDB::GdsDB db;
    GdsParser::GdsDB::GdsReader reader (db);
    limboAssert(reader(argv[1]));

    Ploygons_s ploygon_s;

/// read the gdsii and get ploygon vector

    for (Structure_s::const_iterator str = db.cells().begin(); str != db.cells().end(); ++str){

        std::cout << "cell: " << str->name() << std::endl;

        for(Element_s::const_iterator ele = str->objects().begin(); ele != str->objects().end(); ++ele){
            if(ele->first == GdsParser::GdsRecords::BOUNDARY){
                ploygon_s.push_back(dynamic_cast<GdsParser::GdsDB::GdsPolygon*>(ele->second));
            }
        }
    }

#if 0
    int length = ploygon_s.size();
    std::cout << "length: " << length << std::endl;

    // exaim the ploygon vertex duplicate
    for(int i=0; i < length; i++){
        GdsParser::GdsDB::GdsPolygon* test = ploygon_s[i];
        std::set< pair<Coordinate_type, Coordinate_type> > seenPairs;
        for(Coordinate_s::const_iterator coor = test->begin(); coor != test->end() -1; ++coor){// polygon is closed, so the last point is the same as the first point   
            pair <Coordinate_type, Coordinate_type> temp;
            temp.first = coor->x();
            temp.second = coor->y();
            if (!seenPairs.insert(temp).second) {
                cout << "In ploygon " << i << "Duplicate pair: (" << temp.first << ", " << temp.second << ")\n";
            }
        }
    }

#endif

// access the coordiante    
///    GdsParser::GdsDB::GdsPolygon* test = ploygon_s[1];

#if 0
    GdsParser::GdsDB::GdsPolygon* test = ploygon_s[4];
    // show coordinate of test
    for(Coordinate_s::const_iterator coor = test->begin(); coor != test->end(); ++coor){
        std::cout << "x: " << coor->x() << ", y: " << coor->y() << std::endl;     
    }
#endif

#if 1
    GdsParser::GdsDB::GdsPolygon* test = ploygon_s[4];

    std::vector< Coor<int> > polygon;
    std::vector< Rect<int> > result;

    for(Coordinate_s::const_iterator coor = test->begin(); coor != test->end(); ++coor){
        polygon.emplace_back(coor->x(), coor->y());
    }

    rectirization(polygon.cbegin(), polygon.cend(), result);

    std::cout << "rectangle list:\n";
    for (const auto& rect : result) {
        std::cout << "\t<" << rect.getBL() << " - " << rect.getTR() << ">\n";
    }
#endif

    return 0;
} 