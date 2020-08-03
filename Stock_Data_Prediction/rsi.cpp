#include "rsi.h"

#include "OHLC.h"
#include <math.h>

RSI::RSI(std::vector<OHLC> ohlc_data)
{

    for(size_t i = 0; i < ohlc_data.size(); i++)
    {

        OHLC_Data.push_back(ohlc_data[i]);
        close_prices.push_back(ohlc_data[i].Get_Close_Price());

    }

}

void RSI::Add_OHLC(OHLC ohlc)
{

    OHLC_Data.push_back(ohlc);
    OHLC_Data.erase(OHLC_Data.begin());

    close_prices.push_back(ohlc.Get_Close_Price());
    close_prices.erase(close_prices.begin());


}

double RSI::Get_RSI()
{

    double sumGain = 0;
    double sumLoss = 0;
    for (int i = 1; i < close_prices.size(); i++)
    {
        double difference = close_prices[i] - close_prices[i - 1];
        if (difference >= 0)
        {
            sumGain += difference;
        }
        else
        {
            sumLoss -= difference;
        }
    }

    if (sumGain == 0) return 0;
    if (abs(sumLoss) < 0.5) return 100;

    double relativeStrength = sumGain / sumLoss;

    return 100.0 - (100.0 / (1 + relativeStrength));

}

double RSI::Get_New_RSI(OHLC new_value)
{

    Add_OHLC(new_value);

    return Get_RSI();


}
