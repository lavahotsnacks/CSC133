#include "RandomGenExpoMean.h"

void RandomExpoMean::generate() {
    double uniform = rand() % int(60 * Globalmean - 1) + 1;
    randomExpoMean = -1 * log(1 - (uniform / (60.0 * Globalmean))) * Globalmean * 60;
}

RandomExpoMean::RandomExpoMean(double mean) {
    Globalmean = mean;
    count = 0;
    average = 0;
}

void RandomExpoMean::setMean(double mean) {
    Globalmean = mean;
}

double RandomExpoMean::getValue() {
    count++;
    generate();
    average += randomExpoMean;
    return randomExpoMean;
}