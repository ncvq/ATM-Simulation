#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h> 
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
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
        bool isFull(); // para sa registration
        bool isEmpty();
        void passwordEncryptor(string pin);
        void passwordDecryptor(string encryptedPin); 
        void flashDriveChecker();
        void saveDrive(); // para sa acc num and pin code ng flashdrive
        void retrieveDrive(); // para sa acc num and pin code ng flashdrive
        bool pinValidation(string pin);
    
    public:
        bool registerAccount(string pathname); // trigger kung wala sa loob ng USB yung accountNum at pincode
        bool loginAccount(string pathname); // trigger kung nasa USB yung accountNum at pincode
        void changePin();
        void fundTransfer();
        void balanceInquiry();
        bool withdraw(); 
        bool deposit();
        int menu();
        int locate(string n); // para sa fund transfer kung existing yung user 
        void save(); // para sa mismong database natin
        void retrieve(); // para sa mismong database natin

};

bool AccountManager :: isFull(){
    return(last == MAX-1);
}

bool AccountManager :: isEmpty(){
    return(last == -1);
}

bool AccountManager :: pinValidation(string pin){
    if(pin.size() != 4){
        cout << "Invalid Pin Code **Must Only Contain 4-Digits**" << endl;
        system("pause");
        return false;
    }
    for(int i = 0; i < pin.size(); i++){
        if(!isdigit(pin[i])){
            cout << "Invalid Pin Code **Must Only Contain Numerical Digits**" << endl;
            system("pause");
            return false;
        }
    }
    return true;
}

int AccountManager :: locate(string n){
    for(int i = 0; i <= last; i++){
        if(n == person[i].accountNumber){
            return i;
        }
    }
    return -1;
}

void AccountManager :: save(){
    ofstream file("UserDatabase.csv");
    if(!file){
        cout << "Filename not Found" << endl;
        return;
    }
    else{
        for(int i = 0; i <= last; i++){
            file << person[i].accountName << ","
                << person[i].accountNumber << ","
                << person[i].birthday << ","
                << person[i].encryptedPin << ","
                << person[i].contactNumber << ","
                << person[i].balance << endl;
        }
    }
    file.close();
}

void AccountManager :: retrieve(){
    ifstream file("UserDatabase.csv");
    
    if(!file){
        cout << "Filename not Found" << endl;
        return;
    }
    else{
        string line;
        string strBalance; 
        User filedata;
        while(getline(file, line)){
            if(line.empty()){
                continue;
            }
            stringstream ss(line);
            getline(ss, filedata.accountName, ',');
            getline(ss, filedata.accountNumber, ',');
            getline(ss, filedata.birthday, ',');
            getline(ss, filedata.encryptedPin, ',');
            getline(ss, filedata.contactNumber, ',');
            getline(ss, strBalance, ',');
            filedata.balance = stod(strBalance);
            if(!isFull()){
                person[++last] = filedata;
            }
        }
    }
    file.close();
}

int AccountManager :: menu(){
    system("cls");
    int choice;

    cout << "Welcome to eVault, \"Your Money. Digitally Secured\" " << endl;
    cout << "[1] Balance Inquiry" << endl;
    cout << "[2] Deposit" << endl;
    cout << "[3] Withdraw" << endl;
    cout << "[4] Deposit" << endl;
    cout << "[5] Fund Transfer" << endl;
    cout << "[6] Fund Transfer" << endl;
    cout << "Input your Choice (1-6): " << endl;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        return 0;
    }
}
int main(){



    return 0;
}
