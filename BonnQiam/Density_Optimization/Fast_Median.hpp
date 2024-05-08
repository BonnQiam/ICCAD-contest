#ifndef FAST_MEDIAN
#define FAST_MEDIAN

#include "util.hpp"
#include <algorithm>

template <typename T>
T find_Median(std::vector<T>& vec) {
    std::sort(vec.begin(), vec.end());
    return vec[vec.size() / 2];
}

template <typename T>
void Fast_Median(std::vector<Grid>& grid){
    std::vector<T> density_no_fill_and_filled;
    for (int i = 0; i < grid.size(); i++) {
        density_no_fill_and_filled.push_back(grid[i].Density_no_fill);
        density_no_fill_and_filled.push_back(grid[i].Density_no_fill + grid[i].Density_fillable);
    }
    T median = find_Median(density_no_fill_and_filled);

    std::cout << "The median is " << median << std::endl;

    // update the grid
    for (int i = 0; i < grid.size(); i++) {
        if(median < grid[i].Density_no_fill)
            grid[i].Density_insert = 0.0;
        else if(median > (grid[i].Density_no_fill + grid[i].Density_fillable)){
            grid[i].Density_insert = grid[i].Density_fillable;
            grid[i].Density_filled += grid[i].Density_insert;
        }
        else{
            grid[i].Density_insert = median - grid[i].Density_no_fill;
            grid[i].Density_filled += grid[i].Density_insert;
        }
    }

    // calculate the score
    double score = Density_score(grid);
    std::cout << "The score after optimization is " << score << std::endl;

    // Output the file
    std::ofstream file("Grid_opt.txt");
    for (int i = 0; i < grid.size(); i++) {
        file << grid[i].Coor_x << " " << grid[i].Coor_y << " " << grid[i].Density_filled << std::endl;
    }
    file.close();
}

#endif