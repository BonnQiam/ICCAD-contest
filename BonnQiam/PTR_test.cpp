#include <vector>
#include <iterator>
#include <iostream>

#include "PTR.hpp"

using namespace std;

int main()
{
    vector< Coor<int> > polygon;
    vector< Rect<int> > result;

#if 0
    polygon.emplace_back(1, 1);
    polygon.emplace_back(3, 1);
    polygon.emplace_back(3, 3);
    polygon.emplace_back(4, 3);
    polygon.emplace_back(4, 1);
    polygon.emplace_back(5, 1);
    polygon.emplace_back(5, 5);
    polygon.emplace_back(4, 5);
    polygon.emplace_back(4, 4);
    polygon.emplace_back(2, 4);
    polygon.emplace_back(2, 2);
    polygon.emplace_back(1, 2);
#endif

#if 1    
    polygon.emplace_back(79808, 342852);
    polygon.emplace_back(79814, 342852);
    polygon.emplace_back(79814, 342916);
    polygon.emplace_back(80037, 342916);
    polygon.emplace_back(80037, 343172);
    polygon.emplace_back(79985, 343172);
    polygon.emplace_back(79985, 342932);
    polygon.emplace_back(79797, 342932);
    polygon.emplace_back(79797, 343156);
    polygon.emplace_back(79985, 343156);
    polygon.emplace_back(79985, 343172);
    polygon.emplace_back(79764, 343172);
    polygon.emplace_back(79764, 343092);
    polygon.emplace_back(79728, 343092);
    polygon.emplace_back(79728, 342996);
    polygon.emplace_back(79505, 342996);
    polygon.emplace_back(79505, 342948);
    polygon.emplace_back(79675, 342948);
    polygon.emplace_back(79675, 342916);
    polygon.emplace_back(79808, 342916);
#endif

#if 0
    polygon.emplace_back(79385, 56692);
    polygon.emplace_back(79385, 57236);
    polygon.emplace_back(80174, 57236);
    polygon.emplace_back(80174, 57132);
    polygon.emplace_back(80105, 57132);
    polygon.emplace_back(80105, 57196);
    polygon.emplace_back(79917, 57196);
    polygon.emplace_back(79917, 56796);
    polygon.emplace_back(80237, 56796);
    polygon.emplace_back(80237, 56788);
    polygon.emplace_back(79934, 56788);
    polygon.emplace_back(79934, 56692);
    polygon.emplace_back(79385, 56692);
#endif

    rectirization(polygon.cbegin(), polygon.cend(), result);

    cout << "rectangle list:\n";
    for (const auto& rect : result) {
        cout << "\t<" << rect.getBL() << " - " << rect.getTR() << ">\n";
    }

    return 0;
}