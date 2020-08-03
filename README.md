# AlgoTradeSystem

This is an algorithmic trading system (ATS) written in C++. It uses the SVM included in OpenCV for training a model. Periodic RSI values are computed and used as indicators for what the model label value for a given trading period should be. A 0 indicates a market sell order should be placed for the next day as soon as the market opens. A 1 indicates a market buy.

A benchmark is run using the test data given by the user. Each individual file inside the test data directory represents one stock and one separate test case the program wil run. Data on how well the program performed relative to the market that given period is given at the end of the benchmark.
