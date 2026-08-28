#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h> 
#include <cstdlib>
#include <ctime>
#include <iomanip>
#define MAX 100

using namespace std;

struct User{
    string accountName;
    string accountNumber;
    string birthday;
    string encryptedPin;
    string contactNumber;
    double balance;
};

class AccountManager{
    private:
        User person[MAX];
        int last = -1;
        bool isFull();
        bool isEmpty();
        void passwordEncryptor(string pin);
        void passwordDecryptor(string encryptedPin); 
        void flashDriveChecker();
        void saveDrive(); // para sa acc num and pin code ng flashdrive
        void retrieveDrive(); // para sa acc num and pin code ng flashdrive
        string pinValidation(string pin); // para sa pagcheck kung 4 or 6 digit yung password, string para pwede maglagay 0 sa first digit
    
    public:
        bool registerAccount(string pathname);
        bool loginAccount(string pathname);
        void changePin();
        void fundTransfer();
        void balanceInquiry();
        bool withdraw(); 
        bool deposit();
        int menu();
        int locate(string n); // para sa fund transfer kung existing yung user
        void save();
        void retrieve();

};

bool AccountManager :: isFull(){
    return(last == MAX-1);
}

bool AccountManager :: isEmpty(){
    return(last == -1);
}

string AccountManager :: pinValidation(string pin){
    if(pin.size() != 4){
        cout << "Invalid Pin Code **Must Only Contain 4-Digits**" << endl;
        system("pause");
        return;
    }
    for(int i = 0; i < pin.size(); i++){
        if(isdigit(pin[i])){
            cout << "Invalid Pin Code **Must Only Contain Numerical Digits**" << endl;
            system("pause");
            return;
        }
    }
    return pin;
}

int AccountManager :: locate(string n){
    for(int i = 0; i < last; i++){
        if(n == person[i].accountNumber){
            return i;
        }
    }
    return -1;
}


int main(){



    return 0;
}
