#ifndef Decomposition_hpp
#define Decomposition_hpp

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <tuple>
#include <algorithm>

#include "Polygon.hpp"
#include "Rectangle.hpp"

template <typename T>
static auto findCoorTuple_X(std::vector< Coor<T> >& vertices) // if not using the reference, the vertices will be changed
    -> std::tuple<typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator>;

template <typename T>
static auto findCoorTuple_Y(std::vector< Coor<T> >& vertices) // if not using the reference, the vertices will be changed
    -> std::tuple<typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator>;


template <typename T>
void Edge_based_decomposition(Polygon_edge_collection<T>& Polygon_edges, std::vector< Rect<T> >& result)
{
    result.clear();

    if(Polygon_edges.edges.size() == 0){
        Polygon_edges.vertices_2_edges();
    }
    else if(Polygon_edges.vertices.size() == 0){
        Polygon_edges.edges_2_vertices();
    }

#if 0
    //display the edges
    std::cout << "Edges: " << std::endl;
    for(auto iter = Polygon_edges.edges.begin(); iter != Polygon_edges.edges.end(); iter++)
    {
        std::cout << iter->Coor_pair.first << " " << iter->Coor_pair.second << std::endl;
    }   
#endif

    while (Polygon_edges.edges.size()>1)
    //for(int i=0; i<1; ++i)
    //for(int i=0; i<500; ++i)
    {
        //std::cout << "Edges size: " << Polygon_edges.edges.size() << std::endl;

        const auto& coor_tuple_X = findCoorTuple_X(Polygon_edges.vertices);
        const auto Pk_X = *(std::get<0>(coor_tuple_X));
        const auto Pl_X = *(std::get<1>(coor_tuple_X));
        const auto Pm_X = *(std::get<2>(coor_tuple_X));
        T area_X = (Pl_X.getX() - Pk_X.getX()) * (Pm_X.getY() - Pk_X.getY());

        const auto& coor_tuple_Y = findCoorTuple_Y(Polygon_edges.vertices);
        const auto Pk_Y = *(std::get<0>(coor_tuple_Y));
        const auto Pl_Y = *(std::get<1>(coor_tuple_Y));
        const auto Pm_Y = *(std::get<2>(coor_tuple_Y));
        T area_Y = (Pm_Y.getX() - Pk_Y.getX()) * (Pk_Y.getY() - Pl_Y.getY());

        // choose the larger area
        int Flag = (area_X >= area_Y) ? 0 : 1;
        const auto& Pk = (Flag == 0) ? Pk_X : Pk_Y;
        const auto& Pl = (Flag == 0) ? Pl_X : Pl_Y;
        const auto& Pm = (Flag == 0) ? Pm_X : Pm_Y;

#if 0
        std::cout << "i: " << i << std::endl;
        std::cout << "Pk: " << Pk << std::endl;
        std::cout << "Pl: " << Pl << std::endl;
        std::cout << "Pm: " << Pm << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
#endif

        if(Flag == 0)
        {
            // add the rectangle to the result
            result.emplace_back(Pk.getX(), Pk.getY(), 
                                Pl.getX() - Pk.getX(), Pm.getY() - Pk.getY());
        }
        else
        {
            // add the rectangle to the result
            result.emplace_back(Pl.getX(), Pl.getY(), 
                                Pm.getX() - Pk.getX(), Pk.getY() - Pl.getY());
        }

        // Upl and Upr
        Coor<T> upl = (Flag == 0) ? Coor<T>(Pk.getX(), Pm.getY()) : Coor<T>(Pm.getX(), Pk.getY());
        Coor<T> upr = (Flag == 0) ? Coor<T>(Pl.getX(), Pm.getY()) : Coor<T>(Pm.getX(), Pl.getY());
        
        // edges of the rectangle
        edge<T> e1(Pk, upl), e2(upl, upr), e3(upr, Pl), e4(Pl, Pk);
        // add to the Polygon_edges.edges

        if(Flag == 0){
            edge_list_edge_complement<T>(Polygon_edges.edges, e1, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e2, HORIZONTAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e3, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e4, HORIZONTAL);
        }
        else{
            edge_list_edge_complement<T>(Polygon_edges.edges, e1, HORIZONTAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e2, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e3, HORIZONTAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e4, VERTICAL);
        }

#if 0
        if(i >= 1){
            edge_list_edge_complement<T>(Polygon_edges.edges, e1, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e2, HORIZONTAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e3, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e4, HORIZONTAL);
        }
        else{
            edge_list_edge_complement<T>(Polygon_edges.edges, e1, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e2, HORIZONTAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e3, VERTICAL);
            edge_list_edge_complement<T>(Polygon_edges.edges, e4, HORIZONTAL);
        }
#endif
        Polygon_edges.edges_2_vertices();

#if 0
        //output to Edge.txt
        std::ofstream outfile("Edge.txt");
        //clear the file
        outfile.clear();
        for(auto iter = Polygon_edges.edges.begin(); iter != Polygon_edges.edges.end(); iter++)
        {
            outfile << iter->Coor_pair.first << " " << iter->Coor_pair.second << std::endl;
        }
        outfile.close();
#endif

#if 0
        //display the edges
        std::cout << "Edges: " << std::endl;
        for(auto iter = Polygon_edges.edges.begin(); iter != Polygon_edges.edges.end(); iter++)
        {
            std::cout << iter->Coor_pair.first << " " << iter->Coor_pair.second << std::endl;
        }
        // display the vertices
        std::cout << "Vertices: " << std::endl;
        for(auto iter = Polygon_edges.vertices.begin(); iter != Polygon_edges.vertices.end(); iter++)
        {
            std::cout << *iter << std::endl;
        }

        std::cout << "----------------------------------------------------------" << std::endl;
#endif
    }   
}


/***********************************************************************/

template <typename T>
static auto findCoorTuple_X(std::vector< Coor<T> >& vertices) 
    -> std::tuple<typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator>
{
    // find Pk: the bottom and the left-most coordinate
    const auto Pk = std::min_element(vertices.begin(), vertices.end(), 
        [](const auto& lhs, const auto& rhs) -> bool {
                if (lhs.getY() != rhs.getY()) { 
                    return (lhs.getY() < rhs.getY()); 
                    }
                else { 
                    return (lhs.getX() < rhs.getX()); 
                    }
            }
        );
    // find Pl: the bottom and the left-most coordinate except Pk
    const auto Pl = std::min_element(vertices.begin(), vertices.end(), 
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
    const auto Pm = std::min_element(vertices.begin(), vertices.end(), 
        [&Pk, &Pl](const auto& lhs, const auto& rhs) -> bool {
                // skip the element (x, y) out of the range:
                // Pk.getX() <= x < Pl.getX() && Pk.getY() < y
                if ((lhs.getY() <= Pk->getY()) || 
                    (lhs.getX() < Pk->getX()) || 
                    //(lhs.getX() >= Pl->getX())) {
                    (lhs.getX() > Pl->getX())) {
                        return false; 
                    }
                else if ((rhs.getY() <= Pk->getY()) || 
                        (rhs.getX() < Pk->getX()) || 
                        //(rhs.getX() >= Pl->getX())) {
                        (rhs.getX() > Pl->getX())) {  
                            return true; 
                    }
                // find the smallest element
                else if (lhs.getY() != rhs.getY()) { 
                    return (lhs.getY() < rhs.getY()); 
                    }
                else { return (lhs.getX() < rhs.getX()); }
                
                return true;
            }
        );

    return {Pk, Pl, Pm};
}

template <typename T>
static auto findCoorTuple_Y(std::vector< Coor<T> >& vertices) 
    -> std::tuple<typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator, 
                    typename std::vector< Coor<T> >::iterator>
{
    // find Pk: the most left and most top coordinate
    const auto Pk = std::min_element(vertices.begin(), vertices.end(), 
        [](const auto& lhs, const auto& rhs) -> bool {
                if (lhs.getX() != rhs.getX()) { 
                    return (lhs.getX() < rhs.getX()); 
                    }
                else { 
                    return (lhs.getY() > rhs.getY()); 
                    }
            }
        );
    // find Pl: the most left and most top coordinate except Pk
    const auto Pl = std::min_element(vertices.begin(), vertices.end(), 
        [&Pk](const auto& lhs, const auto& rhs) -> bool {
                // skip the element Pk
                if (lhs == (*Pk)) { 
                    return false; 
                    }
                else if (rhs == (*Pk)) { 
                    return true; 
                    }
                // find the most top element
                else if (lhs.getX() != rhs.getX()) { 
                    return (lhs.getX() < rhs.getX()); 
                    }
                else { 
                    return (lhs.getY() > rhs.getY()); 
                }
            }
        );
    // find Pm
    const auto Pm = std::min_element(vertices.begin(), vertices.end(), 
        [&Pk, &Pl](const auto& lhs, const auto& rhs) -> bool {
                // skip the element (x, y) out of the range:
                // Pk.getY() <= y < Pl.getY() && Pk.getX() < x
                if ((lhs.getX() <= Pk->getX()) || 
                    (lhs.getY() > Pk->getY()) || 
                    //(lhs.getY() >= Pl->getY())) {
                    (lhs.getY() < Pl->getY())) {
                        return false; 
                    }
                else if ((rhs.getX() <= Pk->getX()) || 
                        (rhs.getY() > Pk->getY()) || 
                        //(rhs.getY() >= Pl->getY())) {
                        (rhs.getY() < Pl->getY())) {  
                            return true; 
                    }
                // find the most top element
                else if (lhs.getX() != rhs.getX()) { 
                    return (lhs.getX() < rhs.getX()); 
                    }
                else { return (lhs.getY() > rhs.getY()); }
        }
    );

    return {Pk, Pl, Pm};
}


#endif