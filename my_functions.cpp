#include <iostream>
#include <fstream>
#include "json.hpp"
#include <iomanip>

using namespace std;
using json = nlohmann::json;

// For Loading Json File
json loadJsonFromFile(const string &fileName) {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        throw runtime_error("Could not open file for reading.");
    }
    json db;
    try {
        inFile >> db; // تلاش برای خواندن JSON
    } catch (const json::parse_error &e) {
        cerr << "JSON Parse Error: " << e.what() << endl;
        throw; // ارسال مجدد خطا
    }
    inFile.close();
    return db;
}


// For Saving JSOOn
void saveJsonToFile(const string &fileName, const json &db) {
    ofstream outFile(fileName, ios::trunc); // حالت trunc برای پاک کردن محتوای قدیمی
    if (!outFile.is_open()) {
        throw runtime_error("Could not open file for writing.");
    }
    outFile << setw(4) << db; // تنظیم نمایش با فرمت خوانا
    outFile.close(); // بستن فایل پس از ذخیره
}


// Add Food To JSON
void addFoodToJson() {
    try {
        // Load JSON
        json db = loadJsonFromFile("food_reserved.json");

        string foodDate, foodName;
        int foodPrice;

        // Get Information From Admin To Set Food
        cout << "Enter the date you want to set food (e.g., 1403/09/15): ";
        cin >> foodDate;
        cout << "Enter the food name: ";
        cin >> foodName;
        cout << "Enter the food price: ";
        cin >> foodPrice;

        // Checking Inputs
        if (foodDate.empty() || foodName.empty()) {
            cerr << "Error: Food date and name cannot be empty." << endl;
            return;
        }

        if (foodPrice <= 0) {
            cerr << "Error: Food price must be greater than 0." << endl;
            return;
        }
        //بررسی کردن اینکه نام و تاریخی که کاربر وارد کرده از قبل وجود نداشته باشد (تکراری نباشه)
        // Check if food with the same name and date already exists
        if (db.contains("food_info") && db["food_info"].is_array()) {
            for (const auto &food : db["food_info"]) {
                if (food["food_date"] == foodDate && food["food_name"] == foodName) {
                    cout << "Error: Food with the same name and date already exists." << endl;
                    return;
                }
            }
        }
        //ساخت شی جدید از غذاهای گرفته شده از ادمین
        // Create New OBJ From Food
        json newFood = {
            {"food_date", foodDate},
            {"food_name", foodName},
            {"food_price", foodPrice},
            {"users_reserved", json::array()},
            {"users_canceled", json::array()},
            {"users_rating",json::array()}
        };

        // Add Food Into [food_info] Array
        if (db.contains("food_info") && db["food_info"].is_array()) {
            db["food_info"].push_back(newFood);
        } else {
            db["food_info"] = json::array({newFood});
        }

        // Saving Data In JSON
        saveJsonToFile("food_reserved.json", db);

        cout << "Food successfully added to the JSON file." << endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// پیاده‌سازی تابع appendDataToJsonFile
void appendDataToJsonFile(const json& newData, const std::string& fileName) {
    json existingData = loadJsonFromFile(fileName);  // Loading Old Data
    //یبررسی اینکه کلید "uer_info" را دارا هست یا خیر سپس در صورت وجود نداشتن آنرا ایجاد میکنیم
    //Checking For Having "user_info" Key , And Create It If It Doesn't Exist
    if (existingData.find("users_info") == existingData.end()) {
        existingData["users_info"] = json::array();  // Make A New Empty Array For "user_info"
    }
    //Cheking For If "user_info" Doesn't An Array ,Then Make It To An Array
    // بررسی اینکه اگر "user_info" آرایه نیست، آن را به آرایه تبدیل کنیم
    else if (!existingData["users_info"].is_array()) {
        existingData["users_info"] = json::array({existingData["users_info"]});  // تبدیل به آرایه
    }

    // اضافه کردن داده جدید به آرایه "user_info"
    //Add New Data Into "user_info" Array
    existingData["users_info"].push_back(newData);

    saveJsonToFile(existingData, fileName);  // ذخیره دوباره داده‌ها
}


bool checkAdminCredentials(const std::string &ad_name, const std::string &ad_password) {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("admin.json");

        // بررسی وجود کلید "admin_info"
        // Checking For Having "admin_info"
        if (db.contains("admin_info") && db["admin_info"].is_object()) {
            const auto &admin = db["admin_info"];
            // بررسی مقادیر admin_name و admin_password
            if (admin.contains("admin_name") && admin.contains("admin_password")) {
                if (admin["admin_name"] == ad_name && admin["admin_password"] == ad_password) {
                    return true;
                    //If Data Was Match Func Return True
                    // اگر مقادیر مطابقت داشتند
                }
            }
        }
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return false;
    //If Wasn't Match The Func Returns False
    //در صورت عدم مطابقت
}


bool checkUserCredentials(const std::string &uni_id, const std::string &user_password) {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("users.json");

        // بررسی وجود کلید users_info
        // Checking For Having "user_info"
        if (db.contains("users_info") && db["users_info"].is_array()) {
            const auto &users = db["users_info"];
            for (const auto &user : users) {
                // بررسی مقادیر uni_id و user_password
                // Validating "user_password", "uni_id"
                if (user.contains("uni_id") && user.contains("user_password")) {
                    if (user["uni_id"] == uni_id && user["user_password"] == user_password) {
                        return true; // اگر مقادیر مطابقت داشتند
                    }
                }
            }
        }
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return false; // در صورت عدم مطابقت
}

// تابع برای نمایش آرایه به صورت جدول
//Display Array
void displayArrayAsTable(const json &array, const std::string &title) {
    if (!array.is_array()) {
        std::cerr << title << " is not an array!" << std::endl;
        return;
    }

    // چاپ عنوان جدول
    std::cout << "\n" << title << ":\n";
    std::cout << std::setw(15) << std::left << "Key"
              << std::setw(20) << "Value" << std::endl;
    std::cout << std::string(35, '-') << std::endl;

    // پردازش هر شیء در آرایه
    for (const auto &item : array) {
        if (item.is_object()) {
            for (auto it = item.begin(); it != item.end(); ++it) {
                std::cout << std::setw(15) << std::left << it.key()
                          << std::setw(20) << it.value() << std::endl;
            }
            std::cout << std::string(35, '-') << std::endl; // خط جداکننده برای هر شیء
        }
    }
}


void displayJsonAsTableGroupedByDate(const std::string &fileName) {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile(fileName);

        // بررسی وجود food_info
        if (db.contains("food_info") && db["food_info"].is_array()) {
            // Sort By Date
            // مرتب‌سازی غذاها بر اساس تاریخ
            //Sort Food By Their Date
            auto foodInfo = db["food_info"];
            std::sort(foodInfo.begin(), foodInfo.end(), [](const json &a, const json &b) {
                return a.value("food_date", "") < b.value("food_date", "");
            });

            std::string currentDate;

            for (const auto &food : foodInfo) {
                std::string foodDate = food.value("food_date", "N/A");

                // اگر تاریخ جدید است، عنوان جدیدی چاپ شود
                //If It Is A New Date , Make A New One
                if (foodDate != currentDate) {
                    currentDate = foodDate;
                    cout << "\n+---------------+--------------------+----------+-----------------------+" << endl;
                    cout << "| Food Date     : " << currentDate << setw(37 - currentDate.size()) << " " << "|" << endl;
                    cout << "+---------------+--------------------+----------+-----------------------+" << endl;
                    cout << "| Food Name     | Price             | User Information              |" << endl;
                    cout << "+---------------+--------------------+----------+-----------------------+" << endl;
                }

                // اطلاعات کلی غذا
                // All Food Data's Information
                cout << "| " << setw(13) << left << food.value("food_name", "N/A")
                     << "| " << setw(18) << left << food.value("food_price", 0)
                     << "| Reserved Users:                  |" << endl;

                // کاربران رزروکننده
                // User Who Reserved
                if (food.contains("users_reserved") && food["users_reserved"].is_array()) {
                    for (const auto &user : food["users_reserved"]) {
                        cout << "|               |                    | "
                             << setw(10) << left << user.value("user_name", "N/A")
                             << setw(10) << left << user.value("user_lastName", "N/A")
                             << setw(15) << left << user["uni_id"].dump() << " |" << endl;
                    }
                }

                // کاربران کنسل‌کننده
                //User Who Canceled
                cout << "|               |                    | Canceled Users:               |" << endl;
                if (food.contains("users_canceled") && food["users_canceled"].is_array()) {
                    for (const auto &user : food["users_canceled"]) {
                        cout << "|               |                    | "
                             << setw(10) << left << user.value("user_name", "N/A")
                             << setw(10) << left << user.value("user_lastName", "N/A")
                             << setw(15) << left << user["uni_id"].dump() << " |" << endl;
                    }
                }

                // خط جداکننده
                //Separator Line
                cout << "+---------------+--------------------+----------+-----------------------+" << endl;
            }
        } else {
            cout << "No food_info data found!" << endl;
        }
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


void removeFoodByDate() {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("food_reserved.json");

        if (!db.contains("food_info") || !db["food_info"].is_array()) {
            cout << "Error: No valid 'food_info' array found in the JSON file." << endl;
            return;
        }

        string targetDate;
        cout << "Enter The Date (e.g., 1403/09/18): ";
        cin >> targetDate;

        auto &foodArray = db["food_info"];
        size_t initialSize = foodArray.size();

        // حذف غذاها با تاریخ مشخص
        //Delete Set Food By Admin With Get Date
        foodArray.erase(
            remove_if(foodArray.begin(), foodArray.end(), [&](const json &food) {
                return food.contains("food_date") && food["food_date"] == targetDate;
            }),
            foodArray.end()
        );

        size_t finalSize = foodArray.size();

        if (initialSize == finalSize) {
            cout << "No ٍٍٍٍٍٍٍٍEntries Found With Date " << targetDate << "." << endl;
        } else {
            // ذخیره‌سازی تغییرات در فایل
            //Saving Edition in File
            saveJsonToFile("food_reserved.json", db);
            cout << "Entries With Date " << targetDate << " Have Been Removed Successfully!" << endl;
        }

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


void displayUserInfo(const std::string &uni_id, const std::string &user_password) {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("users.json");

        //Checking For Having "user_info" Key
        // بررسی وجود کلید "users_info"
        if (db.contains("users_info") && db["users_info"].is_array()) {
            const auto &users = db["users_info"];
            for (const auto &user : users) {
                // بررسی مقادیر user_name و user_password
                //Validating "user_password" , "user_name"
                if (user.contains("uni_id") && user.contains("user_password")) {
                    if (user["uni_id"] == uni_id && user["user_password"] == user_password) {
                        // نمایش اطلاعات کاربر
                        //Display Users Information
                        cout << "+-------------------+----------------------------+" << endl;
                        cout << "| Field            | Value                      |" << endl;
                        cout << "+-------------------+----------------------------+" << endl;
                        cout << "| Name             | " << setw(26) << left << user["user_name"] << " |" << endl;
                        cout << "| Last Name        | " << setw(26) << left << user["user_lastName"] << " |" << endl;
                        cout << "| University ID    | " << setw(26) << left << user["uni_id"] << " |" << endl;
                        cout << "| Balance          | " << setw(26) << left << user["user_balance"] << " |" << endl;
                        cout << "+-------------------+----------------------------+" << endl;

                        // نمایش اطلاعات غذاهای رزرو شده
                        //Display Food Reserved Information
                        if (user.contains("reserved_foods") && user["reserved_foods"].is_array()) {
                            cout << "Reserved Foods:" << endl;
                            cout << "+------------------+------------+----------+" << endl;
                            cout << "| Food Name        | Food Date  | Price    |" << endl;
                            cout << "+------------------+------------+----------+" << endl;
                            for (const auto &food : user["reserved_foods"]) {
                                cout << "| " << setw(16) << left << food["food_name"]
                                     << "| " << setw(10) << left << food["food_date"]
                                     << "| " << setw(8) << left << food["food_price"] << " |" << endl;
                            }
                            cout << "+------------------+------------+----------+" << endl;
                        }
                        return; // خروج از تابع پس از نمایش اطلاعات
                    }
                }
            }
        }
        cout << "User not found or invalid credentials!" << endl;
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


void displayFoodByDate(const string &foodDate) {
    try {
        // بارگذاری فایل غذاها
        json db = loadJsonFromFile("food_reserved.json");

        bool foodFound = false;

        // جستجو در آرایه food_info برای پیدا کردن غذاها بر اساس تاریخ
        //Searching In food_info Array For Match Wishing Date
        for (const auto &food : db["food_info"]) {
            if (food["food_date"] == foodDate) {
                foodFound = true;
                cout << "\nFoods for Date: " << foodDate << endl;
                cout << "+------------------+--------------------+----------+" << endl;
                cout << "| Food Name       | Food Price         | Reserved |" << endl;
                cout << "+------------------+--------------------+----------+" << endl;

                // نمایش غذاها
                cout << "| " << setw(16) << left << food["food_name"]
                     << "| " << setw(18) << left << food["food_price"]
                     << "| " << setw(8) << left << "Yes" << "   |" << endl;

                cout << "+------------------+--------------------+----------+" << endl;
            }
        }

        if (!foodFound) {
            cout << "No food found for the date: " << foodDate << endl;
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// تابع رزرو غذا
void reserveFood(const string &userUniId) {
    try {
        // بارگذاری فایل‌ها
        json foodDb = loadJsonFromFile("food_reserved.json");
        json userDb = loadJsonFromFile("users.json");

        string foodDate;
        cout << "Enter the Date of Food (e.g., 1403/09/15): ";
        cin >> foodDate;
        // پیدا کردن یوزر
        //Finding User Information
        auto userIt = find_if(userDb["users_info"].begin(), userDb["users_info"].end(), [&](const json &u) {
            return u["uni_id"] == userUniId;
        });

        if (userIt == userDb["users_info"].end()) {
            cout << "User not found!" << endl;
            return;
        }

        json &user = *userIt;

        // نمایش غذاها برای تاریخ مشخص‌شده
        //Display Food By Choose Date
        vector<int> availableFoodIndexes;
        cout << "\nAvailable Foods On " << foodDate << ":" << endl;
        cout << "+------------------+--------------------+----------+-------+" << endl;
        cout << "| Food Name       | Food Price         | Reserved | Index |" << endl;
        cout << "+------------------+--------------------+----------+-------+" << endl;

        int index = 0;
        for (size_t i = 0; i < foodDb["food_info"].size(); ++i) {
            const auto &food = foodDb["food_info"][i];
            if (food["food_date"] == foodDate) {
                string reserved = (food["users_reserved"].size() > 0) ? "Yes" : "No";
                cout << "| " << setw(16) << left << food["food_name"]
                     << "| " << setw(18) << left << food["food_price"]
                     << "| " << setw(8) << left << reserved
                     << "| " << setw(5) << left << index << " |" << endl;
                availableFoodIndexes.push_back(i);
                ++index;
            }
        }

        if (availableFoodIndexes.empty()) {
            cout << "No Food Found For This Date!" << endl;
            return;
        }

        // گرفتن ورودی انتخاب غذا از کاربر
        // Food Choice Input
        int foodChoice;
        cout << "Enter The Number Of Food You Want To Reserve: ";
        cin >> foodChoice;

        if (foodChoice < 0 || foodChoice >= static_cast<int>(availableFoodIndexes.size())) {
            cout << "Invalid Choice!" << endl;
            return;
        }

        // پیدا کردن غذای انتخابی
        //Finding Chosen Food
        int foodIndex = availableFoodIndexes[foodChoice];
        json &selectedFood = foodDb["food_info"][foodIndex];
        int price = selectedFood["food_price"].get<int>();

        // بررسی موجودی کاربر
        //Checking User Balance "user_balance"
        if (user["user_balance"].get<int>() < price) {
            cout << "Not enough balance!" << endl;
            return;
        }

        // کسر مبلغ از موجودی و به‌روزرسانی کاربر
        //Decrease User Balance with Chosen Food Price
        user["user_balance"] = user["user_balance"].get<int>() - price;

        //Add Food Information Into users.JSON In "reserved_food" Array
        // افزودن اطلاعات غذا به reserved_foods کاربر
        user["reserved_foods"].push_back({
            {"food_date", selectedFood["food_date"]},
            {"food_name", selectedFood["food_name"]},
            {"food_price", price}
        });

        // افزودن اطلاعات کاربر به users_reserved غذای انتخابی
        //Add User Information into food_reserved.JSON IN "users_reserved" Array
        selectedFood["users_reserved"].push_back({
            {"uni_id", user["uni_id"]},
            {"user_name", user["user_name"]},
            {"user_lastName", user["user_lastName"]}
        });

        // ذخیره تغییرات در فایل‌ها
        saveJsonToFile("food_reserved.json", foodDb);
        saveJsonToFile("users.json", userDb);

        cout << "Food Reserved Successfully For User: " << user["user_name"] << endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void cancelReservation(const string &userUniId) {
    try {
        // بارگذاری فایل‌ها
        json foodDb = loadJsonFromFile("food_reserved.json");
        json userDb = loadJsonFromFile("users.json");

        string foodDate, foodName;
        cout << "Enter The Date Of Food To Cancel (e.g., 1403/09/15): ";
        cin >> foodDate;
        cout << "Enter The Name Of Food To Cancel: ";
        cin >> foodName;

        // پیدا کردن یوزر
        //Finding User Filed
        auto userIt = find_if(userDb["users_info"].begin(), userDb["users_info"].end(), [&](const json &u) {
            return u["uni_id"] == userUniId;
        });

        if (userIt == userDb["users_info"].end()) {
            cout << "User not found!" << endl;
            return;
        }

        json &user = *userIt;

        // پیدا کردن غذای رزرو شده در لیست کاربر
        //Find Food Reserved In Users Filed
        auto reservedFoodIt = find_if(user["reserved_foods"].begin(), user["reserved_foods"].end(), [&](const json &f) {
            return f["food_date"] == foodDate && f["food_name"] == foodName;
        });

        if (reservedFoodIt == user["reserved_foods"].end()) {
            cout << "No Reservation Found For This Food!" << endl;
            return;
        }

        int refundAmount = reservedFoodIt->at("food_price").get<int>();

        // (user.json) بازگرداندن مبلغ به موجودی کاربر
        //Returning The Amount To The User's Balance (users.json)
        user["user_balance"] = user["user_balance"].get<int>() + refundAmount;

        // حذف غذا از لیست reserved_foods کاربر(users.json "resereved_foods" Section)
        // Deleting Food Reserved At User Filed From (users.json "resereved_foods" Section)
        user["reserved_foods"].erase(reservedFoodIt);

        // پیدا کردن غذای موردنظر در food_reserved.json
        // Finding Our Wanted Food
        auto foodIt = find_if(foodDb["food_info"].begin(), foodDb["food_info"].end(), [&](const json &f) {
            return f["food_date"] == foodDate && f["food_name"] == foodName;
        });

        if (foodIt == foodDb["food_info"].end()) {
            cout << "Food Not Found In Database!" << endl;
            return;
        }

        json &selectedFood = *foodIt;

        // حذف کاربر از users_reserved
        // Deleting Users Info From users_reserved Section
        auto reservedUserIt = find_if(selectedFood["users_reserved"].begin(), selectedFood["users_reserved"].end(), [&](const json &u) {
            return u["uni_id"] == user["uni_id"];
        });

        if (reservedUserIt != selectedFood["users_reserved"].end()) {
            selectedFood["users_reserved"].erase(reservedUserIt);
        }

        // افزودن کاربر به users_canceled
        selectedFood["users_canceled"].push_back({
            {"uni_id", user["uni_id"]},
            {"user_name", user["user_name"]},
            {"user_lastName", user["user_lastName"]}
        });

        // ذخیره تغییرات در فایل‌ها
        // Saving Changes
        saveJsonToFile("food_reserved.json", foodDb);
        saveJsonToFile("users.json", userDb);

        cout << "Reservation Canceled Successfully For User: " << user["user_name"] << " " <<user["user_lastName"]<< endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


void addUser() {
    try {
        // بارگذاری فایل users.json
        json userDb = loadJsonFromFile("users.json");

        // بررسی وجود کلید "users_info" برای جلوگیری از ارورها وقتی که Users.json خالی باشد (وقتی خالی باشه میاد خودش کلید user_info میسازه)
        //Checking For Having "users_info" Key TO Prevent From Errors When Users.Json IS Empty
        if (!userDb.contains("users_info") || !userDb["users_info"].is_array()) {
            userDb["users_info"] = json::array(); // ایجاد آرایه کاربران در صورت عدم وجود
        }

        // متغیرهای مربوط به اطلاعات کاربر
        string uni_id, user_name, user_last_name, user_password;
        int user_balance;

        // دریافت اطلاعات کاربر از ورودی
        // Get User Info From Admin
        cout << "Enter The University's Student Id: ";
        cin >> uni_id;
        cout << "Enter The University's Student Name: ";
        cin >> user_name;
        cout << "Enter The University's Student Last Name: ";
        cin >> user_last_name;
        cout << "Enter The University's Student Password: ";
        cin >> user_password;
        cout << "Enter The University's Student Balance: ";
        cin >> user_balance;

        //چک کردن کامل بودن اطلاعاتی که ادمین درباره یوزر میده
        //Checking For Getting Completely Info From Admin About User
        if (uni_id.empty() || user_name.empty() || user_last_name.empty() || user_password.empty()) {
            cout<<"Error: All Fields Must Be Filled Out."<<endl;
            return;
        }
        if (user_balance < 0) {
            cout<<"Error: User Balance Must Be At Least 0 Or Be More"<<endl;
            return;
        }

        for (const auto &user : userDb["users_info"]) {
            if (user["uni_id"] == uni_id) {
                cout<<" Error : You Have Already Had A User With This [ " << uni_id << " ] "<<endl;
                return;
            }
        }

        // ساخت یک شی جدید کاربر
        //Make A New User OBJ
        json newUser = {
            {"reserved_foods", json::array()},
            {"uni_id", uni_id},
            {"user_balance", user_balance},
            {"user_lastName", user_last_name},
            {"user_name", user_name},
            {"user_password", user_password}
        };

        // افزودن کاربر جدید به لیست کاربران
        //Add New User Into Users.JSON
        userDb["users_info"].push_back(newUser);

        // ذخیره تغییرات در فایل
        saveJsonToFile("users.json", userDb);

        cout << "New User Added Successfully!" << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


void deleteUserByUniId() {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("users.json");

        string targetUniId;
        cout << "Enter The University's Student ID To Delete: ";
        cin >> targetUniId;

        // بررسی و حذف یوزر
        if (db.contains("users_info") && db["users_info"].is_array()) {
            auto &usersArray = db["users_info"];
            auto it = remove_if(usersArray.begin(), usersArray.end(), [&](const json &user) {
                return user["uni_id"] == targetUniId; // پیدا کردن یوزر بر اساس uni_id
            });

            if (it != usersArray.end()) {
                usersArray.erase(it, usersArray.end()); // حذف یوزر
                cout << "User With University Id { " << targetUniId << " } Has Been Deleted Successfully!" << endl;
            } else {
                cout << "User With University Id { " << targetUniId << " } Not Found!" << endl;
            }
        } else {
            cout << "No users_info Array Found In The JSON File." << endl;
        }

        // ذخیره تغییرات در فایل
        saveJsonToFile("users.json", db);

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}


void increaseUserBalance(const string &targetUniId) {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("users.json");

        int increaseAmount;

        // پیدا کردن یوزر با uni_id
        bool userFound = false;
        for (auto &user : db["users_info"]) {
            if (user["uni_id"] == targetUniId) {
                userFound = true;

                //Get Value TO Add Into Current User Balance
                // دریافت مبلغ افزایش موجودی از کاربر
                cout << "Enter The Amount To Increase The Balance: ";
                cin >> increaseAmount;

                // بررسی اعتبار ورودی
                //Check Input That Must Not Be 0 => (Should Not Be Negative)
                if (increaseAmount <= 0) {
                    cout << "Invalid Amount! The Increase Amount Must Be Greater Than Zero." << endl;
                    return;
                }

                // افزایش موجودی کاربر
                //Increase User Balance ["user_balance"]
                int currentBalance = user["user_balance"].get<int>();
                user["user_balance"] = currentBalance + increaseAmount;

                cout << "Balance Increased Successfully By " << increaseAmount << "!" << endl;
                break;
            }
        }

        if (!userFound) {
            cout << "User With uni_id " << targetUniId << " Not Found!" << endl;
        } else {
            // ذخیره‌سازی تغییرات در فایل
            saveJsonToFile("users.json", db);
        }

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void displayUsersInfo() {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("users.json");

        // بررسی وجود داده‌ها
        if (db.contains("users_info") && db["users_info"].is_array()) {
            // تنظیمات جدول
            const int colWidth1 = 15; // طول ستون نام
            const int colWidth2 = 15; // طول ستون نام خانوادگی
            const int colWidth3 = 20; // طول ستون شناسه دانشگاهی
            const int colWidth4 = 10; // طول ستون موجودی
            const int colWidth5 = 30; // طول ستون غذای رزرو شده

            // چاپ هدر جدول
            cout << "+" << string(colWidth1, '-') << "+"
                 << string(colWidth2, '-') << "+"
                 << string(colWidth3, '-') << "+"
                 << string(colWidth4, '-') << "+"
                 << string(colWidth5, '-') << "+" << endl;

            cout << "|" << setw(colWidth1) << left << "User Name"
                 << "|" << setw(colWidth2) << left << "Last Name"
                 << "|" << setw(colWidth3) << left << "University ID"
                 << "|" << setw(colWidth4) << left << "Balance"
                 << "|" << setw(colWidth5) << left << "Reserved Foods" << "|" << endl;

            cout << "+" << string(colWidth1, '-') << "+"
                 << string(colWidth2, '-') << "+"
                 << string(colWidth3, '-') << "+"
                 << string(colWidth4, '-') << "+"
                 << string(colWidth5, '-') << "+" << endl;

            // پیمایش اطلاعات کاربران
            for (const auto &user : db["users_info"]) {
                // استخراج اطلاعات بدون علامت نقل قول
                string name = user["user_name"].get<string>();
                string lastName = user["user_lastName"].get<string>();
                string uniId = user["uni_id"].get<string>();
                int balance = user["user_balance"].get<int>();

                string reservedFoods = "[ ";
                for (const auto &food : user["reserved_foods"]) {
                    reservedFoods += food["food_name"].get<string>() + ", ";
                }
                if (reservedFoods != "[ ") {
                    reservedFoods = reservedFoods.substr(0, reservedFoods.size() - 2);  // حذف کاما و فاصله اضافی
                }
                reservedFoods += " ]";

                // چاپ اطلاعات هر کاربر
                cout << "|" << setw(colWidth1) << left << name
                     << "|" << setw(colWidth2) << left << lastName
                     << "|" << setw(colWidth3) << left << uniId
                     << "|" << setw(colWidth4) << left << balance
                     << "|" << setw(colWidth5) << left << reservedFoods << "|" << endl;
            }

            // خط پایان جدول
            cout << "+" << string(colWidth1, '-') << "+"
                 << string(colWidth2, '-') << "+"
                 << string(colWidth3, '-') << "+"
                 << string(colWidth4, '-') << "+"
                 << string(colWidth5, '-') << "+" << endl;
        } else {
            cout << "No user data found!" << endl;
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}



void programmerInfo() {
    // رسم خطوط جدول
    cout << setfill('=') << setw(50) << "=" << endl;
    cout << setfill(' ') << left << setw(15) << "Field"
         << right << setw(35) << "Information" << endl;
    cout << setfill('=') << setw(50) << "=" << endl;

    // افزودن اطلاعات
    cout << setfill(' ') << left << setw(15) << "Name"
         << right << setw(35) << "Peyman Ghamari" << endl;
    cout << setfill(' ') << left << setw(15) << "Telegram"
         << right << setw(35) << "@P_Ghamari" << endl;
    cout << setfill(' ') << left << setw(15) << "Instagram"
         << right << setw(35) << "@P_Ghamari" << endl;
    cout << setfill(' ') << left << setw(15) << "LinkedIn"
         << right << setw(35) << "linkedin.com/in/peyman-ghamari" << endl;
    cout << setfill(' ') << left << setw(15) << "GitHub"
         << right << setw(35) << "github.com/Peyman-Ghamari" << endl;
    cout << setfill('-') << setw(50) << " - " << endl;
    cout << setfill(' ') << left << setw(15) << "CopyRight By PM"<<endl;

    // پایان جدول
    cout << setfill('=') << setw(50) << "=" << endl;
}


void changeUserPassword(const string &targetUniId) {
    try {
        // بارگذاری فایل JSON
        json db = loadJsonFromFile("users.json");

        string currentPassword, newPassword;

        // پیدا کردن کاربر با uni_id
        bool userFound = false;
        for (auto &user : db["users_info"]) {
            if (user["uni_id"] == targetUniId) {
                userFound = true;

                // دریافت رمز عبور فعلی از کاربر
                //Get Current Password
                cout << "Enter Your Current Password: ";
                cin >> currentPassword;

                // بررسی رمز عبور فعلی
                //Validating Password
                if (user["user_password"] != currentPassword) {
                    cout << "Incorrect Current Password!" << endl;
                    return;
                }

                // دریافت رمز جدید از کاربر
                //Get A New Password
                cout << "Enter Your New Password: ";
                cin >> newPassword;

                // بررسی اعتبار رمز جدید
                //Validate New Password
                if (newPassword.empty()) {
                    cout << "Invalid Password! The New Password Cannot Be Empty." << endl;
                    return;
                }
                if (newPassword == currentPassword) {
                    cout << "New Password Cannot Be The Same As The Current Password!" << endl;
                    return;
                }

                // تغییر رمز عبور
                //Set New Password For User
                user["user_password"] = newPassword;
                cout << "Password Changed Successfully!" << endl;
                break;
            }
        }

        if (!userFound) {
            cout << "User With uni_id " << targetUniId << " Not Found!" << endl;
        } else {
            // ذخیره تغییرات در فایل
            //Save New Edition Into users.JSON
            saveJsonToFile("users.json", db);
        }

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// تابع وارد کردن امتیاز برای غذا
void rateFood(const string &uniId) {
    try {
        // بارگذاری فایل JSON
        string fileName = "food_reserved.json";
        json db = loadJsonFromFile(fileName);

        // نمایش لیست غذاها
        cout << "=== Food List ===" << endl;
        int index = 1;
        for (const auto &food : db["food_info"]) {
            cout << index++ << ". " << food["food_name"] << " (" << food["food_date"]
                 << ") - Price: " << food["food_price"] << " Toman" << endl;
        }

        // انتخاب غذا برای رتبه‌بندی
        cout << "Enter the number of the food you want to rate: ";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > db["food_info"].size()) {
            cout << "Invalid choice!" << endl;
            return;
        }

        // دسترسی به غذای انتخاب‌شده
        auto &selectedFood = db["food_info"][choice - 1];

        // بررسی اینکه آیا کاربر قبلاً غذا را رزرو کرده است
        bool userReserved = false;
        for (const auto &reservedUser : selectedFood["users_reserved"]) {
            if (reservedUser["uni_id"] == uniId) {
                userReserved = true;
                break;
            }
        }

        if (!userReserved) {
            cout << "You have not reserved this food. You cannot rate it." << endl;
            return;
        }

        // دریافت رتبه‌بندی از کاربر
        int rating;
        cout << "Enter your rating for " << selectedFood["food_name"]
             << " (1 to 5): ";
        cin >> rating;

        if (rating < 1 || rating > 5) {
            cout << "Invalid rating! It must be between 1 and 5." << endl;
            return;
        }

        // ثبت امتیاز کاربر
        selectedFood["users_rating"].push_back({
            {"uni_id", uniId},
            {"rating", rating}
        });

        cout << "Your rating has been successfully recorded!" << endl;

        // ذخیره‌سازی تغییرات در فایل
        saveJsonToFile(fileName, db);

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}
// تابع محاسبه میانگین رتبه‌بندی غذاها
void displayAverageFoodRatings() {
    try {
        // بارگذاری فایل JSON
        string fileName = "food_reserved.json";
        json db = loadJsonFromFile(fileName);

        // نمایش هدر جدول
        cout << left << setw(20) << "Food Name"
             << setw(15) << "Food Date"
             << setw(10) << "Avg Rating" << endl;
        cout << string(45, '-') << endl;

        // پیمایش غذاها
        for (const auto &food : db["food_info"]) {
            string foodName = food["food_name"];
            string foodDate = food["food_date"];

            // محاسبه میانگین رتبه‌بندی
            double totalRating = 0;
            int ratingCount = 0;

            for (const auto &rating : food["users_rating"]) {
                totalRating += rating["rating"].get<int>();
                ratingCount++;
            }

            // محاسبه میانگین
            double avgRating = (ratingCount > 0) ? (totalRating / ratingCount) : 0.0;


            // نمایش اطلاعات غذا
            if (ratingCount == 0) {
                cout << setw(20) << foodName
                     << setw(15) << foodDate
                     << setw(10) << fixed << setprecision(2) <<  "There Is NO Rate Yet!" << endl;
            }else
            {
                cout << setw(20) << foodName
                     << setw(15) << foodDate
                     << setw(10) << fixed << setprecision(2) << avgRating << endl;

            }
        }

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}
//using with node
