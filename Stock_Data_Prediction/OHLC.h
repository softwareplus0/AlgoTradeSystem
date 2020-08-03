#ifndef STOCK_DATA_H
#define STOCK_DATA_H

#include <string>
#include <vector>


class OHLC
{
public:

    OHLC(double open, double high, double low, double close);

    std::vector<double> Get_Difference(OHLC(stock_data_to_get_difference));
    std::vector<double> Get_Percentage_Difference(OHLC stock_data_to_get_difference);
    double Get_Euclidean_Distance(OHLC stock_to_get_distance);
    double Get_Manhattan_Distance(OHLC stock_to_get_distance);

    std::string Get_Date();
    double Get_Open_Price();
    double Get_High_Price();
    double Get_Low_Price();
    double Get_Close_Price();
    double Get_Adj_Close();
    std::int64_t Get_Volume();
    std::vector<char> Get_Blob();
    double Get_Minkowski_Distance(OHLC stock_to_get_distance, double power);

private:

    std::string date = "";
    double open;
    double high;
    double low;
    double close;
    double adj_close;
    uint64_t volume;


};

#endif // STOCK_DATA_H
