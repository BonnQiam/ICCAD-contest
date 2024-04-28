#ifndef Polygon_hpp
#define Polygon_hpp

#include "Coor.hpp"
#include <vector>
#include <iostream>
#include <algorithm> 

#define HORIZONTAL 0
#define VERTICAL 1

#define OVERLAP_subset  0   // e2 is the subset of e1, including the case that one point of e2 is the same as one point of e1
#define OVERLAP_full    1   // e1 and e2 are same
#define OVERLAP_partial 2   // e1 and e2 are overlapped partially, including the case that e1 is the subset of e2
#define OVERLAP_point   3   // e1 and e2 are overlapped at a point
#define OVERLAP_none    4   // e1 and e2 are not overlapped

template <typename T>
struct edge
{
    std::pair< Coor<T>, Coor<T> > Coor_pair;
    edge(const Coor<T>& a, const Coor<T>& b) : Coor_pair(a, b) {}
};

template <typename T>
void edges_sort(std::vector< edge<T> >& edges, int sort_type)
{
    if(sort_type == HORIZONTAL){
        // sort the edges by the min x value of the edge.first and edge.second
        std::sort(edges.begin(), edges.end(), [](const edge<T>& a, const edge<T>& b){
            return a.Coor_pair.first.getX() < b.Coor_pair.first.getX();
        });
    }
    else if(sort_type == VERTICAL){
        // sort the edges by the min y value of the edge.first and edge.second
        std::sort(edges.begin(), edges.end(), [](const edge<T>& a, const edge<T>& b){
            return a.Coor_pair.first.getY() < b.Coor_pair.first.getY();
        });
    }
}

template <typename T>
void sort_edge(edge<T>& e, int type)
{
    if(type == HORIZONTAL){
        if(e.Coor_pair.first.getX() > e.Coor_pair.second.getX()){
            std::swap(e.Coor_pair.first, e.Coor_pair.second);
        }
    }
    else if(type == VERTICAL){
        if(e.Coor_pair.first.getY() > e.Coor_pair.second.getY()){
            std::swap(e.Coor_pair.first, e.Coor_pair.second);
        }
    }
}

template <typename T>
int after_overlapped(edge<T>& e1, edge<T>& e2){
    // remove the overlapped part of e1 and e2 from e1 and e2

    // if e1, e2 are horizontal edges with same y
    if(e1.Coor_pair.first.getY() == e1.Coor_pair.second.getY() && 
        e2.Coor_pair.first.getY() == e2.Coor_pair.second.getY() &&
        e1.Coor_pair.first.getY() == e2.Coor_pair.first.getY()
        ){
        
        // sort the e1 and e2, so that first point of e1/e2 is the leftmost point
        sort_edge(e1, HORIZONTAL);
        sort_edge(e2, HORIZONTAL);

        if(e1.Coor_pair.second.getX() < e2.Coor_pair.first.getX() || 
            e2.Coor_pair.second.getX() < e1.Coor_pair.first.getX()){
            return OVERLAP_none;
        }
        else if (
            e1.Coor_pair.second.getX() == e2.Coor_pair.first.getX() ||
            e2.Coor_pair.second.getX() == e1.Coor_pair.first.getX()
        ){
            return OVERLAP_point;
        }
        else{
            // if e1 and e2 overlap, remove the overlapped part
            
            int flag;
            //check if e1 and e2 are same
            if(e1.Coor_pair.first == e2.Coor_pair.first && e1.Coor_pair.second == e2.Coor_pair.second){
                flag = OVERLAP_full;
            }
            //check if e2 is the subset of e1
            else if(e2.Coor_pair.first.getX() >= e1.Coor_pair.first.getX() && e2.Coor_pair.second.getX() <= e1.Coor_pair.second.getX()){
                flag = OVERLAP_subset;
            }
            else{
                flag = OVERLAP_partial;
            }
            
            Coor<T> tmp = e1.Coor_pair.second;
            e1.Coor_pair.second = e2.Coor_pair.first;
            e2.Coor_pair.first = tmp;
            
            sort_edge(e1, HORIZONTAL);
            sort_edge(e2, HORIZONTAL);

            return flag;
        }
    }
    // if e1, e2 are vertical edges with sanme X
    else if(e1.Coor_pair.first.getX() == e1.Coor_pair.second.getX() && 
        e2.Coor_pair.first.getX() == e2.Coor_pair.second.getX()
        && e1.Coor_pair.first.getX() == e2.Coor_pair.first.getX()){
        // sort the e1 and e2, so that first point of e1/e2 is the bottommost point
        sort_edge(e1, VERTICAL);
        sort_edge(e2, VERTICAL);

        if(e1.Coor_pair.second.getY() < e2.Coor_pair.first.getY() || 
            e2.Coor_pair.second.getY() < e1.Coor_pair.first.getY()){
            return OVERLAP_none;
        }
        else if (
            e1.Coor_pair.second.getY() == e2.Coor_pair.first.getY() ||
            e2.Coor_pair.second.getY() == e1.Coor_pair.first.getY()
        ){
            return OVERLAP_point;
        }
        else{
            // if e1 and e2 overlap, remove the overlapped part
            
            int flag;
            //check if e1 and e2 are same
            if(e1.Coor_pair.first == e2.Coor_pair.first && e1.Coor_pair.second == e2.Coor_pair.second){
                flag = OVERLAP_full;
            }
            //check if e2 is the subset of e1
            else if(e2.Coor_pair.first.getY() >= e1.Coor_pair.first.getY() && e2.Coor_pair.second.getY() <= e1.Coor_pair.second.getY()){
                flag = OVERLAP_subset;
            }
            else{
                flag = OVERLAP_partial;
            }
            
            Coor<T> tmp = e1.Coor_pair.second;
            e1.Coor_pair.second = e2.Coor_pair.first;
            e2.Coor_pair.first = tmp;
            
            sort_edge(e1, VERTICAL);
            sort_edge(e2, VERTICAL);

            return flag;
        }
    }
    else{
        return OVERLAP_none;
    }
}

template <typename T>
void edge_list_edge_complement(std::vector< edge<T> >& edge_list, 
                    edge<T> e2, int sort_type)
{
    edges_sort<T>(edge_list, sort_type);
    // If the e2 overlap with some edges of edge_list, the overlapped parts of these edges should be removed, and the remaining parts (not exist in the edge_list) of e2 should be added into the edge_list.
    std::vector< edge<T> > add_edges;
    for(auto iter = edge_list.begin(); iter != edge_list.end();)
    {
        // calculate the overlapped part of e2 and *iter
        edge<T> e1 = *iter;
        // check if e1 is a point
        if(e1.Coor_pair.first == e1.Coor_pair.second){
            iter = edge_list.erase(iter);
            continue;
        }
#if 0
        std::cout << "--------------------------------" << std::endl;
        std::cout << "e1 is " << e1.Coor_pair.first << " " << e1.Coor_pair.second << std::endl;
        std::cout << "e2 is " << e2.Coor_pair.first << " " << e2.Coor_pair.second << std::endl;
#endif
        int flag = after_overlapped(e1, e2);

//        std::cout << "flag: " << flag << std::endl;

        if(flag == OVERLAP_subset){
            if(e1.Coor_pair.first == e1.Coor_pair.second){
                //if iter is a point, then remove iter and add e2 into the edge_list
                iter = edge_list.erase(iter);
                add_edges.push_back(e2);
            }
            else if(e2.Coor_pair.first == e2.Coor_pair.second){
                //if e2 is a point, then no need to add e2 into the edge_list, just update the *iter
                *iter = e1;
            }
            else{
                *iter = e1;
                add_edges.push_back(e2);
            }
            break;
        }
        else if(flag == OVERLAP_full){
            // remove the overlapped edge
            iter = edge_list.erase(iter);
            break;
        }
        else if(flag == OVERLAP_partial){
            //check if e1 is point
            if(e1.Coor_pair.first != e1.Coor_pair.second){
                *iter = e1;
                
                if(std::next(iter) == edge_list.end()){
                    if(e2.Coor_pair.first != e2.Coor_pair.second){
                        // add the e2 into the edge_list
                        add_edges.push_back(e2);
                    }
                    break;
                }
            }
            else{
                iter = edge_list.erase(iter);

                if(iter == edge_list.end()){
                    if(e2.Coor_pair.first != e2.Coor_pair.second){
                        // add the e2 into the edge_list
                        add_edges.push_back(e2);
                    }
                    break;
                }

                continue;
            }
        }
        else if(flag == OVERLAP_point){
            e2.Coor_pair.first.setX(std::min(e1.Coor_pair.first.getX(), e2.Coor_pair.first.getX()));
            e2.Coor_pair.first.setY(std::min(e1.Coor_pair.first.getY(), e2.Coor_pair.first.getY()));
            e2.Coor_pair.second.setX(std::max(e1.Coor_pair.second.getX(), e2.Coor_pair.second.getX()));
            e2.Coor_pair.second.setY(std::max(e1.Coor_pair.second.getY(), e2.Coor_pair.second.getY()));

//            std::cout << "In this case, e2 is " << e2.Coor_pair.first << " " << e2.Coor_pair.second << std::endl;

            if(std::next(iter) == edge_list.end()){
                iter = edge_list.erase(iter);
                // add the e2 into the edge_list
                add_edges.push_back(e2);
                break;
            }
            else{
                iter = edge_list.erase(iter);
                continue;
            }
        }
        else if(flag == OVERLAP_none){
            if(std::next(iter) == edge_list.end()){
                // add the e2 into the edge_list
                add_edges.push_back(e2);
                break;
            }
        }
        iter++;
    }

    // add the add_edges into the edge_list
    edge_list.insert(edge_list.end(), add_edges.begin(), add_edges.end());
}

template <typename T>
struct Polygon_edge_collection
{
    std::vector< edge<T> > edges;
    std::vector< Coor<T> > vertices;

    void edges_sort(int sort_type);
    void edges_2_vertices();
};

template <typename T>
void Polygon_edge_collection<T>::edges_2_vertices()
{
    vertices.clear();
    for(auto& e : edges)
    {
        vertices.push_back(e.Coor_pair.first);
        vertices.push_back(e.Coor_pair.second);
    }

    // remove the duplicate vertices
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
}

#endif