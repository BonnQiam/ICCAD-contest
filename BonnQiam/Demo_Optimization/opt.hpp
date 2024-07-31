#include <vector>
#include <cppad/ipopt/solve.hpp>

//! Attention: solve_callback.hpp in /usr/local/include/cppad/ipopt has been modified by QIAMKING
//? Original solve_callback.hpp is <cppad/ipopt/solve_callback.hpp>


// factor 4 s
#define alpha_std       0.2
#define beta_std        0.077
#define alpha_line      0.2
#define beta_line       11.758
#define alpha_outlier   0.15
#define beta_outlier    0.014

using CppAD::AD;

// 定义 max_func 函数

AD<double> max_func(AD<double> x, AD<double> y, AD<double> z) {
    //using logsumexp
    return CppAD::log(CppAD::exp(x) + CppAD::exp(y))*z;
};

//AD<double> max_func(AD<double> x, AD<double> y, AD<double> z) {
//    return CppAD::CondExpGt(x, y, x, y)*z;
//};

// Cost 函数定义
class FG_eval{
    private:
        std::vector<double> Density_Mental;
        AD<double> Std1, Std2, Std3 = 0.0;
        AD<double> Mean1, Mean2, Mean3 = 0.0;
    public:
        typedef CPPAD_TESTVECTOR(AD<double>)    ADvector;

        FG_eval(std::vector<double> Density_Mental){
            this->Density_Mental = Density_Mental;
        };

        void operator()(ADvector& fg, const ADvector& x){
            fg[0] = -1.0*Std(x)-1.0*Outlier(x);
            fg[1] = 0.0;
        };

        AD<double> Std(const ADvector & x){
            AD<double> Sum1, Sum2, Sum3 = 0.0;
            for(int i=0; i < (x.size()/3); i++){
                Sum1 += x[i] + Density_Mental[i];
                Sum2 += x[i + (x.size()/3)] + Density_Mental[i + (x.size()/3)];
                Sum3 += x[i + 2*(x.size()/3)] + Density_Mental[i + 2*(x.size()/3)];
            }

            Mean1 = Sum1/(x.size()/3);
            Mean2 = Sum2/(x.size()/3);
            Mean3 = Sum3/(x.size()/3);

            for(int i=0; i < (x.size()/3); i++){
                Std1 += (x[i] + Density_Mental[i] - Mean1)*(x[i] + Density_Mental[i] - Mean1);
                Std2 += (x[i + (x.size()/3)] + Density_Mental[i + (x.size()/3)] - Mean2)*(x[i + (x.size()/3)] + Density_Mental[i + (x.size()/3)] - Mean2);
                Std3 += (x[i + 2*(x.size()/3)] + Density_Mental[i + 2*(x.size()/3)] - Mean3)*(x[i + 2*(x.size()/3)] + Density_Mental[i + 2*(x.size()/3)] - Mean3);
            }
            Std1 = sqrt(Std1/(x.size()/3));
            Std2 = sqrt(Std2/(x.size()/3));
            Std3 = sqrt(Std3/(x.size()/3));
            
            return max_func(0., 1.0-Std1/beta_std, alpha_std)
                    +max_func(0., 1.0-Std2/beta_std, alpha_std)
                    +max_func(0., 1.0-Std3/beta_std, alpha_std);
        };


        AD<double> Outlier(const ADvector & x){
            AD<double> Outlier1,Outlier2, Outlier3 = 0.0;

            for(int i=0; i < (x.size()/3); i++){
                Outlier1 += max_func(0., fabs(x[i] - Mean1),1.0);
                Outlier2 += max_func(0., fabs(x[i + (x.size()/3)] - Mean2),1.0);
                Outlier3 += max_func(0., fabs(x[i + 2*(x.size()/3)] - Mean3),1.0);
            }

            return max_func(0., 1.0-Outlier1/beta_outlier, alpha_outlier)
                    +max_func(0., 1.0-Outlier2/beta_outlier, alpha_outlier)
                    +max_func(0., 1.0-Outlier3/beta_outlier, alpha_outlier);
        };
};

