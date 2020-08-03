#include <QCoreApplication>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>

#include <bitset>

#include "OHLC.h"
#include "csv_parser.h"
#include "bank_account.h"
#include "rsi.h"

#include <opencv4/opencv2/ml.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

double Percentage_Change(double first_number, double second_number);
double Get_Total_Percentage_Change_In_Stock(std::string directory);
double Get_Sharpe_Ratio(double portfolio_return, double stock_return, double standard_deviation_of_portfolio_excess_return);
std::vector<std::string> split (const std::string &s, char delim);
std::string Get_File_Name_From_Path(const std::string & s);
double Get_Standard_Deviation(std::vector<double> sample);
std::vector<int> Get_Labels(std::vector<OHLC> stock_data);
std::vector<double> Get_Distance(std::vector<OHLC> stock_data);
std::vector<std::string> To_SVMLight_Format(std::vector<std::vector<double>> data, std::vector<int> labels);
std::vector<std::vector<double>> Get_Euclidean_Distances(std::vector<OHLC> stock_data);
std::vector<std::vector<double>> Get_Manhattan_Distances(std::vector<OHLC> stock_data);
std::vector<std::vector<double>> Get_Minkowski_Distances(std::vector<OHLC> stock_data, double power);
int Run_Simulation(cv::Ptr<cv::ml::SVM> svm, std::vector<OHLC> stock_data_to_test, int starting_balance, bool & buy_stocks);
std::vector<std::vector<double>> Get_Features(std::vector<OHLC> stock_data);
std::vector<std::vector<double>> Get_Percentage_Changes_In_Rows(std::vector<std::vector<double>> stock_data);
cv::Ptr<cv::ml::SVM> Train_SVM(std::vector<OHLC> stock_data);
std::vector<std::vector<double>> Get_Percentage_Changes_In_Rows(std::vector<std::vector<double>> stock_data);
std::vector<std::vector<double>> Get_Percentage(std::vector<OHLC> stock_data, int stocks_to_get_percentage_of);
std::vector<std::vector<double>> Get_Difference(std::vector<OHLC> stock_data);
std::vector<int> Get_Labels(std::vector<std::vector<double>> training_mat);
OHLC String_Vector_To_Stock_Data(std::vector<std::string> string_vector);
std::vector<OHLC> CSV_To_OHLC_Vector(std::string file_dir);
void Print_Char_Array(std::vector<char> char_vector_to_print);

template<typename _Tp> static  cv::Mat toMat(const std::vector<std::vector<_Tp> > vecIn) {
    cv::Mat_<_Tp> matOut(vecIn.size(), vecIn.at(0).size());
    for (int i = 0; i < matOut.rows; ++i) {
        for (int j = 0; j < matOut.cols; ++j) {
            matOut(i, j) = vecIn.at(i).at(j);
        }
    }
    return matOut;
}

const int WINDOW_INCREMENT = 1;
const int STOCKS_TO_GET_DISTANCE_OF = 2;
const int STARTING_BALANCE = 100000;

int main()
{



    std::vector<std::string> train_files;
    cv::glob("[TRAIN DATA FILE PATH]", train_files);

    std::vector<std::string> test_files;
    cv::glob("[TEST DATA FILE PATH]", test_files);

    if(train_files.size() != test_files.size())
    {
        std::cout << "Error." << std::endl;
        exit(0);
    }

    std::vector<std::pair<std::string, int>> balances_pair;
    std::vector<double> excess_returns;

    double beat_market = 0;

    double total_simulation_percentage = 0;
    double total_percentage_change_in_stock = 0;
    double total_performance = 0;

    for(size_t i = 0; i < train_files.size(); i++) {



        double current_percentage_change_in_stock = Get_Total_Percentage_Change_In_Stock(test_files[i]);

        std::vector<OHLC> stock_data = CSV_To_OHLC_Vector(train_files[i]);
        cv::Ptr<cv::ml::SVM> svm = Train_SVM(stock_data);
        std::vector<OHLC> stock_data_test = CSV_To_OHLC_Vector(test_files[i]);

        std::string current_file = test_files[i];

        bool was_in_market = false;

        int balance = Run_Simulation(svm, stock_data_test, STARTING_BALANCE, was_in_market);

        double current_simulation_percentage = Percentage_Change(STARTING_BALANCE, balance);


        if(was_in_market) {

            total_simulation_percentage += current_simulation_percentage;
            total_percentage_change_in_stock += current_percentage_change_in_stock;

            double excess_return = current_simulation_percentage - current_percentage_change_in_stock;

            excess_returns.push_back(excess_return);

            std::cout << "Stock: " << current_file << std::endl;
            std::cout << "Total percentage change in stock: " << current_percentage_change_in_stock << std::endl;
            std::cout << "Simulation percentage: " << current_simulation_percentage << "\n" << std::endl;
            std::cout << "Balance: " << balance << std::endl;
            std::cout << "Total percentage change: " << excess_return << "\n\n" << std::endl;
            total_performance += excess_return;

            balances_pair.push_back(std::pair<std::string, int>(current_file, balance));

            if(current_simulation_percentage > current_percentage_change_in_stock) {
                beat_market++;
            }

        }

    }

    double excess_returns_standard_deviation = Get_Standard_Deviation(excess_returns);
    double sharpe_ratio = Get_Sharpe_Ratio(total_simulation_percentage, total_percentage_change_in_stock, excess_returns_standard_deviation);

    int more_than_balance = 0;
    int equal_to_balance = 0;
    int less_than_balance = 0;

    for(size_t balances_index = 0; balances_index < balances_pair.size(); balances_index++)
    {

        if(balances_pair[balances_index].second > STARTING_BALANCE)
        {
            more_than_balance++;
        } else if(balances_pair[balances_index].second == STARTING_BALANCE) {
            equal_to_balance++;
        } else if (balances_pair[balances_index].second < STARTING_BALANCE) {
            less_than_balance++;
        }

    }

    std::cout << "Beat market: " << beat_market << std::endl;
    std::cout << "More than balance: " << more_than_balance << std::endl;
    std::cout << "Equal to balance: " << equal_to_balance << std::endl;
    std::cout << "Less than balance: " << less_than_balance << std::endl;
    std::cout << "Total performance compared to market: " << total_performance << std::endl;
    std::cout << "Sharpe Ratio: " << sharpe_ratio << std::endl;

}

/**
 * @brief Get_Sharpe_Ratio
 * This program calculates the sharpe ratio to evaluate
 * the performance of an investing strategy.
 * @param portfolio_return
 * The total return of a investing strategy.
 * @param stock_return
 * The total return of the market.
 * @param standard_deviation_of_portfolio_excess_return
 * The performance of an investing strategy relative to the market.
 * @return
 * The sharpe ratio of the given data points.
 */

double Get_Sharpe_Ratio(double portfolio_return, double stock_return, double standard_deviation_of_portfolio_excess_return)
{

    double sharpe_ratio = (portfolio_return - stock_return) / standard_deviation_of_portfolio_excess_return;

    return sharpe_ratio;

}

/**
 * @brief Run_Simulation
 * This function runs a simulation on a CSV file containing
 * stock data in OHLC format.
 * @param svm
 * The svm that will be used to run the simulation.
 * @param stock_data_to_test
 * The CSV file containing stock data in OHLC format.
 * @param starting_balance
 * The balance that the simulation will use.
 * @param buy_stocks
 * A bool value that is passed by reference that will be
 * set to true if at any point the simulation buys stocks.
 * @return
 * An integer representing the amount of money left after the
 * simulation.
 */

int Run_Simulation(cv::Ptr<cv::ml::SVM> svm, std::vector<OHLC> stock_data_to_test, int starting_balance, bool & buy_stocks)
{

    std::vector<std::vector<double>> euclidean_distances = Get_Features(stock_data_to_test);

    bool in_market = false;
    Bank_Account bank_account(starting_balance);
    int withdraw_amount = 0;
    int deposit_amount = 0;
    int current_shares = 0;
    int last_prediction = 0;

    for(size_t i = 0; i < euclidean_distances.size(); i++)
    {
        cv::Mat current_distance(euclidean_distances[i]);
        current_distance.convertTo(current_distance, CV_32F);
        current_distance = current_distance.reshape(1, 1);

        int prediction = svm->predict(current_distance);

        if(!in_market && last_prediction == 1) {

            //buy

            buy_stocks = true;
            double current_open_price = stock_data_to_test[i].Get_Open_Price();
            current_shares = bank_account.getCurrentBalance() / current_open_price;
            current_shares = current_shares - 5;

            //            printf("\nBUY %d shares at %lf\n", current_shares, current_open_price);

            withdraw_amount = current_shares * current_open_price;

            bank_account.withdraw(withdraw_amount);
            bank_account.withdraw(5);
            in_market = true;

        } else if(in_market && last_prediction == 0) {

            double current_open_price = stock_data_to_test[i].Get_Open_Price();

            //            printf("\nSELL %d shares at %lf\n", current_shares, current_open_price);

            deposit_amount = current_shares * current_open_price;
            current_shares = 0;
            bank_account.deposit(deposit_amount);
            bank_account.withdraw(5);
            in_market = false;

            //sell
        }


        last_prediction = prediction;

        //        printf("Open: %f Close: %f Prediction: %d\n", stock_data_to_test[i].Get_Open_Price(), stock_data_to_test[i].Get_Close_Price(), prediction);



    }

    int current_value_in_market = 0;

    if(in_market) {

        double current_open_price = stock_data_to_test[stock_data_to_test.size() - 1].Get_Open_Price();
        current_value_in_market = current_shares * current_open_price;
        //        printf("\n\nIn market: %d", current_value_in_market);

    }

    //    printf("\n\nTotal: %d\n\n", bank_account.getCurrentBalance() + current_value_in_market);

    return bank_account.getCurrentBalance() + current_value_in_market;

}

/**
 * @brief Train_SVM
 * This function trains an SVM given a vector
 * of OHLC data. The SVM parameters are automatically
 * set using trainAuto(cv::ml::TrainData).
 * @param stock_data
 * The vector of OHLC data to train the SVM with.
 * @return
 * A trained SVM.
 */

cv::Ptr<cv::ml::SVM> Train_SVM(std::vector<OHLC> stock_data)
{

    std::vector<int> labels = Get_Labels(stock_data);
    std::vector<std::vector<double>> distance = Get_Features(stock_data);

    cv::Mat percentages_image = toMat(distance);
    cv::Mat labels_image(labels);

    percentages_image.convertTo(percentages_image, CV_32F);
    labels_image.convertTo(labels_image, CV_32S);



    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(percentages_image, cv::ml::SampleTypes::ROW_SAMPLE, labels_image);


    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
//    svm->setGamma(1);
//    svm->setDegree(1);
    svm->train(tData); //can also use trainAuto();

    return svm;

}

std::vector<std::string> To_SVMLight_Format(std::vector<std::vector<double>> data, std::vector<int> labels)
{

    std::vector<std::string> string_vector;

    for(size_t i = 0; i < data.size(); i++)
    {


        std::string current_row = "";

        int label = labels[i];
        std::string string_label = "";

        if(label == 0) {
            string_label = "-1";
        } else if(label == 1) {
            string_label = "1";
        }

        current_row.append(string_label + " ");

        for(size_t j = 0; j < data[0].size(); j++)
        {


            current_row.append(std::to_string(j) + ":");
            current_row.append(std::to_string(data[i][j]));

            if(j < data[0].size() - 1)
            {
                current_row.append(" ");
            }

        }

        string_vector.push_back(current_row);

    }

    return string_vector;


}

double Get_Standard_Deviation(std::vector<double> sample)
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

/**
 * @brief split
 * This function splits a string into a vector
 * of strings. The string is split wherever a
 * given char is.
 * @param s
 * The string to split.
 * @param delim
 * The character to split the string with.
 * @return
 * A vector of strings that were split where the character
 * delim exists.
 */

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

/**
 * @brief Percentage_Change
 * This function gets the percentage change between
 * two doubles.
 * @param first_number
 * The number that the percentage will be based on
 * with second_number.
 * @param second_number
 * The number that the percentage will be found out of
 * first_number.
 * @return
 * The percentage of first_number and second_number.
 */

double Percentage_Change(double first_number, double second_number)
{
    double result = 0;
    result = ((second_number - first_number) * 100) / first_number;

    return result;
}

/**
 * @brief Get_Total_Percentage_Change_In_Stock
 * This function gets the total percentage change of a stock.
 * @param directory
 * The path of a file that containsn the stock data in OHLC format.
 * @return
 * The percentage change of the stock data in directory.
 */

double Get_Total_Percentage_Change_In_Stock(std::string file_path)
{

    CSV_Parser csv_parser(file_path);
    std::vector<std::string> first_line_split = split(csv_parser.Get_First_Line(), ',');
    std::vector<std::string> last_line_split = split(csv_parser.Get_Last_Line(), ',');

    OHLC first_ohlc = String_Vector_To_Stock_Data(first_line_split);
    OHLC last_ohlc = String_Vector_To_Stock_Data(last_line_split);

    double percentage_change = Percentage_Change(first_ohlc.Get_Open_Price(), last_ohlc.Get_Open_Price());

    return percentage_change;

}

/**
 * @brief Get_Features
 * This function uses a single function to get features
 * of a stock using the stock data itself, and the amount of stocks
 * to take into account during a calculation. This function is used
 * in multiple places which is why it is used as a wrapper for a single
 * function in order to quickly change the function used inside
 * of this function.
 * @param stock_data
 * The vector of stock data in OHLC format to get the features of.
 * @param stocks_to_get_distance_of
 * The amount of stocks to take into account during  an iteration.
 * @return
 * A vector of vector of doubles that are the features of stock_data.
 */

std::vector<std::vector<double>> Get_Features(std::vector<OHLC> stock_data)
{

    std::vector<std::vector<double>> test = Get_Percentage_Changes_In_Rows(Get_Euclidean_Distances(stock_data));

    return test;

}

std::vector<std::vector<double>> Get_Percentage_Changes_In_Rows(std::vector<std::vector<double>> stock_data)
{

    std::vector<std::vector<double>> percentage_changes;

    for(size_t row = 0; row < stock_data.size() - 1; row++) {


        std::vector<double> current_row;

        for(size_t column = 0; column < stock_data[row].size(); column++) {

            current_row.push_back( Percentage_Change(stock_data[row][column], stock_data[row + 1][column]) );

        }

        percentage_changes.push_back(current_row);

    }

    return percentage_changes;

}

/**
 * @brief Get_Euclidean_Distances
 * This function returns the euclidean distance
 * of some amount of OHLC objects given by
 * stocks_to_get_distance_of.
 * @param stock_data
 * The stock data to get the euclidean distnaces for.
 * @param stocks_to_get_distance_of
 * The amount of stocks to get the euclidean distance
 * of in a single calculation.
 * @return
 * The euclidean distances of the OHLC data in stock_data.
 */

std::vector<std::vector<double>> Get_Euclidean_Distances(std::vector<OHLC> stock_data)
{

    std::vector<std::vector<double>> distances;

    for(size_t i = STOCKS_TO_GET_DISTANCE_OF; i < stock_data.size(); i += WINDOW_INCREMENT)
    {

        std::vector<double> current_distance;

        for(size_t j = i - STOCKS_TO_GET_DISTANCE_OF + 1; j < i; j++) {

            current_distance.push_back( stock_data[j].Get_Euclidean_Distance(stock_data[j - 1]));



        }
        distances.push_back(current_distance);


    }

    return distances;

}

/**
 * @brief Get_Manhattan_Distances
 * This function returns the manhattan distance
 * of some amount of OHLC objects given by
 * stocks_to_get_distance_of.
 * @param stock_data
 * The stock data to get the manhattan distances for.
 * @param stocks_to_get_distance_of
 * The amount of stocks to get the manhattan distance
 * of in a single calculation.
 * @return
 * The manhattan distances of the OHLC data in stock_data.
 */

std::vector<std::vector<double>> Get_Manhattan_Distances(std::vector<OHLC> stock_data)
{

    std::vector<std::vector<double>> distances;

    for(size_t i = STOCKS_TO_GET_DISTANCE_OF; i < stock_data.size(); i++)
    {

        std::vector<double> current_distance;

        for(size_t j = i - STOCKS_TO_GET_DISTANCE_OF + 1; j < i; j++) {

            current_distance.push_back( stock_data[j].Get_Manhattan_Distance(stock_data[j - 1]));

        }


        distances.push_back(current_distance);

    }

    return distances;

}

/**
 * @brief Get_Euclidean_Distances
 * This function returns the euclidean distance
 * of some amount of OHLC objects given by
 * stocks_to_get_distance_of.
 * @param stock_data
 * The stock data to get the euclidean distnaces for.
 * @param stocks_to_get_distance_of
 * The amount of stocks to get the euclidean distance
 * of in a single calculation.
 * @return
 * The euclidean distances of the OHLC data in stock_data.
 */

std::vector<std::vector<double>> Get_Minkowski_Distances(std::vector<OHLC> stock_data, double power)
{

    std::vector<std::vector<double>> distances;

    for(size_t i = STOCKS_TO_GET_DISTANCE_OF; i < stock_data.size(); i += WINDOW_INCREMENT)
    {

        std::vector<double> current_distance;

        for(size_t j = i - STOCKS_TO_GET_DISTANCE_OF + 1; j < i; j++) {

            current_distance.push_back( stock_data[j].Get_Minkowski_Distance(stock_data[j - 1], power));



        }
        distances.push_back(current_distance);


    }

    return distances;

}

std::vector<int> Get_Labels(std::vector<OHLC> stock_data)
{

    std::vector<int> labels;
    int start_index = 0;

    for(size_t row_index = STOCKS_TO_GET_DISTANCE_OF; row_index < stock_data.size() - 1; row_index += WINDOW_INCREMENT)
    {

        std::vector<OHLC> sub(&stock_data[start_index],&stock_data[row_index]);
        start_index++;

        RSI rsi(sub);

        double current_rsi = rsi.Get_RSI();

        if(Percentage_Change( stock_data[row_index].Get_Open_Price(), stock_data[row_index].Get_Close_Price() ) > 0.1 && current_rsi < 30)
        {

            labels.push_back(1);

        } else {

            labels.push_back(0);

        }

    }

    return labels;


}

std::vector<std::vector<double>> Get_Percentage(std::vector<OHLC> stock_data, int stocks_to_get_percentage_of)
{

    std::vector<std::vector<double>> stock_percentages;

    for(int i = stocks_to_get_percentage_of; i < stock_data.size(); i++)
    {
        stock_percentages.push_back(stock_data[i].Get_Percentage_Difference(stock_data[i - 1]));

    }


    return stock_percentages;

}

std::vector<std::vector<double>> Get_Difference(std::vector<OHLC> stock_data)
{

    std::vector<std::vector<double>> stock_differences;

    for(int i = 1; i < stock_data.size(); i++)
    {
        stock_differences.push_back(stock_data[i].Get_Difference(stock_data[i - 1]));

    }


    return stock_differences;

}

OHLC String_Vector_To_Stock_Data(std::vector<std::string> string_vector)
{

    std::string date = string_vector[0];

    double open_price = std::stod(string_vector[1]);
    double high_price = std::stod(string_vector[2]);
    double low_price = std::stod(string_vector[3]);
    double close_price = std::stod(string_vector[4]);

    OHLC current_stock_data(open_price, high_price, low_price, close_price);

    return current_stock_data;

}

std::vector<OHLC> CSV_To_OHLC_Vector(std::string file_dir)
{

    CSV_Parser csv_parser(file_dir);
    std::vector<OHLC> data;
    int counter = 0;

    while(csv_parser.Has_Next())
    {
        std::vector<std::string> current_line = csv_parser.Get_Next_Row();
        if(current_line.size() > 0)
        {

            OHLC current_stock_data = String_Vector_To_Stock_Data(current_line);
            data.push_back(current_stock_data);

        }
        counter++;
    }

    return data;

}

