#include "LoginManager.h"
#include "TriviaExceptions.h"
#include <exception>
#include <regex>

LoginManager::LoginManager(IDatabase* db) : m_database(db)
{
}

bool LoginManager::signup(std::string username, std::string password, std::string email)
{
    // Check if the user exists...
    if (m_database->doesUserExist(username))
    {
        return false; // User already exists - cannot signup
    }

    // if the user does not exist, we will need to check that the parameters are valid first!
    // if any of the fields are invalid - an exception will be thrown

    validateUsername(username);
    validatePassword(password, username);
    validateEmail(email);

    // If all parameters are valid, we can add the user to the database!
    // if addNewUser returned 1 - the user signed up successfully.
    return m_database->addNewUser(username, password, email) == 1;
}

bool LoginManager::login(std::string username, std::string password)
{
    // First we will check that the user does exist in the database..
    if (!m_database->doesUserExist(username))
    {
        throw UserDoesNotExistException(username);
    }
    
    // We will need to make sure that the password matches the user's password in the database!
    if (!m_database->doesPasswordMatch(username, password))
    {
        return false; // Wrong password
    }

    LoggedUser user(username);
    auto it = m_loggedUsers.find(user);     // Check that the user isn't already logged on..

    if (it != m_loggedUsers.end())
    {
        return false;   // If found - return false, user already logged on
    }

    // If everything is valid up to now - we can insert the user as logged on!
    m_loggedUsers.insert(user);
    return true;
}

bool LoginManager::logout(std::string username)
{
    LoggedUser user(username);
    auto it = m_loggedUsers.find(user); // Try and find the username we are trying to logout from

    if (it != m_loggedUsers.end())
    {
        m_loggedUsers.erase(it);
        return true;                    // Logout successful
    }

    return false;                       // Did not find the user, logout had an error.
}

void LoginManager::validateUsername(const std::string& username)
{
    if (username.empty())   // USERNAME NOT EMPTY
    {
        throw UsernameInvalidException(username, USERNAME_EMPTY);
    }
    else if (username.length() <= MINIMUM_LENGTH_USERNAME || username.length() >= MAXIMUM_LENGTH_USERNAME)       // USERNAME LENGTH BETWEEN 5 AND 13
    {
        throw UsernameInvalidException(username, LENGTH_INVALID);
    }
    else if (!std::isalpha(username[INDEX_OF_FIRST_CHAR]))                                                       // FIRST CHARACTER IS A LETTER
    {
        throw UsernameInvalidException(username, DOESNT_START_LETTER);
    }

    bool hasUpper = false;      // does the username have at least one uppercase letter?

    // Going through all of the characters of the username
    for (char c : username)
    {
        // If not a letter or a number..
        if (!std::isalnum(c))
        {
            if (std::isspace(c))
            {
                throw UsernameInvalidException(username, USERNAME_SPACES);
            }
            else
            {
                throw UsernameInvalidException(username, SYMBOLS);
            }
        }
        else if (std::isupper(c))   // If the current character is an uppercase letter...
        {
            hasUpper = true;
        }
    }

    if (!hasUpper)  // If the username does not contain an uppercase letter...
    {
        throw UsernameInvalidException(username, NO_UPPER_CASE);
    }
}

void LoginManager::validatePassword(const std::string& password, const std::string& username)
{
    // Checking if the password is empty...
    if (password.empty())
    {
        throw PasswordInvalidException(password, PASSWORD_EMPTY);
    }
    else if (password.length() <= 8 || password.length() >= 20)
    {
        throw PasswordInvalidException(password, SHORT_LENGTH);
    }

    // boolean values that will let us check the password's validity
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    // going through all of the password's characters, checking that all of the requirements are met
    for (char c : password)
    {
        // If there is a space in the password...
        if (std::isspace(c))
        {
            throw PasswordInvalidException(password, PASSWORD_SPACES);
        }
        else if (std::isupper(c))
        {
            hasUpper = true;
        }
        else if (std::islower(c))
        {
            hasLower = true;
        }
        else if (std::isdigit(c))
        {
            hasDigit = true;
        }
        else if (std::ispunct(c))
        {
            hasSpecial = true;
        }
    }

    // Making sure all requirements are met, if not - throw an exception

    if (!hasUpper)
    {
        throw PasswordInvalidException(password, NO_UPPERCASE);
    }
    else if (!hasLower)
    {
        throw PasswordInvalidException(password, NO_LOWERCASE);
    }
    else if (!hasDigit)
    {
        throw PasswordInvalidException(password, NO_DIGIT);
    }
    else if (!hasSpecial)
    {
        throw PasswordInvalidException(password, NO_SPECIAL);
    }
    else if (password.find(username) != std::string::npos)  // If the password contains the username...
    {
        throw PasswordInvalidException(password, CONTAINS_USERNAME);
    }
}

void LoginManager::validateEmail(const std::string& email)
{
    // First check that the email isn't empty...
    if (email.empty())
    {
        throw EmailInvalidException(email, EMAIL_EMPTY);
    }
    else if (email.find(' ') != std::string::npos)      // Checking that there are no spaces in the email..
    {
        throw EmailInvalidException(email, EMAIL_SPACES);
    }
    else if (email.find('@') == std::string::npos)      // Checking that there is an at sign in the email..
    {
        throw EmailInvalidException(email, NO_AT_SIGN);
    }
    else if (email.find('.') == std::string::npos || email.find('.') < email.find('@'))     // Checking that there is a dot in the email, and it is after the at sign
    {
        throw EmailInvalidException(email, NO_DOT);
    }

    // Using REGEX, we will check if the email's format is valid!

    std::regex emailRegex(R"(([^@\s]+)@([^@\s]+\.[^@\s]+))");
    if (!std::regex_match(email, emailRegex))
    {
        throw EmailInvalidException(email, INVALID_FORMAT);
    }
}
