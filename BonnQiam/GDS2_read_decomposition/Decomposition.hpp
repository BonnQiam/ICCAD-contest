#ifndef Decomposition_hpp
#define Decomposition_hpp

#include "Polygon.hpp"
#include <tuple>
#include <algorithm>

#define Debug 0

template <typename T>
static auto findCoorTuple_Refactor(Polygon<T>& polygon)
    -> std::tuple<Coor<T>, Coor<T>, Coor<T>>;


template <typename T>
void Decomposition(
    const Polygon<T>& poly,
    std::vector< Polygon<T> >& result
)
{
    result.clear();

    Polygon<T> poly_copy = poly;
    poly_copy.vertexes   = poly.vertexes;
    poly_copy.edges      = poly.edges;

    while(poly_copy.vertexes.size() > 1){
//    for(int i=0; i<13; i++){
        const auto& coor_tuple = findCoorTuple_Refactor(poly_copy);

        const auto Pk = std::get<0>(coor_tuple);
        const auto Pl = std::get<1>(coor_tuple);
        const auto Pm = std::get<2>(coor_tuple);

#if Debug
        std::cout << "Pk: " << Pk << std::endl;
        std::cout << "Pl: " << Pl << std::endl;
        std::cout << "Pm: " << Pm << std::endl;
#endif
        Coor<T> Upl(Pk.getX(), Pm.getY()), Upr(Pl.getX(), Pm.getY());

        Polygon<T> Rectangle;
        Rectangle.vertexes.push_back(Pk);
        Rectangle.vertexes.push_back(Pl);
        Rectangle.vertexes.push_back(Upr);
        Rectangle.vertexes.push_back(Upl);
        
        Rectangle.edges_init();

        result.push_back(Rectangle);

        Polygon<T> poly_complement;
        Edge_list_complement(poly_copy, Rectangle, poly_complement);

#if Debug
        std::cout << "poly_complement: " << std::endl;
        for(auto v: poly_complement.vertexes){
            std::cout << v << std::endl;
        }
#endif
        // copy poly_complement to poly_copy
        poly_copy.vertexes = poly_complement.vertexes;
        poly_copy.edges    = poly_complement.edges;

//        std::cout << "==========================" << std::endl;
    }

#if Debug
    std::cout << "The rectangle list is " << std::endl;
    for(auto rec: result){
        for(auto v: rec.vertexes){
            std::cout << v << std::endl;
        }
        std::cout << "--------------------" << std::endl;
    }
#endif
}


template <typename T>
static auto findCoorTuple_Refactor(Polygon<T>& polygon)
    -> std::tuple<Coor<T>, Coor<T>, Coor<T>>
{   
    Polygon<T> poly_copy = polygon;
    poly_copy.vertexes   = polygon.vertexes;
    poly_copy.edges      = polygon.edges;

    bool legal_Pk = false;
    auto Pk = poly_copy.vertexes.begin();
    while(1){
        Pk = std::min_element(poly_copy.vertexes.begin(), poly_copy.vertexes.end(), 
        [](const auto& lhs, const auto& rhs) -> bool {
                if (lhs.getY() != rhs.getY()) { 
                    return (lhs.getY() < rhs.getY()); 
                    }
                else { 
                    return (lhs.getX() < rhs.getX()); 
                    }
            }
        );
        // Check the Pk
        Coor<T> Test = *Pk;
        Test.addToX(1);
        Test.addToY(1);

        legal_Pk = polygon.isInside(Test);

        if(legal_Pk)
            break;
        else{
            poly_copy.vertexes.erase(Pk);
        }
        
    }

    // find Pl: the bottom and the left-most coordinate except Pk
    auto Pl = std::min_element(poly_copy.vertexes.begin(), poly_copy.vertexes.end(), 
        [&Pk](const auto& lhs, const auto& rhs) -> bool {
                // skip the element Pk
                if (lhs == (*Pk)) { 
                    return false; 
                    }
                else if (rhs == (*Pk)) { 
                    return true; 
                    }
                // find the smallest element
                else if (lhs.getY() != rhs.getY()) { 
                    return (lhs.getY() < rhs.getY()); 
                    }
                else { 
                    return (lhs.getX() < rhs.getX()); 
                }
            }
        );

    // find Pm
    auto Pm = std::min_element(poly_copy.vertexes.begin(), poly_copy.vertexes.end(), 
        [&Pk, &Pl](const auto& lhs, const auto& rhs) -> bool {
                // skip the element (x, y) out of the range:
                // Pk.getX() <= x < Pl.getX() && Pk.getY() < y
                if ((lhs.getY() <= Pk->getY()) || 
                    (lhs.getX() < Pk->getX()) || 
                    (lhs.getX() >= Pl->getX())) { 
                        return false; 
                    }
                else if ((rhs.getY() <= Pk->getY()) || 
                        (rhs.getX() < Pk->getX()) || 
                        (rhs.getX() >= Pl->getX())) { 
                            return true; 
                    }
                // find the smallest element
                else if (lhs.getY() != rhs.getY()) {
                    return (lhs.getY() < rhs.getY()); 
                    }
                else { 
                    return (lhs.getX() < rhs.getX()); 
                }
            }
        );
    
    return std::make_tuple(*Pk, *Pl, *Pm);
}

#endif