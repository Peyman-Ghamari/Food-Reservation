#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include "json.hpp"
#include <string>
using json = nlohmann::json;
using namespace std;
// Define Our Functions
void saveJsonToFile(const json& data, const std::string& fileName);
void appendDataToJsonFile(const json& newData, const std::string& fileName);
json loadJsonFromFile(const std::string& fileName);

bool checkAdminCredentials(const std::string &ad_name, const std::string &ad_password);
bool checkUserCredentials(const std::string &uni_id, const std::string &user_password);

void displayJsonAsTableGroupedByDate(const std::string &fileName);
void displayArrayAsTable(const json &array, const std::string &title);
void displayUserInfo(const std::string &uni_id, const std::string &user_password);
void displayFoodByDate(const string &foodDate);
void displayUsersInfo();
void displayAverageFoodRatings();

void addFoodToJson();
void removeFoodByDate();

void reserveFood(const string &userUniId);
void cancelReservation(const string &userUniId);
void increaseUserBalance(const string &targetUniId);
void changeUserPassword(const string &uniId);
void rateFood(const string &uniId);

void addUser();
void deleteUserByUniId();


void programmerInfo();

#endif // MY_FUNCTIONS_H
