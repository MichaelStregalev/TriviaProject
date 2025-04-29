#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db)
{
    m_database = db;
}

bool LoginManager::signup(std::string userName, std::string passWord, std::string email)
{
    if (!(m_database->doesUserExist(userName))) //If user already exists, dont bother checking if all is valid
    {
        if (!(m_database->addNewUser(userName, passWord, email))) //If user exists, check if all is valid
        {
            return true;
        }
    }
    return false;
}

bool LoginManager::login(std::string userName, std::string passWord)
{
    if (m_database->doesUserExist(userName)) //If user exists, check the password
    {
        if (m_database->doesPasswordMatch(userName, passWord)) //If passWord match, Login was successfull
        {
            return true;
        }
    }
    return false;
}

bool LoginManager::logout(std::string userName)
{
    return false;
}
