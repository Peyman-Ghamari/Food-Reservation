cmake_minimum_required(VERSION 3.29)
project(Food_Reservation)

set(CMAKE_CXX_STANDARD 20)

# ایجاد فایل اجرایی با نام دلخواه
add_executable(Food_Reservation main.cpp my_functions.cpp my_functions.h)

## تغییر نام فایل اجرایی به "Food Reservation"
#set_target_properties(untitled6 PROPERTIES OUTPUT_NAME FoodReservation)
