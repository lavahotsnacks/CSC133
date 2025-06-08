/*
 * File:   RandomGenExpoMean.h
 * Author: jojo
 *
 * Created on July 22, 2012, 2:30 PM
 */

#ifndef RANDOMGENEXPOMEAN_H
#define RANDOMGENEXPOMEAN_H
#include <cmath>
#include <cstdlib>

class RandomExpoMean {
public:
    RandomExpoMean(double mean);
    void setMean(double mean);
    double getValue();
    double getAverageMean() const { return average / count; }

private:
    double randomExpoMean;
    double Globalmean;
    double average;
    int count;
    void generate();
};

#endif /* RANDOMGENEXPOMEAN_H */