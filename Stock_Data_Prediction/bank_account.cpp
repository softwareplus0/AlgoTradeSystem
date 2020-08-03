#include "bank_account.h"
#include <iostream>

Bank_Account::Bank_Account(int pCurrentBalance)
{
    currentBalance = pCurrentBalance;
    numberOfTransactions = 0;
}

void Bank_Account::deposit(int amount)
{
    currentBalance += amount;
    numberOfTransactions++;
}

void Bank_Account::withdraw(int amount)
{
    if (currentBalance >= amount)
    {
        currentBalance -= amount;
    }
    else
    {
        std::cout << "ERROR: Cannot withdraw " << amount << " since current balance is " << currentBalance << std::endl;
        exit(0);
    }
    numberOfTransactions++;
}

int Bank_Account::getCurrentBalance()
{
    return currentBalance;
}

int Bank_Account::getNumberOfTransactions()
{
    return numberOfTransactions;
}
