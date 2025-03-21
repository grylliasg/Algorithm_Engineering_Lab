#include <iostream>
#include <string>
using namespace std;

class BankAccount
{
    public:
    // Constructor 
    BankAccount(const string& h, double b) : holder(h), balance(b) {}

    double getBalance() {return balance;}

    void deposit(double amount) {balance += amount;}

    virtual void withdraw(double amount)
    {
        if (amount <= balance)
        {
            balance -= amount;
            cout << "New balance: " << balance <<endl;
        }
        else {cout << "Not enough balance..." << endl;}
    }

    virtual void display() {cout << "Holder Name: " << holder << "  " << "Balance: " << balance << endl;}

    bool operator==(BankAccount& other)
    {
        return (holder == other.holder && balance == other.balance);
    }

    void transfer(BankAccount& toAccount, double amount)
    {
        if (balance >= amount)
        {
            toAccount.balance += amount;
            cout << "Transfer completed successfully!" << endl;
        }
        else {cout << "Not enough balance..." << endl;}
    }

    protected:
    string holder;
    double balance;
};

class CheckingAccount: public BankAccount
{
    public:
    // Constructor
    CheckingAccount(const string& h, double b, double f) : BankAccount(h, b), fee(f) {}

    // Overriding
    void display()
    {
        cout << "Holder Name:" << holder << ",  Balance:" << balance << ",  Fee:" << fee << endl;
    }

    void withdraw(double amount)
    {
        double totalAmount = amount + fee; // Synoliko poso
        if (totalAmount <= balance)
        {
            balance -= totalAmount;
            cout << "New balance: " << balance <<endl;
        }
        else {cout << "Not enough balance..." << endl;}
    }

    private:
    double fee;

};

int main()
{
    BankAccount acc1("John Doe", 1000);
    CheckingAccount acc2("John Doe", 1000, 2);

    acc1.withdraw(100);
    //acc2.withdraw(100);

    BankAccount& ref = acc2;
    ref.withdraw(100);

    return 0;
}