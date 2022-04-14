#include <sqlite3.h>
#include <iostream>
#include "sqlite_orm.h"

struct User{
    int id;
    std::string firstName;
    std::string lastName;
    int birthDate;
    std::unique_ptr<std::string> imageUrl;
    int typeId;
};

struct UserType {
    int id;
    std::string name;
};
int main(){
    using namespace sqlite_orm;
    auto storage = make_storage("db.sqlite",
                                make_table("users",
                                           make_column("id", &User::id, autoincrement(), primary_key()),
                                           make_column("first_name", &User::firstName),
                                           make_column("last_name", &User::lastName),
                                           make_column("birth_date", &User::birthDate),
                                           make_column("image_url", &User::imageUrl),
                                           make_column("type_id", &User::typeId)),
                                make_table("user_types",
                                           make_column("id", &UserType::id, autoincrement(), primary_key()),
                                           make_column("name", &UserType::name, default_value("name_placeholder"))));

    User user{-1, "Jonh", "Doe", 664416000, std::make_unique<std::string>("url_to_heaven"), 3 };
    storage.sync_schema();

    auto insertedId = storage.insert(user);
    std::cout << "insertedId = " << insertedId << std::endl;      //  insertedId = 8

    auto allUsers = storage.get_all<User>();
    std::cout << "allUsers (" << allUsers.size() << "):" << std::endl;
    for(auto &user : allUsers) {
        std::cout << storage.dump(user) << std::endl; //  dump returns std::string with json-like style object info. For example: { id : '1', first_name : 'Jonh', last_name : 'Doe', birth_date : '664416000', image_url : 'https://cdn1.iconfinder.com/data/icons/man-icon-set/100/man_icon-21-512.png', type_id : '3' }
    }
    storage.sync_schema(true);
}