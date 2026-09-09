#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cctype>
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
        int currentUser = -1;
        bool isFull();
        string passwordEncryptor(string pin);
        string flashDriveChecker();
        bool saveDrive(string accNum, string encPin);
        bool retrieveDrive(string &accNum, string &encPin);
        bool pinValidation(string pin);

    public:
        bool driveConnected();
        void registerAccount();
        bool loginAccount();
        void logout();
        void fundTransfer();
        void balanceInquiry();
        void changePin();
        bool withdraw();
        bool deposit();
        int  menu();
        int  loginMenu();
        int  locate(string n);
        void save();
        void retrieve();
};

string AccountManager :: flashDriveChecker(){
    for(char letter = 'A'; letter <= 'Z'; letter++){
        string drive = string(1,letter) + ":\\";
        if(GetDriveTypeA(drive.c_str()) == DRIVE_REMOVABLE){
            return drive;
        }
    }
    return "";
}

bool AccountManager :: driveConnected(){
    return flashDriveChecker() != "";
}

bool AccountManager :: saveDrive(string accNum, string encPin){
    string path = flashDriveChecker();
    if(path == ""){
        cout << "Please Insert an ATM Card to the Machine!" << endl;
        return false;
    }
    ofstream file(path + "\\pin.code");   // FIXED: consistent filename
    if(!file){
        cout << "Unable to write to card." << endl;
        return false;
    }
    file << accNum << "\n" << encPin << "\n";
    file.close();
    return true;
}

bool AccountManager :: retrieveDrive(string &accNum, string &encPin){
    string path = flashDriveChecker();
    if(path == ""){
        cout << "Please Insert an ATM Card to the Machine!" << endl;   // ADDED
        return false;
    }
    ifstream file(path + "\\pin.code");   // FIXED: consistent filename
    if(!file){
        cout << "Unable to read card data. The card may not be registered." << endl;   // ADDED
        return false;
    }
    getline(file, accNum);
    getline(file, encPin);
    file.close();
    return true;
}

bool AccountManager :: isFull(){
    return(last == MAX-1);
}

int AccountManager :: locate(string n){
    for(int i = 0; i <= last; i++){
        if(n == person[i].accountNumber){
            return i;
        }
    }
    return -1;
}

bool AccountManager :: pinValidation(string pin){
    if(pin.size() != 4){
        cout << "Invalid Pin Code **Must Only Contain 4-Digits**" << endl;
        system("pause");
        return false;
    }
    for(int i = 0; i < (int)pin.size(); i++){
        if(!isdigit(pin[i])){
            cout << "Invalid Pin Code **Must Only Contain Numerical Digits**" << endl;
            system("pause");
            return false;
        }
    }
    return true;
}

string AccountManager :: passwordEncryptor(string pin){
    string encpin = "";
    for(int i = 0; i < (int)pin.size(); i++){
        encpin += (((pin[i] - '0') + 3) % 10) + '0';   // key = 3
    }
    return encpin;
}

void AccountManager :: save(){
    ofstream file("UserDatabase.csv");
    if(!file){
        cout << "Filename not Found" << endl;
        return;
    }
    for(int i = 0; i <= last; i++){
        file << person[i].accountName    << ","
             << person[i].accountNumber  << ","
             << person[i].birthday       << ","
             << person[i].encryptedPin   << ","
             << person[i].contactNumber  << ","
             << person[i].balance        << endl;
    }
    file.close();
}

void AccountManager :: retrieve(){
    ifstream file("UserDatabase.csv");
    if(!file){
        cout << "No existing database. A new one will be created." << endl;
        return;
    }
    last = -1;
    string line, strBalance;
    User filedata;
    while(getline(file, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        getline(ss, filedata.accountName, ',');
        getline(ss, filedata.accountNumber, ',');
        getline(ss, filedata.birthday, ',');
        getline(ss, filedata.encryptedPin, ',');
        getline(ss, filedata.contactNumber, ',');
        getline(ss, strBalance, ',');
        if(strBalance.empty()) continue;
        filedata.balance = stod(strBalance);
        if(!isFull()){
            person[++last] = filedata;
        }
    }
    file.close();
}

void AccountManager :: registerAccount(){
    system("cls");

    if(isFull()){
        cout << "Our banking system has reached max capacity!" << endl;
        system("pause");
        return;
    }

    if(!driveConnected()){
        cout << "Please Insert an ATM Card to the Machine!" << endl;
        system("pause");
        return;
    }

    string existingAccNum, existingEncPin;
    if(retrieveDrive(existingAccNum, existingEncPin)){
        int pos = locate(existingAccNum);
        if(pos != -1){
            cout << "This card is already registered to an existing account." << endl;
            cout << "Account Name: " << person[pos].accountName << endl;
            cout << "Registering a new account will overwrite this card and you" << endl;
            cout << "will lose access to the existing account through this card." << endl;
            cout << "Continue anyway? (Y/N): ";
            char confirm;
            cin >> confirm;
            if(confirm != 'Y' && confirm != 'y'){
                cout << "Registration cancelled." << endl;
                system("pause");
                return;
            }
        }
    }

    User x;
    string accNum;
    string rawPin;
    cout << "========================================" << endl;
    cout << "           NEW ACCOUNT REGISTRATION      " << endl;
    cout << "========================================" << endl;

    cin.ignore(1000, '\n');
    cout << "Full Name: ";              getline(cin, x.accountName);
    cout << "Birthday (MM/DD/YYYY): ";  getline(cin, x.birthday);
    cout << "Contact Number: ";         getline(cin, x.contactNumber);

    do{
        cout << "Initial Deposit (min PHP 5000): PHP ";
        cin >> x.balance;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            x.balance = 0;
            cout << "Invalid input. Numbers only." << endl;
        }
        else if(x.balance < 5000){
            cout << "Amount is below the minimum. Initial deposit must be at least PHP 5000.00" << endl;   // ADDED
        }
    } while(x.balance < 5000);

    do{
        accNum = to_string(10000 + rand() % 90000);
    } while(locate(accNum) != -1);
    x.accountNumber = accNum;

    do{
        cout << "Set a 4-digit PIN: ";
        cin >> rawPin;
    } while(!pinValidation(rawPin));
    x.encryptedPin = passwordEncryptor(rawPin);

    last++;
    person[last] = x;

    if(!saveDrive(accNum, x.encryptedPin)){   // FIXED: now checks the result
        cout << "Registration failed: could not write to card." << endl;
        last--; 
        system("pause");
        return;
    }

    save();

    cout << "\n--- REGISTRATION SUCCESSFUL ---" << endl;
    cout << "Account Number: " << x.accountNumber << endl;
    cout << "Please keep your card safe." << endl;
    system("pause");
}

bool AccountManager :: loginAccount(){
    system("cls");
    string cardAccNum;
    string cardEncPin;

    if(!retrieveDrive(cardAccNum, cardEncPin)){
        system("pause");
        return false;
    }

    int pos = locate(cardAccNum);
    if(pos == -1){
        cout << "Account not recognized by this system." << endl;
        system("pause");
        return false;
    }

    if(cardEncPin != person[pos].encryptedPin){
        cout << "Card data mismatch. Possible tampering." << endl;
        system("pause");   // FIXED: was missing
        return false;
    }

    string enteredPin;
    cout << "Enter your PIN: ";
    cin >> enteredPin;

    if(passwordEncryptor(enteredPin) != person[pos].encryptedPin){
        cout << "Incorrect PIN." << endl;
        system("pause");
        return false;
    }

    currentUser = pos;
    cout << "\nWelcome, " << person[pos].accountName << "!" << endl;
    system("pause");
    return true;
}

void AccountManager :: logout(){
    currentUser = -1;
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
        cin.ignore(1000, '\n');
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
        cin.ignore(1000, '\n');
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
        cin.ignore(1000, '\n');
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

    string newEncPin = passwordEncryptor(newPin);
    person[currentUser].encryptedPin = newEncPin;

    if(!saveDrive(person[currentUser].accountNumber, newEncPin)){   // ADDED: keep card in sync
        cout << "\nWarning: PIN changed in system, but failed to update the card." << endl;
        system("pause");
        return;
    }

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

    if(cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
        return 0;
    }
    return choice;
}

int AccountManager :: loginMenu(){
    system("cls");
    int choice;

    cout << "========================================" << endl;
    cout << "  eVault - \"Your Money. Digitally Secured\"" << endl;
    cout << "========================================" << endl;
    cout << "[1] Register" << endl;
    cout << "[2] Login" << endl;
    cout << "[3] Exit" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Input your Choice (1-3): ";
    cin >> choice;

    if(cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
        return 0;
    }
    return choice;
}

int main(){
    srand((unsigned)time(0));

    AccountManager atm;

    if(!atm.driveConnected()){
        cout << "Please Insert an ATM Card to the Machine!" << endl;
        cout << "No drive detected. Terminating program." << endl;
        system("pause");
        return 0;
    }

    atm.retrieve();

    int loginChoice;
    do{
        loginChoice = atm.loginMenu();

        switch(loginChoice){
            case 1:
                atm.registerAccount();
                break;

            case 2:
                if(atm.loginAccount()){
                    int choice;
                    do{
                        choice = atm.menu();
                        switch(choice){
                            case 1: atm.balanceInquiry(); break;
                            case 2: atm.deposit();        break;
                            case 3: atm.withdraw();       break;
                            case 4: atm.fundTransfer();   break;
                            case 5: atm.changePin();      break;
                            case 6:
                                atm.logout();   // FIXED: actually clears the session now
                                cout << "\nLogged out. Thank you!" << endl;
                                system("pause");
                                break;
                            default:
                                cout << "\nInvalid choice. Try again." << endl;
                                system("pause");
                        }
                    } while(choice != 6);
                }
                break;

            case 3:
                atm.save();
                cout << "\nThank you for using eVault. Goodbye!" << endl;
                break;

            default:
                cout << "\nInvalid choice. Try again." << endl;
                system("pause");
        }

    } while(loginChoice != 3);

    return 0;
}