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
#define MAX_HISTORY 20

using namespace std;

struct Transaction {
    string type;
    double amount;
    double balanceAfter;
    string date;
};

struct User{
    string accountName;
    string accountNumber;
    string birthday;
    string encryptedPin;
    string contactNumber;
    double balance;
    string secQuestion;
    string encSecAnswer;
    Transaction history[MAX_HISTORY];
    int histCount;
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
        bool nameValidation(string name);
        bool monthValidation(int month);
        bool dayValidation(int day, int month, int year);
        bool yearValidation(int year);
        bool contactValidation(string number);
        string getCurrentDate();
        void addHistory(int idx, string type, double amount, double balAfter);

    public:
        bool existingUser();
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
        int  locate(string n);
        void save();
        void retrieve();
        void recoverPin();
        void miniStatement();
};

bool AccountManager :: existingUser(){
    string existingAccNum, existingEncPin;
    if(retrieveDrive(existingAccNum, existingEncPin)){  
        int pos = locate(existingAccNum);
        if(pos != -1){
            return true;
        }
    }
    return false;
}

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
    ofstream file(path + "\\pin.code");
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
        return false;
    }
    ifstream file(path + "\\pin.code");
    if(!file){
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

bool AccountManager :: nameValidation(string name){
    if(name.empty()){
        cout << "Name cannot be empty." << endl;
        system("pause");
        return false;
    }
    if(name.size() > 50){
        cout << "Name is too long **Maximum 50 Characters**" << endl;
        system("pause");
        return false;
    }
    bool hasLetter = false;
    for(int i = 0; i < (int)name.size(); i++){
        char c = name[i];
        if(isalpha(c)){
            hasLetter = true;
        }
        else if(c == ' ' || c == '.' || c == '\'' || c == '-'){
            continue;
        }
        else{
            cout << "Invalid Name **Must Only Contain Letters, Spaces, and ' . - **" << endl;
            system("pause");
            return false;
        }
    }
    if(!hasLetter){
        cout << "Name must contain at least one letter." << endl;
        system("pause");
        return false;
    }
    return true;
}

bool AccountManager :: monthValidation(int month){
    if(month < 1 || month > 12){
        cout << "Invalid Month **Must Be Between 1-12**" << endl;
        system("pause");
        return false;
    }
    return true;
}

bool AccountManager :: yearValidation(int year){
    if(year < 1900 || year > 2026){
        cout << "Invalid Year **Must Be Between 1900-2026**" << endl;
        system("pause");
        return false;
    }
    return true;
}

bool AccountManager :: dayValidation(int day, int month, int year){
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int maxDay = daysInMonth[month - 1];
    if(month == 2 && leapYear){
        maxDay = 29;
    }
    if(day < 1 || day > maxDay){
        cout << "Invalid Day **Day Does Not Exist For This Month**" << endl;
        system("pause");
        return false;
    }
    return true;
}

bool AccountManager :: contactValidation(string contact){
    if(contact.size() != 11){
        cout << "Invalid Contact Number **Must Be Exactly 11 Digits**" << endl;
        system("pause");
        return false;
    }
    if(contact[0] != '0' || contact[1] != '9'){
        cout << "Invalid Contact Number **Must Start With 09**" << endl;
        system("pause");
        return false;
    }
    for(int i = 0; i < (int)contact.size(); i++){
        if(!isdigit(contact[i])){
            cout << "Invalid Contact Number **Must Only Contain Digits**" << endl;
            system("pause");
            return false;
        }
    }
    return true;
}

string AccountManager :: passwordEncryptor(string pin){
    string encpin = "";
    for(int i = 0; i < (int)pin.size(); i++){
        encpin += (((pin[i] - '0') + 3) % 10) + '0';
    }
    return encpin;
}

string AccountManager :: getCurrentDate(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[20];
    sprintf(buf, "%02d/%02d/%04d %02d:%02d",
        ltm->tm_mon + 1, ltm->tm_mday, 1900 + ltm->tm_year,
        ltm->tm_hour, ltm->tm_min);
    return string(buf);
}

void AccountManager :: addHistory(int idx, string type, double amount, double balAfter){
    if(person[idx].histCount >= MAX_HISTORY){
        for(int i = 0; i < MAX_HISTORY - 1; i++)
            person[idx].history[i] = person[idx].history[i + 1];
        person[idx].histCount = MAX_HISTORY - 1;
    }
    int h = person[idx].histCount;
    person[idx].history[h].type         = type;
    person[idx].history[h].amount       = amount;
    person[idx].history[h].balanceAfter = balAfter;
    person[idx].history[h].date         = getCurrentDate();
    person[idx].histCount++;
}

void AccountManager :: save(){
    ofstream file("UserDatabase.csv");
    if(!file){
        cout << "Filename not Found" << endl;
        return;
    }
    file << "AccountName,AccountNumber,Birthday,EncryptedPin,ContactNumber,Balance,SecQuestion,EncSecAnswer,HistCount";
    for(int h = 0; h < MAX_HISTORY; h++)
        file << ",HType" << h << ",HAmount" << h << ",HBal" << h << ",HDate" << h;
    file << endl;
    for(int i = 0; i <= last; i++){
        file << person[i].accountName    << ","
             << person[i].accountNumber  << ","
             << person[i].birthday       << ","
             << person[i].encryptedPin   << ","
             << person[i].contactNumber  << ","
             << fixed << setprecision(2) << person[i].balance << ","
             << person[i].secQuestion    << ","
             << person[i].encSecAnswer   << ","
             << person[i].histCount;
        for(int h = 0; h < MAX_HISTORY; h++){
            if(h < person[i].histCount){
                file << "," << person[i].history[h].type
                     << "," << person[i].history[h].amount
                     << "," << person[i].history[h].balanceAfter
                     << "," << person[i].history[h].date;
            } else {
                file << ",,,,";
            }
        }
        file << endl;
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

    getline(file, line);

    while(getline(file, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string tmp;
        filedata.histCount = 0;
        getline(ss, filedata.accountName,   ',');
        getline(ss, filedata.accountNumber, ',');
        getline(ss, filedata.birthday,      ',');
        getline(ss, filedata.encryptedPin,  ',');
        getline(ss, filedata.contactNumber, ',');
        getline(ss, strBalance,             ',');
        if(strBalance.empty()) continue;
        filedata.balance = stod(strBalance);
        getline(ss, filedata.secQuestion,   ',');
        getline(ss, filedata.encSecAnswer,  ',');
        getline(ss, tmp,                    ',');
        filedata.histCount = tmp.empty() ? 0 : stoi(tmp);
        for(int h = 0; h < MAX_HISTORY; h++){
            string htype, hamount, hbal, hdate;
            getline(ss, htype,   ',');
            getline(ss, hamount, ',');
            getline(ss, hbal,    ',');
            getline(ss, hdate,   ',');
            if(h < filedata.histCount && !htype.empty()){
                filedata.history[h].type         = htype;
                filedata.history[h].amount       = hamount.empty() ? 0 : stod(hamount);
                filedata.history[h].balanceAfter = hbal.empty()    ? 0 : stod(hbal);
                filedata.history[h].date         = hdate;
            }
        }
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

    User x;
    x.histCount = 0;
    string accNum;
    string rawPin;
    int bMonth, bDay, bYear;

    cout << "========================================" << endl;
    cout << "        NEW ACCOUNT REGISTRATION        " << endl;
    cout << "========================================" << endl;
    do{
        cout << "Full Name: ";
        getline(cin, x.accountName);
    } while(!nameValidation(x.accountName));

    do{
        cout << "Birth Month (1-12): ";
        cin >> bMonth;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            bMonth = 0;
        }
    } while(!monthValidation(bMonth));

    do{
        cout << "Birth Year: ";
        cin >> bYear;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            bYear = 0;
        }
    } while(!yearValidation(bYear));

    do{
        cout << "Birth Day: ";
        cin >> bDay;
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            bDay = 0;
        }
    } while(!dayValidation(bDay, bMonth, bYear));
    x.birthday = (bMonth < 10 ? "0" : "") + to_string(bMonth) + "/" + (bDay < 10 ? "0" : "") + to_string(bDay) + "/" + to_string(bYear);

    cin.ignore(1000, '\n');
    do{
        cout << "Contact Number (e.g. 09171234567): ";
        getline(cin, x.contactNumber);
    } while(!contactValidation(x.contactNumber));

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
            cout << "Amount is below the minimum. Initial deposit must be at least PHP 5000.00" << endl;
        }
    } while(x.balance < 5000);

    do{
        accNum = to_string(10000 + rand() % 90000);
    } while(locate(accNum) != -1);
    x.accountNumber = accNum;

    cin.ignore(1000, '\n');
    cout << "\n--- SECURITY QUESTION SETUP ---" << endl;
    cout << "Choose a security question:" << endl;
    cout << "[1] What is your mother's maiden name?" << endl;
    cout << "[2] What is the name of your first pet?" << endl;
    cout << "[3] What is your elementary school name?" << endl;
    cout << "[4] What is your favorite childhood nickname?" << endl;
    int qChoice = 0;
    do{
        cout << "Select [1-4]: ";
        cin >> qChoice;
        if(cin.fail()){ cin.clear(); cin.ignore(1000, '\n'); qChoice = 0; }
    } while(qChoice < 1 || qChoice > 4);
    string questions[] = {
        "What is your mother's maiden name?",
        "What is the name of your first pet?",
        "What is your elementary school name?",
        "What is your favorite childhood nickname?"
    };
    x.secQuestion = questions[qChoice - 1];
    cin.ignore(1000, '\n');
    string secAnswer;
    cout << x.secQuestion << " ";
    getline(cin, secAnswer);
    for(int i = 0; i < (int)secAnswer.size(); i++) secAnswer[i] = tolower(secAnswer[i]);
    x.encSecAnswer = passwordEncryptor(secAnswer.size() >= 4 ? secAnswer.substr(0, 4) : secAnswer);

    do{
        cout << "Set a 4-digit PIN: ";
        cin >> rawPin;
    } while(!pinValidation(rawPin));
    x.encryptedPin = passwordEncryptor(rawPin);

    last++;
    person[last] = x;
    addHistory(last, "ACCOUNT OPENED", x.balance, x.balance);

    if(!saveDrive(accNum, x.encryptedPin)){
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
        cout << "Unable to read card data." << endl;
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
        system("pause");
        return false;
    }

    cout << "========================================" << endl;
    cout << "                 LOGIN                  " << endl;
    cout << "========================================" << endl;
    string enteredPin;
    cout << "Enter your PIN: ";
    cin >> enteredPin;

    if(!pinValidation(enteredPin)){
        return false;
    }

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
    addHistory(currentUser, "DEPOSIT", amount, person[currentUser].balance);
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
    addHistory(currentUser, "WITHDRAW", amount, person[currentUser].balance);
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
    addHistory(currentUser, "TRANSFER OUT to "  + person[index].accountNumber,       amount, person[currentUser].balance);
    addHistory(index,       "TRANSFER IN from " + person[currentUser].accountNumber,  amount, person[index].balance);
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

    if(!saveDrive(person[currentUser].accountNumber, newEncPin)){
        cout << "\nWarning: PIN changed in system, but failed to update the card." << endl;
        system("pause");
        return;
    }

    save();

    cout << "\nPIN changed successfully." << endl;
    system("pause");
}

void AccountManager :: recoverPin(){
    system("cls");
    string cardAccNum, cardEncPin;
    if(!retrieveDrive(cardAccNum, cardEncPin)){
        cout << "Unable to read card." << endl;
        system("pause");
        return;
    }
    int pos = locate(cardAccNum);
    if(pos == -1){
        cout << "Account not found." << endl;
        system("pause");
        return;
    }
    cout << "========================================" << endl;
    cout << "             PIN RECOVERY               " << endl;
    cout << "========================================" << endl;
    cout << "Security Question: " << person[pos].secQuestion << endl;
    cin.ignore(1000, '\n');
    string answer;
    cout << "Your Answer: ";
    getline(cin, answer);
    for(int i = 0; i < (int)answer.size(); i++) answer[i] = tolower(answer[i]);
    string encAnswer = passwordEncryptor(answer.size() >= 4 ? answer.substr(0, 4) : answer);
    if(encAnswer != person[pos].encSecAnswer){
        cout << "\nIncorrect answer. Cannot recover PIN." << endl;
        system("pause");
        return;
    }
    string newPin, confirmPin;
    cout << "\nAnswer verified! Set a new 4-digit PIN: ";
    cin >> newPin;
    if(!pinValidation(newPin)) return;
    cout << "Confirm new PIN: ";
    cin >> confirmPin;
    if(newPin != confirmPin){
        cout << "\nPINs do not match." << endl;
        system("pause");
        return;
    }
    string newEncPin = passwordEncryptor(newPin);
    person[pos].encryptedPin = newEncPin;
    saveDrive(person[pos].accountNumber, newEncPin);
    save();
    cout << "\nPIN reset successfully! Please log in with your new PIN." << endl;
    system("pause");
}

void AccountManager :: miniStatement(){
    system("cls");
    if(currentUser == -1){
        cout << "No account is currently logged in." << endl;
        system("pause");
        return;
    }
    cout << fixed << setprecision(2);
    cout << "========================================" << endl;
    cout << "            MINI STATEMENT              " << endl;
    cout << "Account: " << person[currentUser].accountNumber << endl;
    cout << "Name   : " << person[currentUser].accountName   << endl;
    cout << "========================================" << endl;
    int count = person[currentUser].histCount;
    if(count == 0){
        cout << "No transactions yet." << endl;
        system("pause");
        return;
    }
    cout << left
         << setw(18) << "DATE"
         << setw(26) << "TYPE"
         << setw(12) << "AMOUNT"
         << "BALANCE" << endl;
    cout << string(68, '-') << endl;
    for(int i = count - 1; i >= 0; i--){
        cout << left
             << setw(18) << person[currentUser].history[i].date
             << setw(26) << person[currentUser].history[i].type
             << setw(12) << person[currentUser].history[i].amount
             << person[currentUser].history[i].balanceAfter << endl;
    }
    cout << "========================================" << endl;
    cout << "Current Balance: PHP " << person[currentUser].balance << endl;
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
    cout << "[6] Mini Statement" << endl;
    cout << "[7] Logout" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Input your Choice (1-7): ";
    cin >> choice;

    if(cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
        return 0;
    }
    return choice;
}

int main(){
    srand(time(0));

    AccountManager atm;

    if(!atm.driveConnected()){
        cout << "Please Insert an ATM Card to the Machine!" << endl;
        cout << "No drive detected." << endl;
        while(!atm.driveConnected()){
            atm.driveConnected();
        }
    }

    atm.retrieve();

    if(!atm.existingUser()){
        atm.registerAccount();
    }

    bool loggedIn = false;
    int attempts = 4;

    while(attempts > 0){
        system("cls");
        cout << "========================================" << endl;
        cout << "  eVault - \"Your Money. Digitally Secured\"" << endl;
        cout << "========================================" << endl;
        cout << "[1] Login" << endl;
        cout << "[2] Forgot PIN" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Select: ";
        int startChoice; cin >> startChoice;

        if(startChoice == 2){
            atm.recoverPin();
            continue;
        }

        if(atm.loginAccount()){
            loggedIn = true;
            break;
        }
        attempts--;
        if(attempts > 0){
            cout << attempts << " attempt(s) remaining." << endl;
            system("pause");
        }
    }

    if(!loggedIn){
        cout << "\n========================================" << endl;
        cout << "  Too many failed attempts." << endl;
        cout << "  Session terminated." << endl;
        cout << "========================================" << endl;
        atm.save();
        system("pause");
        return 0; 
    }

    int choice;
    do{
        choice = atm.menu();
        switch(choice){
            case 1: 
                atm.balanceInquiry(); 
                break;
            case 2: 
                atm.deposit();        
                break;
            case 3:    
                atm.withdraw();       
                break;
            case 4: 
                atm.fundTransfer();     
                break;
            case 5: 
                atm.changePin();      
                break;
            case 6:
                atm.miniStatement();
                break;
            case 7:
                atm.logout();
                cout << "\nLogged out. Thank you!" << endl;
                system("pause");
                break;
            default:
                cout << "\nInvalid choice. Try again." << endl;
                system("pause");
        }
    } while(choice != 7);

    atm.save();
    return 0;
}
