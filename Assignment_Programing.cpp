#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct User {
    string name;
    double points;
    double amount;
};

struct Product {
    string name;
    int code;
    double price;
    double loyaltyRate;
};

const int MAX_USERS = 100;
User userList[MAX_USERS];
int userCount = 0;

Product productList[] = {
    {"T-shirt", 122, 2400, 0.30},
    {"Shirt", 114, 1850, 0.45},
    {"Short", 156, 1250, 0.55},
    {"Trouser", 124, 3600, 0.20},
    {"Wallet", 144, 650, 0.05},
    {"Jacket", 155, 2300, 0.40},
    {"Cap", 166, 1050, 0.35},
    {"Belt", 177, 500, 0.25},
    {"Underwear", 188, 1300, 0.10},
    {"Sock", 125, 400, 0.22},
    {"Shoe", 234, 7500, 0.50},
    {"Parfum", 359, 1900, 0.15}
};

void loadUsers();
void saveUser(const User &user);
void buyItem();
void showPoints();
void redeemPoints();
void displayMenu();
int findUserIndex(const string &name);
Product* findProductByCode(int code);

int main() {
    loadUsers();
    int option;

    do {
        displayMenu();
        cin >> option;

        switch (option) {
            case 1:
                buyItem();
                break;
            case 2:
                showPoints();
                break;
            case 3:
                redeemPoints();
                break;
            case 4:
                cout << "Exiting program. Thank you!" << endl;
                break;
            default:
                cout << "Invalid selection. Please try again." << endl;
        }
    } while (option != 4);

    return 0;
}

void displayMenu() {
    cout << "\n++ Elegance Boutique ++\n";
    cout << "1. Buy Item\n";
    cout << "2. Show Points\n";
    cout << "3. Redeem Points\n";
    cout << "4. Exit\n";
    cout << "Enter Your Selection: ";
}

void loadUsers() {
    ifstream inFile("users.txt");
    if (!inFile) {
        cout << "No previous user data found." << endl;
        return;
    }

    while (userCount < MAX_USERS && inFile >> userList[userCount].name >> userList[userCount].points >> userList[userCount].amount) {
        if (!userList[userCount].name.empty()) {
            userCount++;
        }
    }
    inFile.close();
}

void saveUser(const User &user) {
    ofstream outFile("users.txt", ios::app);
    outFile << user.name << " " << user.points << " " << user.amount << "\n";
    outFile.close();
}

void buyItem() {
    string name;
    cout << "Enter Customer Name: ";
    cin >> name;

    int userIndex = findUserIndex(name);
    if (userIndex == -1) {
        userIndex = userCount++;
        userList[userIndex].name = name;
        userList[userIndex].points = 0;
        userList[userIndex].amount = 0;
    }

    User &user = userList[userIndex];

    cout << "\nAvailable Products:\n";
    cout << "Item\t\tCode\tPrice (Rs.)\tLoyalty Rate\n";
    for (const auto &product : productList) {
        cout << left << setw(12) << product.name
             << "\t" << product.code
             << "\t" << product.price
             << "\t\t" << product.loyaltyRate << endl;
    }

    int code, quantity;
    double totalAmount = 0, earnedPoints = 0;
    while (true) {
        cout << "\nEnter Item Code (-1 to finish): ";
        cin >> code;
        if (code == -1) break;

        Product *product = findProductByCode(code);
        if (!product) {
            cout << "Invalid code. Please try again." << endl;
            continue;
        }

        cout << "Enter Quantity: ";
        cin >> quantity;

        if (quantity < 0) {
            cout << "Quantity cannot be negative. Please try again." << endl;
            continue;
        }

        totalAmount += product->price * quantity;
        earnedPoints += product->price * quantity * product->loyaltyRate;
    }

    user.amount += totalAmount;
    user.points += earnedPoints;

    cout << "Total Purchase Amount: Rs. " << totalAmount << endl;
    cout << "Earned Loyalty Points: " << earnedPoints << endl;
    saveUser(user);
}

void showPoints() {
    string name;
    cout << "Enter Customer Name: ";
    cin >> name;

    int userIndex = findUserIndex(name);
    if (userIndex == -1) {
        cout << "Customer not found." << endl;
        return;
    }

    cout << "Current Points for " << name << ": " << userList[userIndex].points << endl;
}

void redeemPoints() {
    string name;
    cout << "Enter Customer Name: ";
    cin >> name;

    int userIndex = findUserIndex(name);
    if (userIndex == -1) {
        userIndex = userCount++;
        userList[userIndex].name = name;
        userList[userIndex].points = 0;
        userList[userIndex].amount = 0;
    }

    User &user = userList[userIndex];

    double redeemableAmount = user.points;
    if (redeemableAmount == 0) {
        cout << "No points available for redemption." << endl;
        return;
    }

    cout << "Redeemable Points: " << user.points << endl;
    user.amount -= redeemableAmount;
    user.points = 0;

    cout << "Amount after Redemption: Rs. " << user.amount << endl;

    ofstream outFile("users.txt");
    for (int i = 0; i < userCount; ++i) {
        outFile << userList[i].name << " " << userList[i].points << " " << userList[i].amount << "\n";
    }
    outFile.close();
}

int findUserIndex(const string &name) {
    for (int i = 0; i < userCount; ++i) {
        if (userList[i].name == name) {
            return i;
        }
    }
    return -1;
}

Product* findProductByCode(int code) {
    for (auto &product : productList) {
        if (product.code == code) {
            return &product;
        }
    }
    return nullptr;
}