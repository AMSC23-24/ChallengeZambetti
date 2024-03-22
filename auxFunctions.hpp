//Useful functions needed to implement the gradient descent method

#ifndef HH_AUXFUNCTIONS_HH
#define HH_AUXFUNCTIONS_HH

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "json.hpp"
#include "muParserXFun.hpp"
#include "parameters.hpp"

//FILES IN FONDO DA ELIMINARE!!!!!!



using Vector=std::vector<double>;
using json = nlohmann::json;


//calculates the norm of a Vector object
double norm (const Vector &v) {
    double res=0.;
    for(auto &i: v ){
        res += i*i;
    }
    return std::sqrt(res);
};

//prints the elements of Vector on the row
void print(const Vector &v) {
    for( auto &i: v)
       std::cout << i << " ";
    std::cout << "\n";
};

//overloading of - operator
//if the sizes of the two vectors don't coincide, the function returns the first vector passed
Vector operator-(const Vector &v1, const Vector &v2){
    Vector result(v1.size());
    if(v1.size()==v2.size())
      for(unsigned int i=0; i<v1.size(); ++i)
          result[i]= v1[i] - v2[i];
    return result;
};

//product of a Vector with a double
//double*Vector
Vector operator*(double alpha, const Vector &v) {
      std::vector<double> result=v;
      for(auto i: v)
          result[i] *= alpha;
      return result;
};




//computing the gradient of the function f given using finite differences
//method evaluates the gradient in a Vector x
Vector gradFiniteDiff ( muParserXFun f, const Vector &x)
{
    const double epsilon=1e-6;  // CAMBIARE!!!!!
    int n= x.size();
    Vector grad(n,0.0);    
    f=muParserXFun(n,f.getExpression());

    //using a centered scheme to compute each partial derivative 
    for(int i=0; i<n ; ++i){
        Vector x_plus=x;
        x_plus[i] += epsilon;
        Vector x_minus=x;
        x_minus[i] -= epsilon;
        grad[i]= (f(x_plus) - f(x_minus))/(2*epsilon);
    }
    
    return grad;
}


//Strategies to calculate step alphak at each iteration

enum strategies{
    exponential_decay, inverse_decay, Armijo_rule
};

double exp_decay(const int k,const double alpha, const double mu) {
    return alpha*std::exp(-mu*k);
};

double inv_decay(const int k, const double alpha, const double mu) {
    return alpha/(1+ mu*k);
};

/*double line_search(const double alpha, const Vector& xk, const double sigma, muParserXFun &f){  //, std::vector<muParserXFun> &df)  {   
    if(sigma>0 || sigma<0.5){
        Vector s(xk.size()), normk=s;
        
        for (unsigned int i=0; i<xk.size(); ++i){
            normk[i]= df[i](xk);
            s[i]=  xk[i] - alpha*normk[i];
        }

        
        if ( f(xk) - f(s) >= sigma*alpha*norm(normk)*norm(normk) )
          return alpha;
        else
            return line_search(alpha/2, xk, sigma, f);
    }
    std::cerr<< "Sigma has to be in the interval (0, 0.5)" << std::endl;
    return -1. ;
};*/

template<strategies S>
double compute_step(const double alpha, const int k, const Vector &xk, params_for_GD &g, muParserXFun & f){  //, std::vector<muParserXFun> &df)  {
    if constexpr(S==strategies::exponential_decay)
       return exp_decay(k, alpha, g.mu);
    if constexpr(S==strategies::inverse_decay)
       return inv_decay(k, alpha, g.mu);
    //return line_search(alpha,xk, g.sigma, f);  //, df);
};



#endif // HH_AUXFUNCTIONS_HH
