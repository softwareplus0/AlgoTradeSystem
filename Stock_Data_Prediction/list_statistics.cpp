#include "list_statistics.h"

#include <vector>
#include <math.h>
#include <memory>
#include <stdlib.h>
#include <cmath>

List_Statistics::List_Statistics(std::vector<double> sample, double mean, double standard_deviation)
{

    this->sample = sample;
    this->mean = mean;
    this->standard_deviation = standard_deviation;

}

double List_Statistics::Get_Standard_Deviations_From_Mean()
{

    double sample_mean = Get_Average();
    double sample_standard_deviation = Get_Standard_Deviation();

    double standard_deviations_from_mean = (mean - sample_mean) / sample_standard_deviation;

    return standard_deviations_from_mean;


}

double List_Statistics::normal_pdf(double x, double m, double s)
{
    static const double inv_sqrt_2pi = 0.3989422804014327;
    double a = (x - m) / s;

    return inv_sqrt_2pi / s * std::exp(-0.5f * a * a);
}

double List_Statistics::normalCDF(double value)
{
    return 0.5 * erfc(-value * M_SQRT1_2);
}

std::vector<double> List_Statistics::Get_Within_Range(double standard_deviations)
{

    double standard_deviation = Get_Standard_Deviation();
    double mean = Get_Average();

    double min = -(standard_deviation * standard_deviations);
    double max = standard_deviation * standard_deviations;

    std::vector<double> numbers = filter(sample, min, max);

    return numbers;

}

double List_Statistics::Get_Standard_Deviation()
{

    double sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    for(i = 0; i < sample.size(); ++i)
    {
        sum += sample[i];
    }
    mean = sum/sample.size();
    for(i = 0; i < sample.size(); ++i) {

        standardDeviation += pow(sample[i] - mean, 2);

    }
    return sqrt(standardDeviation / sample.size());

}

std::vector<double> List_Statistics::Get_Outliers()
{

    if(sample.size() < 4) {
        return sample;
    }

    double values, q1, q3, iqr, maxValue, minValue;


    if((sample.size() / 4) % 1 == 0){//find quartiles
        q1 = 1/2.0 * (sample[(sample.size() / 4.0)] + sample[(sample.size() / 4.0) + 1]);
        q3 = 1/2.0 * (sample[(sample.size() * (3 / 4.0))] + sample[(sample.size() * (3 / 4.0)) + 1]);
    } else {
        q1 = sample[floor(sample.size() / 4.0 + 1)];
        q3 = sample[ceil(sample.size() * (3 / 4.0) + 1)];
    }

    iqr = q3 - q1;
    maxValue = q3 + iqr * 1.5;
    minValue = q1 - iqr * 1.5;

    return filter(sample, minValue, maxValue);


}

std::vector<double> List_Statistics::filter(std::vector<double> vector_to_filter, double minValue, double maxValue)
{

    std::vector<double> vector_to_return;

    for(size_t i = 0; i < vector_to_filter.size(); i++)
    {

        if(vector_to_filter[i] >= minValue && vector_to_filter[i] <= maxValue)
        {

            vector_to_return.push_back(vector_to_filter[i]);

        }

    }

    return vector_to_return;

}

double List_Statistics::Get_Average()
{

    double average = 0;

    for(size_t i = 0; i < sample.size(); i++)
    {

        average += sample[i];

    }

    average = average / sample.size();

    return average;

}

double List_Statistics::Get_Average(std::vector<double> vector_to_get_average)
{

    double average = 0;

    for(size_t i = 0; i < vector_to_get_average.size(); i++)
    {

        average += vector_to_get_average[i];

    }

    average = average / vector_to_get_average.size();

    return average;

}
