#ifndef Polygon_hpp
#define Polygon_hpp

#include "Coor.hpp"
#include <vector>

#define log 0

/*******************************************************************
 * Rectangle Definition
*/
template <typename T>
struct Rectangle
{
    Coor<T> TopLeft;
    Coor<T> BottomRight;
    
    Rectangle(const Coor<T>& tl, const Coor<T>& tr) : TopLeft(tl), BottomRight(tr) {}
    double Area() const;
};

template <typename T>
double Rectangle<T>::Area() const
{
    double area = (BottomRight.getX() - TopLeft.getX()) * (TopLeft.getY() - BottomRight.getY());

    return area;
}

// return intersection reactangle of two Rectangle
template <typename T>
Rectangle<T> Rectangle_intersection(const Rectangle<T>& r1, const Rectangle<T>& r2)
{
    // Check intersection
    Coor<T> tl(std::max(r1.TopLeft.getX(), r2.TopLeft.getX()), std::min(r1.TopLeft.getY(), r2.TopLeft.getY()));
    Coor<T> br(std::min(r1.BottomRight.getX(), r2.BottomRight.getX()), std::max(r1.BottomRight.getY(), r2.BottomRight.getY()));

    //check if the intersection is valid
    if(tl.getX() > br.getX() || tl.getY() < br.getY())
        return Rectangle<T>(Coor<T>(0, 0), Coor<T>(0, 0));
    
    return Rectangle<T>(tl, br);
}

/************************************************************
 * Edge Definition
*/
template <typename T>
struct edge
{
    std::pair< Coor<T>, Coor<T> > Coor_pair;
    edge(const Coor<T>& a, const Coor<T>& b) : Coor_pair(a, b) {}
/*
    edge(const Coor<T>& a, const Coor<T>& b){
        if(a.getX() < b.getX()){
            Coor_pair.first = a;
            Coor_pair.second = b;
        }
        else if(a.getX() > b.getX()){
            Coor_pair.first = b;
            Coor_pair.second = a;
        }
        else{
            if(a.getY() < b.getY()){
                Coor_pair.first = a;
                Coor_pair.second = b;
            }
            else{
                Coor_pair.first = b;
                Coor_pair.second = a;
            }
        }
    }
*/
};
/************************************************************
 * Polygon Definition
*/
template <typename T>
struct Polygon
{   
    // vertexes set of the polygon
    std::vector<Coor<T>> vertexes;
    // edges set of the polygon
    std::vector<edge<T>> edges;

    void edges_init(); // according to vertexes, initialize edges
    void vertexes_init(); // according to edges, initialize vertexes
    bool isInside(Coor<T> point); // check if the point is inside the polygon based on the Winding Number Algorithm

};

template <typename T>
void Polygon<T>::edges_init()
{
    edges.clear();
    auto vertex = vertexes.begin();
    for(; vertex != (vertexes.end()-1); vertex++){
        edges.push_back(edge<T>(*vertex, *(vertex+1)));
    } 
}

template <typename T>
void Polygon<T>::vertexes_init()
{
    vertexes.clear();
    auto edge = edges.begin();
    for(; edge != edges.end(); edge++){
        vertexes.push_back(edge->Coor_pair.first);
    }
    vertexes.push_back(edges.back().Coor_pair.second);
}

// Calculate the area of a triangle formed by three points
template <typename T>
double triangleArea(const Coor<T>& p1, const Coor<T>& p2, const Coor<T>& p3) {
    // calculate the area of a triangle
    return 0.5 * (p1.getX() * p2.getY() + p2.getX() * p3.getY() + p3.getX() * p1.getY() - p1.getX() * p3.getY() - p2.getX() * p1.getY() - p3.getX() * p2.getY());
}

template <typename T>
bool Polygon<T>::isInside(Coor<T> point)
{
    int windingNumber = 0;

    for(int i=0; i<vertexes.size()-1; i++){
        if(vertexes[i].getY() <= point.getY()){
            if(vertexes[(i+1) % vertexes.size()].getY() > point.getY()){
                if(isLeft(vertexes[i], vertexes[(i+1) % vertexes.size()], point) > 0)
                    windingNumber++;
            }
        }
        else{
            if(vertexes[(i+1) % vertexes.size()].getY() <= point.getY()){
                if(isLeft(vertexes[i], vertexes[(i+1) % vertexes.size()], point) < 0)
                    windingNumber--;
            }
        }

        double area = triangleArea(vertexes[i], vertexes[(i+1) % vertexes.size()], point);
        if(area == 0){
            return true;
        }
    }

    return windingNumber != 0;
}


/**************************************************************************************
 * remove the redundancy point in the polygon
*/
template <typename T>
void Polygon_shrink_redundancy_point(const Polygon<T> &polygon,  Polygon<T> &polygon_shrink)
{
    auto e = polygon.edges.begin();
    auto v = polygon_shrink.vertexes.begin() + 1;

    while(e != polygon.edges.end()){
        auto e_nxt = ( (e+1) == polygon.edges.end() ) ? polygon.edges.begin() : (e+1);

        bool find_duplicate = false;

        T e_X     = e->Coor_pair.second.getX() - e->Coor_pair.first.getX();
        T e_nxt_X = e_nxt->Coor_pair.second.getX() - e_nxt->Coor_pair.first.getX();

        T e_Y     = e->Coor_pair.second.getY() - e->Coor_pair.first.getY();
        T e_nxt_Y = e_nxt->Coor_pair.second.getY() - e_nxt->Coor_pair.first.getY();

        if(
            (e->Coor_pair.second.getY() == e->Coor_pair.first.getY() ) &&
            (e_nxt->Coor_pair.second.getY() == e_nxt->Coor_pair.first.getY() )
        ){
            if(e_X*e_nxt_X > 0)
                find_duplicate = true;
        }
        else if ( (e->Coor_pair.second.getX() == e->Coor_pair.first.getX() ) &&
            (e_nxt->Coor_pair.second.getX() == e_nxt->Coor_pair.first.getX() )
        ){
            if(e_Y*e_nxt_Y > 0)
                find_duplicate = true;
        }
        
        if(find_duplicate){
            if( (e+1) == polygon.edges.end() ){
                *(polygon_shrink.vertexes.end()-1) = *(polygon_shrink.vertexes.begin()+1);
                polygon_shrink.vertexes.erase(polygon_shrink.vertexes.begin());
                break;
            }
            else{
                e = e+1;
                v = polygon_shrink.vertexes.erase(v);
            }
        }
        else{
            e = e+1;
            v = v+1;
        }
    }

    polygon_shrink.edges_init();
}

/****************************************************************************************
 * remove the redundancy edge in the polygon
*/
template <typename T>
bool Polygon_shrink_redundancy_edge(const Polygon<T> &polygon,  Polygon<T> &polygon_shrink)
{
    int original_size = polygon_shrink.vertexes.size();

    auto e = polygon.edges.begin();
    auto v = polygon_shrink.vertexes.begin() + 1;

    while( e != polygon.edges.end()){
        auto e_nxt = ( (e+1) == polygon.edges.end() ) ? polygon.edges.begin() : (e+1);

        bool find_duplicate = false;

        T e_X     = e->Coor_pair.second.getX() - e->Coor_pair.first.getX();
        T e_nxt_X = e_nxt->Coor_pair.second.getX() - e_nxt->Coor_pair.first.getX();

        T e_Y     = e->Coor_pair.second.getY() - e->Coor_pair.first.getY();
        T e_nxt_Y = e_nxt->Coor_pair.second.getY() - e_nxt->Coor_pair.first.getY();

        int e_X_sign = (e_X > 0) ? 1 : -1;
        int e_Y_sign = (e_Y > 0) ? 1 : -1;
        int e_nxt_X_sign = (e_nxt_X > 0) ? 1 : -1;
        int e_nxt_Y_sign = (e_nxt_Y > 0) ? 1 : -1;

        if(
            (e->Coor_pair.second.getY() == e->Coor_pair.first.getY() ) &&
            (e_nxt->Coor_pair.second.getY() == e_nxt->Coor_pair.first.getY() )
        ){
            if(e_X_sign != e_nxt_X_sign)
                find_duplicate = true;
        }
        else if ( (e->Coor_pair.second.getX() == e->Coor_pair.first.getX() ) &&
            (e_nxt->Coor_pair.second.getX() == e_nxt->Coor_pair.first.getX() )
        ){
            if(e_Y_sign != e_nxt_Y_sign)
                find_duplicate = true;
        }

        if(find_duplicate){
            T v_NewX  = e_nxt->Coor_pair.second.getX();
            T v_NewY  = e_nxt->Coor_pair.second.getY();

            if( (v_NewX == e->Coor_pair.first.getX()) && (v_NewY == e->Coor_pair.first.getY()) ){
                if( (e+1) == polygon.edges.end() ){
                    polygon_shrink.vertexes.erase(v);
                    polygon_shrink.vertexes.erase(polygon_shrink.vertexes.begin());
                    break;
                }
                else{
                    v = polygon_shrink.vertexes.erase(v);
                    v = polygon_shrink.vertexes.erase(v);
                    e = e+2;
                }
            }
            else{
                if( (e+1) == polygon.edges.end() ){
                    *(polygon_shrink.vertexes.end()-1) = *(polygon_shrink.vertexes.begin()+1);
                    polygon_shrink.vertexes.erase(polygon_shrink.vertexes.begin());
                    break;
                }
                else{
                    v = polygon_shrink.vertexes.erase(v)+1;
                    e = e+2;
                }
            }
        }
        else{
            e = e+1;
            v = v+1;
        }

    }

    
    int shrink_size = polygon_shrink.vertexes.size();
    if(original_size == shrink_size)
        return false;
    else
        polygon_shrink.edges_init();
        return true;
}

/***************************************************************************************
 * Edge complement operation
*/

template <typename T>
void Edge_list_complement(
    const Polygon<T> &polygon,
    const Polygon<T> &rectangle, // Pk -> Pl -> Upr -> Upl -> Pk
    Polygon<T> &polygon_complement)
{
    Coor<T> Pk  = rectangle.vertexes[0];
    Coor<T> Pl  = rectangle.vertexes[1];
    Coor<T> Upr = rectangle.vertexes[2];
    Coor<T> Upl = rectangle.vertexes[3];

//    std::cout << "Pk is " << Pk << std::endl;
//    std::cout << "Pl is " << Pl << std::endl;
//    std::cout << "Upr is " << Upr << std::endl;
//    std::cout << "Upl is " << Upl << std::endl;

    /***********************************************************************************
     * complement edges step 1: remove Pk-Pl and collect all other edges in polygon and rectangle
     */
    Polygon<T> polygon_tmp = polygon;
    polygon_tmp.vertexes = polygon.vertexes;
    polygon_tmp.edges = polygon.edges;

//    Polygon_shrink_redundancy_point(polygon, polygon_tmp);

//    std::cout << "Polygon_tmp is " << std::endl;
//    for(auto v : polygon_tmp.vertexes){
//        std::cout << "(" << v.getX() << ", " << v.getY() << ")" << std::endl;
//    }

    // find Pk -> Pl edges in polygon
    auto itr_kl = polygon_tmp.edges.begin();
    auto itr_v  = polygon_tmp.vertexes.begin();

    for(; itr_kl != polygon_tmp.edges.end(); itr_kl++){
        itr_v = itr_v + 1;

        if(itr_kl->Coor_pair.first.getY() == itr_kl->Coor_pair.second.getY()){
            if(itr_kl->Coor_pair.first == Pk){
                if(itr_kl->Coor_pair.second == Pl)
                    break;
                else {
                    polygon_tmp.vertexes.insert(itr_v, Pl);
                    polygon_tmp.edges_init();
                    break;
                }
            }
            else if(itr_kl->Coor_pair.first == Pl){
                if(itr_kl->Coor_pair.second == Pk)
                    break;
                else {
                    polygon_tmp.vertexes.insert(itr_v, Pk);
                    polygon_tmp.edges_init();
                    break;
                }
            }
        }
        else{
            continue;
        }
        
        if( (itr_kl+1) == polygon_tmp.edges.end()){
            std::cout << "Error: Pk -> Pl edge not found in polygon" << std::endl;
            return;
        }
    }

    // remove Pk-Pl and collect all other edges in polygon and rectangle
    for(auto e = polygon_tmp.edges.begin(); e != polygon_tmp.edges.end(); e++){
        if( (e->Coor_pair.first == Pk) && (e->Coor_pair.second == Pl) ){
            polygon_complement.edges.push_back(edge<T>(Pk, Upl));
            polygon_complement.edges.push_back(edge<T>(Upl, Upr));
            polygon_complement.edges.push_back(edge<T>(Upr, Pl));
        }
        else if((e->Coor_pair.first == Pl) && (e->Coor_pair.second == Pk)){
            polygon_complement.edges.push_back(edge<T>(Pl, Upr));
            polygon_complement.edges.push_back(edge<T>(Upr, Upl));
            polygon_complement.edges.push_back(edge<T>(Upl, Pk));
        }
        else
            polygon_complement.edges.push_back(*e);
    }
    polygon_complement.vertexes_init();

#if log
    std::cout << "Before shrink, polygon_complement is " << std::endl;
    for(auto v : polygon_complement.vertexes){
        std::cout << "(" << v.getX() << ", " << v.getY() << ")" << std::endl;
    }
#endif
   /**********************************************************************************
     * complement edges step 2: remove redundancy_edge
    */ 
    
    Polygon<int> poly_shrink;
    poly_shrink.vertexes = polygon_complement.vertexes;

    bool redundancy = true;

    while(redundancy){
        redundancy = Polygon_shrink_redundancy_edge(polygon_complement, poly_shrink);

        if(poly_shrink.vertexes.size() == 1){
            break;
        }

        if(redundancy){
            polygon_complement.vertexes = poly_shrink.vertexes;
            polygon_complement.edges = poly_shrink.edges;
        }
    }

#if log
    std::cout << "After shrink, polygon_complement is " << std::endl;
    for(auto v : poly_shrink.vertexes){
        std::cout << "(" << v.getX() << ", " << v.getY() << ")" << std::endl;
    }
#endif
    // Copy poly_shrink to polygon_complement
    polygon_complement.vertexes = poly_shrink.vertexes;
    polygon_complement.edges = poly_shrink.edges;

    /**********************************************************************************
     * complement edges step 3: remove redundancy_edge
    */ 
    if(polygon_complement.vertexes.size() == 1){
        return;
    }
    poly_shrink.vertexes = polygon_complement.vertexes;
    poly_shrink.edges = polygon_complement.edges;

    Polygon_shrink_redundancy_point(polygon_complement, poly_shrink);
    
    polygon_complement.vertexes = poly_shrink.vertexes;
    polygon_complement.edges = poly_shrink.edges;

}

#endif