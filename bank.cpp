#include<bits/stdc++.h>
using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds{};

class Account{
    private:
    long acc_no;
    string name;
    string DOB;
    float acc_balance;
    static long next_acc_no;

    public:
    Account(){}
    Account(string name,string DOB,float balance);
    long get_accno(){
        return acc_no;
    }
    string get_name(){
        return name;
    }
    string get_DOB(){
        return DOB;
    }
    float get_balance(){
        return acc_balance;
    }

    void Deposit(float amount);
    void Withdraw(float amount);

    static void setlast_accno(long accountnumber);
    static long getlast_accno();
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);
};

long Account::next_acc_no=0;

class Bank{
    private:
    map<long,Account> accounts;

    public:
    Bank();
    Account OpenAccount(string name,string DOB,float acc_balance);
    Account BalanceEnquiry(long acc_no );
    Account Deposit(long acc_no,float amount);
    Account Withdraw(long acc_no, float amount);
    void CloseAccount(long acc_no);
    void ShowAllAccounts();
    ~Bank();
    
};

Account::Account(string name, string DOB, float acc_balance){
    next_acc_no++;
    this->acc_no=next_acc_no;
    this->name=name;
    this->DOB=DOB;
    this->acc_balance=acc_balance;
}

void Account::Deposit(float amount){
    acc_balance+=amount;
}

void Account::Withdraw(float amount){
    if(acc_balance-amount<MIN_BALANCE){
        throw InsufficientFunds();
    }
    acc_balance-=amount;
}

void Account::setlast_accno(long acc_no){
    next_acc_no=acc_no;
}

long Account::getlast_accno(){
    return next_acc_no;
}

ofstream & operator<<(ofstream &ofs, Account &acc){
    ofs<<acc.acc_no<<endl;
    ofs<<acc.name<<endl;
    ofs<<acc.DOB<<endl;
    ofs<<acc.acc_balance<<endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs, Account &acc){
    ifs>>acc.acc_no;
    ifs>>acc.name;
    ifs>>acc.DOB;
    ifs>>acc.acc_balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc){
    os<<"Account Number: "<<acc.get_accno()<<endl;
    os<<"Name: "<<acc.get_name()<<endl;
    os<<"DOB: "<<acc.get_DOB()<<endl;
    os<<"Account Balance: "<<acc.get_balance()<<endl;
    return os;
}

Bank::Bank(){
    Account account;
    ifstream infile;
    infile.open("Bank.txt");

    if(!infile){
        return;
    }

    while(!infile.eof()){
        while(infile>>account){
            accounts.insert(pair<long,Account>(account.get_accno(),account));
        }
    }

    Account::setlast_accno(account.get_accno());
    infile.close();
}

Account Bank::OpenAccount(string name,string DOB, float balance){
    ofstream outfile;
    Account acc(name,DOB,balance);

    accounts.insert(pair<long,Account>(acc.get_accno(),acc));
    outfile.open("Bank.txt",ios::trunc);
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        outfile<<itr->second;
    }
    outfile.close();
    return acc;
}

Account Bank::BalanceEnquiry(long acc_no){
    map<long,Account>::iterator itr=accounts.find(acc_no);
    return itr->second;
}

Account Bank::Deposit(long acc_no, float amount){
    map<long,Account>::iterator itr=accounts.find(acc_no);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdraw(long acc_no, float amount){
    map<long,Account>::iterator itr=accounts.find(acc_no);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long acc_no){
    map<long,Account>::iterator itr=accounts.find(acc_no);
    cout<<"Account Closed "<<endl<<itr->second;
    accounts.erase(acc_no);
}

void Bank::ShowAllAccounts(){
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.txt",ios::trunc);

    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        outfile<<itr->second;
    }
    outfile.close();
}

int main(){
    Bank b;
    Account acc;

    int choice;
    string name;
    string DOB; 
    long acc_no; 
    float balance; 
    float amount;

    cout<<"BANKING SYSTEM"<<endl;
    do{
        cout<<endl;
        cout<<"What service you want to avail?"<<endl;
        cout<<"1. Open an Account"<<endl;
        cout<<"2. Balance Enquiry"<<endl;
        cout<<"3. Deposit"<<endl;
        cout<<"4. Withdraw"<<endl;
        cout<<"5. CLose an Account"<<endl;
        cout<<"6. Show all Accounts"<<endl;
        cout<<"7. Quit"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;

        cin.ignore();

        switch(choice){
            case 1:
            cout<<"Enter the name:";
            getline(cin,name);
            cout<<"Enter your Date of Birth:";
            getline(cin,DOB);
            cout<<"Enter the initial balance:";
            cin>>balance;
            acc=b.OpenAccount(name,DOB,balance);
            cout<<endl<<"Congratulations, the Account has been created successfully"<<endl;
            cout<<acc;
            break;

            case 2:
            cout<<"Enter your account number:";
            cin>>acc_no;
            acc=b.BalanceEnquiry(acc_no);
            cout<<endl<<"Account Details"<<endl;
            cout<<acc;
            break;

            case 3:
            cout<<"Enter your account number:";
            cin>>acc_no;
            cout<<"Enter amount to be deposited:";
            cin>>amount;
            acc=b.Deposit(acc_no,amount);
            cout<<endl<<"Amount is Deposited"<<endl;
            cout<<acc;
            break;

            case 4:
            cout<<"Enter your account number:";
            cin>>acc_no;
            cout<<"Enter amount to be withdrawwn:";
            cin>>amount;
            acc=b.Withdraw(acc_no,amount);
            cout<<endl<<"Amount Withdrawn"<<endl;
            cout<<acc;
            break;

            case 5:
            cout<<"Enter your account number:";
            cin>>acc_no;
            b.CloseAccount(acc_no);
            cout<<endl<<"Account is closed"<<endl;
            cout<<acc;
            break;

            case 6:
            b.ShowAllAccounts();
            break;

            case 7:
            break;

            default:
            cout<<"Enter correct choice";
            exit(0);
        }
    }while(choice!=7);
    return 0;

}







