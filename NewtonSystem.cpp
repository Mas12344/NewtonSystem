#include "NewtonSystem.h"

using namespace interval_arithmetic;

bool ld::isCloseEnough(std::vector<long double>* x, std::vector<long double>* old_x, long double eps){
    bool close = true;
    for (int i = 0; i < x->size(); i++) {
        long double nom = fabsl(x->at(i) - old_x->at(i));
        long double den = std::max(fabsl(x->at(i)), fabsl(old_x->at(i)));
        close = (nom / den) < eps;
        if (!close)
            break;
    }
    return close;
}

int ld::NewtonSystem(uint16_t  n,
		std::vector<long double>* x,
		std::function<long double(uint16_t i, uint16_t n, std::vector<long double>* x)> f,
		std::function<void(uint16_t i, uint16_t n, std::vector<long double>* x, std::vector<long double>* out)> df,
		uint16_t                mit,
		long double             eps,
		uint16_t*                it)
        {
		// x_k+1  =  x_k - J(x_k)^-1 * F(x_k)
		// (x_k+1 - x_k) * J(x_k) = -F(x_k)
		// gauss-jordan
		// x_k+1 - x_k = result
		// x_k+1 = result + x_k

		std::vector<std::vector<long double>> gauss;
		gauss.resize(n);
		for (int i = 0; i < n; i++) {
			gauss[i].resize(n + 1);
		}

		std::vector<long double> result;
		result.resize(n);

		std::vector<long double> old_x;
		old_x.resize(n);

		int flag;

		int my_mit = 0;

		while (!isCloseEnough(x, &old_x, eps) && (++my_mit < mit))
		{
			for (int i = 0; i < n; i++) {
				df(i, n, x, &gauss[i]);
				gauss[i][n] = -f(i, n, x);
			}
			

			flag = ld::vectoredGaussJordan(&gauss, &result);
			if (flag != 0) {
				return flag;
			}
			else{
				for (int i = 0; i < n; i++) {
					old_x[i] = x->at(i);
					(*x)[i] += result[i];
				}
			}
			

			
		}

		*it = my_mit;

		return flag;

	}

bool iv::isCloseEnough(std::vector<Interval<long double>>* x, std::vector<Interval<long double>>* old_x, Interval<long double> eps) {
	bool close = true;
    for (int i = 0; i < x->size(); i++) {
        Interval<long double> nom = IAbs(x->at(i) - old_x->at(i));
        Interval<long double> nv = IAbs(x->at(i));
        Interval<long double> ov = IAbs(old_x->at(i));
        Interval<long double> den;
        if (nv.b < ov.a){
            den = Interval<long double>(ov);
        } else {
            den = Interval<long double>(nv);
        }
        close = (nom / den).b < eps.a;
        if (!close)
            break;
    }
    return close;
}

int iv::NewtonSystem(uint16_t  n,
		std::vector<Interval<long double>>* x,
		std::function<Interval<long double>(uint16_t i, uint16_t n, std::vector<Interval<long double>>* x)> f,
		std::function<void(uint16_t i, uint16_t n, std::vector<Interval<long double>>* x, std::vector<Interval<long double>>* out)> df,
		uint16_t                          mit,
		Interval<long double>             eps,
		uint16_t*                          it)
	{
		// x_k+1  =  x_k - J(x_k)^-1 * F(x_k)
		// (x_k+1 - x_k) * J(x_k) = -F(x_k)
		// gauss-jordan
		// x_k+1 - x_k = result
		// x_k+1 = result + x_k
		if (n < 1 || mit < 1){
			return 1;
		}

		std::vector<std::vector<Interval<long double>>> gauss;
		gauss.resize(n);
		for (int i = 0; i < n; i++) {
			gauss[i].resize(n + 1);
		}

		std::vector<Interval<long double>> result;
		result.resize(n);

		std::vector<Interval<long double>> old_x;
		old_x.resize(n);

		int flag;

		int my_mit = 0;

		bool isNotClose = true;
		
		while (isNotClose)
		{
			
			isNotClose = !isCloseEnough(x, &old_x, eps) && (++my_mit < mit);			
			
			for (int i = 0; i < n; i++) {

				df(i, n, x, &gauss[i]);
				
				gauss[i][n] = f(i, n, x).Opposite();
			}
			

			flag = iv::vectoredGaussJordan(&gauss, &result);

			if (my_mit >= mit) flag = 3;
			
			if(flag == 2){
				return flag;
			}
			else{
				for (int i = 0; i < n; i++) {
					old_x[i] = x->at(i);
					(*x)[i] = IAdd((*x)[i], result[i]);
				}
			}
			

			
		}
		
		

		*it = my_mit;

		return flag;

	}