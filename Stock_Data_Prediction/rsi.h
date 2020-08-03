#ifndef RSI_H
#define RSI_H

#include "OHLC.h"


class RSI
{
public:
    RSI(std::vector<OHLC> ohlc_data);

    double Get_RSI();
    double Get_New_RSI(OHLC new_value);
    void Add_OHLC(OHLC ohlc);
    double Get_New_RSI(double new_value);

private:


    std::vector<OHLC> OHLC_Data;
    std::vector<double> close_prices;

};

#endif // RSI_H
