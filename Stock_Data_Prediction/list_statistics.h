#ifndef LIST_STATISTICS_H
#define LIST_STATISTICS_H

#include <vector>

class List_Statistics
{
public:
    List_Statistics(std::vector<double> sample, double sample_mean, double sample_standard_deviation);
    double Get_Standard_Deviations_From_Mean();

private:

    std::vector<double> sample;
    std::vector<double> filter(std::vector<double> vector_to_filter, double minValue, double maxValue);
    std::vector<double> Get_Within_Percentage_Of_Mean(double percentage);
    std::vector<double> Get_Within_Range(double standard_deviations);
    std::vector<double> Get_Outliers();
    double Get_Average(std::vector<double> vector_to_get_average);
    double Get_Average();
    double Get_Standard_Deviation();

    double mean;
    double standard_deviation;
    double normal_pdf(double x, double m, double s);
    double normalCDF(double value);
};

#endif // LIST_STATISTICS_H
