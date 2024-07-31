#include "util.hpp"
#include "Fast_Median.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    /**********************************
     * 
     * Read sM1.txt ('Benchmark' file) and grid.txt ('Grid' file)
     * establish the grid structure
     * 
     * *******************************/

    std::vector<Data> data = parse_No_Fill_Density(argv[1]);
    std::vector<double> gridValues = parse_Grid(argv[2]);

#if 0
    // Print the data to verify it was loaded correctly
    for (const auto& d : data) {
        std::cout << "x: " << d.x << ", y: " << d.y << ", val1: " << d.val1 << ", val2: " << d.val2 << '\n';
    }

    // Print the grid density values
    for (double value : gridValues) {
        std::cout << "The fillable density is " << value << std::endl;
    }

    // chekc the size
    std::cout << "The size of the data is " << data.size() << std::endl;
    std::cout << "The size of the grid is " << gridValues.size() << std::endl;
#endif

    std::vector<Grid> grid;
    for (int i = 0; i < data.size(); i++) {
        Grid g;
        g.Coor_x            = data[i].x;
        g.Coor_y            = data[i].y;
        g.Density_no_fill   = data[i].val1;
        g.Density_fillable  = gridValues[i];
        g.Density_filled    = g.Density_no_fill;
        g.Density_insert    = 0.0;
        grid.push_back(g);
    }

    double Initial_score = Density_score(grid);

    std::cout << "The initial score is " << Initial_score << std::endl;

    Fast_Median<double>(grid);
    
    return 0;
}


