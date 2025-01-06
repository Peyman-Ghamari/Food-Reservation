#include <iostream>
#include <fstream>
#include "json.hpp"
#include "my_functions.h"  // هدر توابع کمکی
using json = nlohmann::json;
using namespace std;
void adminMenu() {
    string food_name, food_date, target_date, uni_id, user_password, user_name, user_last_name;
    int ad_req;
    char ad_req2;
    bool keepGoing = true;
    while (keepGoing) {
        cout << "\n=== Admin Menu ===" << endl;
        cout << "1. See the Food Schedule" << endl;
        cout << "2. Set the Food" << endl;
        cout << "3. Delete Food" << endl;
        cout << "4. See All Users Info" << endl;
        cout << "5. Create New User" << endl;
        cout << "6. Delete A User" << endl;
        cout << "7. See Foods Rating" << endl;
        cout << "Enter Your Choice: ";
        cin >> ad_req;
        switch (ad_req) {
            case 1: {
                displayJsonAsTableGroupedByDate("food_reserved.json");
                break;
            }
            case 2: {
                addFoodToJson();
                break;
            }
            case 3: {
                removeFoodByDate();
                break;
            }
            case 4: {
                displayUsersInfo();
                break;
            }
            case 5: {
                addUser();
                break;
            }
            case 6: {
                deleteUserByUniId();
                break;
            }
            case 7: {
                displayAverageFoodRatings();
            }
            default: {
                cout << "Invalid Option. Please Choose A Valid Option." << endl;
                break;
            }
        }
        cout << "Do You Want To Perform Another Action? (Y/N): ";
        cin >> ad_req2;
        if (ad_req2 == 'n' || ad_req2 == 'N') {
            keepGoing = false;
            cout << "Exiting Admin Menu." << endl;
        }
    }
}
void userMenu(const string &uni_id, const string &user_password) {
    int us_req;
    char us_req2;
    bool keepGoing = true;
    string foodDate;
    while (keepGoing) {
        cout << "\n=== User Menu ===" << endl;
        cout << "1. Your Profile Schedule " << endl;
        cout << "2. Reserve Food " << endl;
        cout << "3. Cancel Your Food Reservation" << endl;
        cout << "4. Add Balance " << endl;
        cout << "5. Change Your Password " << endl;
        cout << "6. Rate Your Food "<<endl;
        cout << "Your Choice: ";
        cin >> us_req;
        switch (us_req) {
            case 1: {
                displayUserInfo(uni_id, user_password);
                break;
            }
            case 2: {
                reserveFood(uni_id);
                break;
            }
            case 3: {
                cancelReservation(uni_id);
                break;
            }
            case 4: {
                increaseUserBalance(uni_id);
                break;
            }
            case 5: {
                changeUserPassword(uni_id);
                break;
            }
            case 6: {
                rateFood(uni_id);
                break;
            }
            case 0: {
                keepGoing = false;
                cout << "Exiting User Menu." << endl;
                break;
            }
            default: {
                cout << "Invalid Option!" << endl;
                break;
            }
        }
        cout << "Do You Want To Perform Another Action? (Y/N): ";
        cin >> us_req2;

        if (us_req2 == 'n' || us_req2 == 'N') {
            keepGoing = false;
            cout << "Exiting Admin Menu." << endl;
        }
        }
}
int main() {
    bool x = true;
    string ad_password, ad_name, uni_id, user_password ;
    while (x) {
        int req;
        cout << "1. LogIn As Admin" << endl;
        cout << "2. LogIn As User" << endl;
        cout << "3. Programmer Info"<<endl;
        cout << "Enter 0 To Exit" << endl;
        cout << "Your Choice: ";
        cin >> req;
        switch (req) {
            case 1: {
                cout << "Enter Admin UserName: ";
                cin >> ad_name;
                cout << "Enter Admin Password: ";
                cin >> ad_password;

                if (checkAdminCredentials(ad_name, ad_password)) {
                    cout << "Admin Credentials Verified Successfully." << endl;
                    adminMenu();
                } else {
                    cout << "Invalid Admin Credentials!" << endl;
                }
                break;
            }
            case 2: {
                cout << "For LogIn As User Enter Your University Id: ";
                cin >> uni_id;
                cout << "Enter Your Password : " <<endl;
                cin >> user_password;
            if (checkUserCredentials(uni_id, user_password)) {
                cout << "User Credentials Verified Successfully." << endl;
                userMenu(uni_id,user_password);
            }else {
                cout << "Invalid User Credentials!" << endl;
            }
            break;
            }
            case 3: {
                programmerInfo();
                break;
            }
            case 0:
                cout << "Exiting The Program..." << endl;
                x = false;
                break;

            default:
                cout << "Invalid Request!" << endl;
                break;
        }
    }
    return 0;
}
