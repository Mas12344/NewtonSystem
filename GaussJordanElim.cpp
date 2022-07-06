#include "GaussJordanElim.h"
using namespace interval_arithmetic;


void ld::swapRows(std::vector<std::vector<long double>>* x, int first, int second){
    std::vector<std::vector<long double>>& ArrayRef = *x;

    std::vector<long double> tmp = ArrayRef.at(first);
    ArrayRef[first] = ArrayRef.at(second);
    ArrayRef[second] = tmp;

}

void ld::divideRow(std::vector<std::vector<long double>>* x, int index, long double val){
    std::vector<std::vector<long double>>& ArrayRef = *x;

    for (int number = 0; number < ArrayRef[index].size(); number++) {
        ArrayRef[index][number] /= val;
    }
}

// second += first * mult;
void ld::addMultipliedRow(std::vector<std::vector<long double>>* x, int first, int second, long double mult) {
    std::vector<std::vector<long double>>& ArrayRef = *x;

    for (int number = 0; number < ArrayRef[second].size(); number++) {
        ArrayRef[second][number] += ArrayRef[first][number] * mult;
    }
}

int ld::vectoredGaussJordan(std::vector<std::vector<long double>>* x, std::vector<long double>* res) {
    std::vector<std::vector<long double>>& ArrayRef = *x;
    int n = ArrayRef.size();

    for (int column = 0; column < n; column++) {
        int c = column + 1;
        while (ArrayRef[column][column] == 0) {
            if (c == n) {
                return 2;
            }
            swapRows(x, column, c++);
        }

        divideRow(x, column, ArrayRef[column][column]);
        for (int otherColumn = 0; otherColumn < n; otherColumn++) {
            if (otherColumn == column) {
                continue;
            }
            addMultipliedRow(x, column, otherColumn, -ArrayRef[otherColumn][column]);
        }

    }

    for (int v = 0; v < n; v++) {
        (*res)[v] = ArrayRef[v][n];
    }

    return 0;

}



void iv::swapRows(std::vector<std::vector<Interval<long double>>>* x, int first, int second){
    std::vector<std::vector<Interval<long double>>>& ArrayRef = *x;

    std::vector<Interval<long double>> tmp = ArrayRef.at(first);
    ArrayRef[first] = ArrayRef.at(second);
    ArrayRef[second] = tmp;

}

void iv::divideRow(std::vector<std::vector<Interval<long double>>>* x, int index, Interval<long double> val){
    std::vector<std::vector<Interval<long double>>>& ArrayRef = *x;

    for (int number = 0; number < ArrayRef[index].size(); number++) {
        ArrayRef[index][number] = IDiv(ArrayRef[index][number], val);
    }
}

// second += first * mult;
void iv::addMultipliedRow(std::vector<std::vector<Interval<long double>>>* x, int first, int second, Interval<long double> mult) {
    std::vector<std::vector<Interval<long double>>>& ArrayRef = *x;

    for (int number = 0; number < ArrayRef[second].size(); number++) {
        ArrayRef[second][number] = IAdd(ArrayRef[second][number], IMul(ArrayRef[first][number], mult));
    }
}

int iv::vectoredGaussJordan(std::vector<std::vector<Interval<long double>>>* x, std::vector<Interval<long double>>* res) {
    std::vector<std::vector<Interval<long double>>>& ArrayRef = *x;
    int n = ArrayRef.size();

    for (int column = 0; column < n; column++) {
        int c = column + 1;
        Interval<long double> y = ArrayRef[column][column];
        while ((y.a <= 0) && (y.b >= 0)){
            if (c == n) {
                return 2;
            }
            swapRows(x, column, c++);
        }

        divideRow(x, column, ArrayRef[column][column]);
        for (int otherColumn = 0; otherColumn < n; otherColumn++) {
            if (otherColumn == column) {
                continue;
            }
            addMultipliedRow(x, column, otherColumn, ArrayRef[otherColumn][column].Opposite());
        }

    }

    for (int v = 0; v < n; v++) {
        (*res)[v] = ArrayRef[v][n];
    }

    return 0;

}