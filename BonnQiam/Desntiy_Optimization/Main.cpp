#include <fstream>
#include <sstream>
#include <iostream>

#include <cmath>

#include <vector>

//contest configure: the window size is 20 microns, the gdsii unit si 1 nm, i.e., 0.001 micron
#define x_grid_num 8
#define y_grid_num 20
#define grid_size  20000

#define alpha_varation  0.2
#define beta_varation   0.077
#define alpha_line      0.2
#define beta_line       11.758
#define alpha_outlier   0.15
#define beta_outlier    0.014

struct Data {
    int x;
    int y;
    double val1;
    double val2;
};

struct Grid{
    double Coor_x;
    double Coor_y;
    double Density_no_fill;
    double Density_fillable;
    double Density_filled;
};

std::vector<Data> parse_No_Fill_Density(const std::string& filename);
std::vector<double> parse_Grid(const std::string& filename);
double Density_score(const std::vector<Grid>& grid);
double Score_function(double beta, double metric);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    /**********************************
     * 
     * Read sM1.txt and grid.txt
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
        grid.push_back(g);
    }

    double Initial_score = Density_score(grid);

    std::cout << "The initial score is " << Initial_score << std::endl;

    return 0;
}


std::vector<Data> parse_No_Fill_Density(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << '\n';
        exit(1);
    }

    std::string line;
    std::vector<Data> data;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Data d;
        if (!(iss >> d.x >> d.y >> d.val1 >> d.val2)) { 
            break; // Error
        }
        data.push_back(d);
    }

    return data;
}

std::vector<double> parse_Grid(const std::string& filename){
    std::ifstream file(filename);
    std::string line;
    std::vector<double> gridValues;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line == "<grid>") {
                getline(file, line);
                gridValues.push_back(std::stod(line));
            }
        }
        file.close();
    } else {
        std::cout << "Unable to open file";
    }

    return gridValues;
}

double Density_score(const std::vector<Grid>& grid){
    // standard score
    double sum = 0;
    for (int i = 0; i < grid.size(); i++) {
        sum += grid[i].Density_filled;
    }
    double mean = sum / grid.size();
    double std  = 0;
    for (int i = 0; i < grid.size(); i++) {
        std += (grid[i].Density_filled - mean) * (grid[i].Density_filled - mean);
    }
    std = sqrt(std / grid.size());
    double std_score = Score_function(beta_varation, std);

    // outlier score
    double outlier = 0;
    for (int i = 0; i < grid.size(); i++) {
        double delta = abs(grid[i].Density_filled-mean) - 3*std;
        if(delta >= 0)
            outlier += delta;
    }
    double outlier_score = Score_function(beta_outlier, outlier);

    // line score
    double line = 0;
    for(int i = 0; i < x_grid_num; i++){
        double sum_line = 0;
        for(int j = 0; j < y_grid_num; j++){
            sum_line += grid[i*y_grid_num+j].Density_filled;
        }
        double mean_line = sum_line / y_grid_num;

        for(int j = 0; j < y_grid_num; j++){
            line += abs((grid[i*y_grid_num+j].Density_filled - mean));
        }
    }
    double line_score = Score_function(beta_line, line);

    return alpha_varation*std_score + alpha_outlier*outlier_score + alpha_line*line_score;
}

double Score_function(double beta, double metric){
    return ((1.0-metric/beta) >= 0) ? (1.0-metric/beta) : 0;
}