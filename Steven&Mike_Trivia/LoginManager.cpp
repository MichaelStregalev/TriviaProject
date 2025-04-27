#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db)
{
    m_database = db;
}

bool LoginManager::signup(std::string userName, std::string passWord, std::string email)
{
    return false;
}

bool LoginManager::login(std::string userName, std::string passWord)
{
    return false;
}

bool LoginManager::logout(std::string userName)
{
    return false;
}
