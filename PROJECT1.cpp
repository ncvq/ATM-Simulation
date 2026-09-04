#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h> 
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cmath>        // ADDED: for fmod() in withdraw()
#include <cctype>       // ADDED: for isdigit() in pinValidation()
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
        int currentUser = -1;   // ADDED: index ng naka-login na account
        bool isFull(); // para sa registration
        bool isEmpty();
        string passwordEncryptor(string pin);
        string passwordDecryptor(string encryptedPin); 
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
            file << person[i].accountName    << ","
                 << person[i].accountNumber  << ","
                 << person[i].birthday       << ","
                 << person[i].encryptedPin   << ","
                 << person[i].contactNumber  << ","
                 << person[i].balance        << endl;
        }
    }
    file.close();
}

void AccountManager :: retrieve(){
    ifstream file("UserDatabase.csv");
    
    if(!file){
        cout << "No existing database. A new one will be created." << endl;
        return;
    }
    else{
        last = -1;
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

            if(strBalance.empty()){
                continue;
            }
            filedata.balance = stod(strBalance);

            if(!isFull()){
                person[++last] = filedata;
            }
        }
    }
    file.close();
}

string AccountManager :: passwordEncryptor(string pin){
    string hexadecimalMap[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

    for(int i = 0; i < pin.size(); i++){
        int XOR = pin[i] ^ 67; // key = 67
        pin[i] = (XOR / 16) + (XOR % 16); 
    }
    return pin;
}

void AccountManager :: balanceInquiry(){
    system("cls");

    if(currentUser == -1){
        cout << "No account is currently logged in." << endl;
        system("pause");
        return;
    }

    cout << fixed << setprecision(2);
    cout << "========================================" << endl;
    cout << "            BALANCE INQUIRY             " << endl;
    cout << "========================================" << endl;
    cout << "Account Name   : " << person[currentUser].accountName   << endl;
    cout << "Account Number : " << person[currentUser].accountNumber << endl;
    cout << "Available Bal. : PHP " << person[currentUser].balance   << endl;
    cout << "========================================" << endl;
    system("pause");
}

bool AccountManager :: deposit(){
    system("cls");
    double amount;

    if(currentUser == -1){
        cout << "No account is currently logged in." << endl;
        system("pause");
        return false;
    }

    cout << fixed << setprecision(2);
    cout << "========================================" << endl;
    cout << "                DEPOSIT                 " << endl;
    cout << "========================================" << endl;
    cout << "Current Balance: PHP " << person[currentUser].balance << endl;
    cout << "Enter amount to deposit: PHP ";
    cin  >> amount;

    if(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid input. Numbers only." << endl;
        system("pause");
        return false;
    }

    if(amount <= 0){
        cout << "\nAmount must be greater than zero." << endl;
        system("pause");
        return false;
    }

    if(amount > 50000){
        cout << "\nMaximum deposit per transaction is PHP 50,000.00" << endl;
        system("pause");
        return false;
    }

    person[currentUser].balance += amount;
    save();

    cout << "\n--- DEPOSIT SUCCESSFUL ---" << endl;
    cout << "Amount Deposited: PHP " << amount << endl;
    cout << "New Balance     : PHP " << person[currentUser].balance << endl;
    system("pause");
    return true;
}

bool AccountManager :: withdraw(){
    system("cls");
    double amount;

    if(currentUser == -1){
        cout << "No account is currently logged in." << endl;
        system("pause");
        return false;
    }

    cout << fixed << setprecision(2);
    cout << "========================================" << endl;
    cout << "               WITHDRAW                 " << endl;
    cout << "========================================" << endl;
    cout << "Current Balance: PHP " << person[currentUser].balance << endl;
    cout << "Enter amount to withdraw: PHP ";
    cin  >> amount;

    if(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid input. Numbers only." << endl;
        system("pause");
        return false;
    }

    if(amount <= 0){
        cout << "\nAmount must be greater than zero." << endl;
        system("pause");
        return false;
    }

    if(fmod(amount, 100) != 0){
        cout << "\nAmount must be in multiples of PHP 100.00" << endl;
        system("pause");
        return false;
    }

    if(amount > person[currentUser].balance){
        cout << "\nInsufficient balance." << endl;
        cout << "Available: PHP " << person[currentUser].balance << endl;
        system("pause");
        return false;
    }

    person[currentUser].balance -= amount;
    save();

    cout << "\n--- WITHDRAWAL SUCCESSFUL ---" << endl;
    cout << "Amount Withdrawn: PHP " << amount << endl;
    cout << "New Balance     : PHP " << person[currentUser].balance << endl;
    system("pause");
    return true;
}

void AccountManager :: fundTransfer(){
    system("cls");
    string recipientAcc;
    double amount;
    char confirm;

    if(currentUser == -1){
        cout << "No account is currently logged in." << endl;
        system("pause");
        return;
    }

    cout << fixed << setprecision(2);
    cout << "========================================" << endl;
    cout << "             FUND TRANSFER              " << endl;
    cout << "========================================" << endl;
    cout << "Your Balance: PHP " << person[currentUser].balance << endl;
    cout << "Enter recipient account number: ";
    cin  >> recipientAcc;

    int index = locate(recipientAcc);

    if(index == -1){
        cout << "\nAccount number not found." << endl;
        system("pause");
        return;
    }

    if(index == currentUser){
        cout << "\nYou cannot transfer funds to your own account." << endl;
        system("pause");
        return;
    }

    cout << "\nRecipient Found: " << person[index].accountName << endl;
    cout << "Enter amount to transfer: PHP ";
    cin  >> amount;

    if(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid input. Numbers only." << endl;
        system("pause");
        return;
    }

    if(amount <= 0){
        cout << "\nAmount must be greater than zero." << endl;
        system("pause");
        return;
    }

    if(amount > person[currentUser].balance){
        cout << "\nInsufficient balance." << endl;
        system("pause");
        return;
    }

    cout << "\nTransfer PHP " << amount << " to "
         << person[index].accountName << "? (Y/N): ";
    cin  >> confirm;

    if(confirm != 'Y' && confirm != 'y'){
        cout << "\nTransaction cancelled." << endl;
        system("pause");
        return;
    }

    person[currentUser].balance -= amount;
    person[index].balance       += amount;
    save();

    cout << "\n--- TRANSFER SUCCESSFUL ---" << endl;
    cout << "Sent To         : " << person[index].accountName << endl;
    cout << "Amount          : PHP " << amount << endl;
    cout << "Your New Balance: PHP " << person[currentUser].balance << endl;
    system("pause");
}

void AccountManager :: changePin(){
    system("cls");
    string oldPin, newPin, confirmPin;

    if(currentUser == -1){
        cout << "No account is currently logged in." << endl;
        system("pause");
        return;
    }

    cout << "========================================" << endl;
    cout << "              CHANGE PIN                " << endl;
    cout << "========================================" << endl;
    cout << "Enter current PIN: ";
    cin  >> oldPin;

    if(passwordEncryptor(oldPin) != person[currentUser].encryptedPin){
        cout << "\nIncorrect PIN." << endl;
        system("pause");
        return;
    }

    cout << "Enter new 4-digit PIN: ";
    cin  >> newPin;

    if(!pinValidation(newPin)){
        return;
    }

    if(newPin == oldPin){
        cout << "\nNew PIN must be different from the old PIN." << endl;
        system("pause");
        return;
    }

    cout << "Confirm new PIN: ";
    cin  >> confirmPin;

    if(newPin != confirmPin){
        cout << "\nPINs do not match. PIN was not changed." << endl;
        system("pause");
        return;
    }

    person[currentUser].encryptedPin = passwordEncryptor(newPin);
    save();

    cout << "\nPIN changed successfully." << endl;
    system("pause");
}

int AccountManager :: menu(){
    system("cls");
    int choice;

    cout << "========================================" << endl;
    cout << "  eVault - \"Your Money. Digitally Secured\"" << endl;
    cout << "========================================" << endl;
    cout << "[1] Balance Inquiry" << endl;
    cout << "[2] Deposit" << endl;
    cout << "[3] Withdraw" << endl;
    cout << "[4] Fund Transfer" << endl;
    cout << "[5] Change PIN" << endl;
    cout << "[6] Logout" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Input your Choice (1-6): ";
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 0;
    }
    return choice;
}

int main(){
    AccountManager atm;
    int choice;

    atm.retrieve();     // load database sa startup

    // atm.loginAccount("E:\\");

    do{
        choice = atm.menu();

        switch(choice){
            case 1: atm.balanceInquiry(); break;
            case 2: atm.deposit();        break;
            case 3: atm.withdraw();       break;
            case 4: atm.fundTransfer();   break;
            case 5: atm.changePin();      break;
            case 6:
                atm.save();
                cout << "\nLogged out. Thank you!" << endl;
                break;
            default:
                cout << "\nInvalid choice. Try again." << endl;
                system("pause");
        }
    }while(choice != 6);

    return 0;
}
