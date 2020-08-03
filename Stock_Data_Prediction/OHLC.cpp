#include "OHLC.h"
#include <string>
#include <vector>
#include <bitset>
#include <type_traits>
#include <limits>

#include <iostream>
#include <math.h>

OHLC::OHLC(double open, double high, double low,
                       double close)
{

    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;

}

constexpr std::size_t ULONGLONG_BITS = std::numeric_limits<unsigned long long>::digits ;

template< std::size_t N1, std::size_t N2, std::size_t N3, std::size_t N4>
typename std::enable_if< ( (N1+N2+N3+N4) > ULONGLONG_BITS ), std::bitset<N1+N2+N3+N4> >::type
cat( const std::bitset<N1>& a, const std::bitset<N2>& b,
     const std::bitset<N3>& c, const std::bitset<N4>& d) { return std::bitset<N1+N2+N3+N4>( a.to_string() + b.to_string() + c.to_string() + d.to_string()) ; }



std::vector<char> OHLC::Get_Blob()
{

    std::bitset<64> open_price(open);
    std::bitset<64> high_price(high);
    std::bitset<64> low_price(low);
    std::bitset<64> close_price(close);

    std::bitset<256> data_blob = cat(open_price, high_price, low_price, close_price);

    std::vector<char> bits_to_return;

    for(int i = 0; i < 320; i++)
    {

        bits_to_return.push_back(data_blob[i]);

    }

    return bits_to_return;


}

std::vector<double> OHLC::Get_Difference(OHLC(stock_data_to_get_difference)) {

    std::vector<double> difference;

    difference.push_back(Get_Open_Price() - stock_data_to_get_difference.Get_Open_Price());
    difference.push_back(Get_High_Price() - stock_data_to_get_difference.Get_High_Price());
    difference.push_back(Get_Low_Price() - stock_data_to_get_difference.Get_Low_Price());
    difference.push_back(Get_Close_Price() - stock_data_to_get_difference.Get_Close_Price());

    return difference;

}

std::vector<double> OHLC::Get_Percentage_Difference(OHLC stock_data_to_get_difference)
{

    OHLC stock_data_2 = stock_data_to_get_difference;

    std::vector<double> percentage;

    percentage.push_back( ((Get_Open_Price() - stock_data_2.Get_Open_Price() ) / stock_data_2.Get_Open_Price()) * 100);
    percentage.push_back( ((Get_High_Price() - stock_data_2.Get_High_Price() ) / stock_data_2.Get_High_Price()) * 100);
    percentage.push_back( ((Get_Low_Price() - stock_data_2.Get_Low_Price() ) / stock_data_2.Get_Low_Price()) * 100);
    percentage.push_back( ((Get_Close_Price() - stock_data_2.Get_Close_Price() ) / stock_data_2.Get_Close_Price()) * 100);

    return percentage;


}

double OHLC::Get_Euclidean_Distance(OHLC stock_to_get_distance)
{

    double distance = sqrt( pow(Get_Open_Price() - stock_to_get_distance.Get_Open_Price(), 2) +
                            pow(Get_High_Price() - stock_to_get_distance.Get_High_Price(), 2) +
                            pow(Get_Low_Price() - stock_to_get_distance.Get_Low_Price(), 2) +
                            pow(Get_Close_Price() - stock_to_get_distance.Get_Close_Price(), 2));

    return distance;

}

double OHLC::Get_Manhattan_Distance(OHLC stock_to_get_distance)
{

    double distance = abs(Get_Open_Price() - stock_to_get_distance.Get_Open_Price()) +
                      abs(Get_High_Price() - stock_to_get_distance.Get_High_Price()) +
                      abs(Get_Low_Price() - stock_to_get_distance.Get_Low_Price()) +
                      abs(Get_Close_Price() - stock_to_get_distance.Get_Close_Price());

    return distance;

}

double OHLC::Get_Minkowski_Distance(OHLC stock_to_get_distance, double power)
{

    double distance = abs( pow(Get_Open_Price() - stock_to_get_distance.Get_Open_Price(), power) ) +
            abs(pow(Get_High_Price() - stock_to_get_distance.Get_High_Price(), power)) +
            abs(pow(Get_Low_Price() - stock_to_get_distance.Get_High_Price(), power)) +
            abs(pow(Get_Close_Price() - stock_to_get_distance.Get_High_Price(), power));

    distance = pow(distance, power);

}

double OHLC::Get_Open_Price()
{
    return open;
}

double OHLC::Get_High_Price()
{
    return high;
}

double OHLC::Get_Low_Price()
{
    return low;
}

double OHLC::Get_Close_Price()
{
    return close;
}
