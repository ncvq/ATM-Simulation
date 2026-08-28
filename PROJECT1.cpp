#include <iostream>
using namespace std;

struct AccountDetails{
    string accountName;
    string accountNumber;
    string birthday;
    string encryptedPin;
    string contactNumber;
    double balance;
};

struct Node{
    AccountDetails data;
    Node *next;
    Node(AccountDetails x){
        data = x;
        next = NULL;
    }
};

class AccountManager{
    private:
        Node* head;
    
    public:
        AccountManager() {
        head = NULL;
    }
    ~AccountManager() {
        Node* p;
        while (head != NULL) {
            p = head;
            head = head->next;
            delete p;
        }
    }

        bool registerAccount();
        bool loginAccount();
        void changePin();
        void fundTransfer();
        void balanceInquiry();
        bool withdraw(); // to return if transaction is successfull
        bool deposit(); // since you could just check the balance in balanceInquiry pero pwede rin naman double ewan

};