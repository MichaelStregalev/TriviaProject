#include "TriviaExceptions.h"

UsernameInvalidException::UsernameInvalidException(const std::string& username, usernameValidity validity)
    : SignupException(""), _username(username)
{
    // The start of the message, unless the username is empty and then we'll override the start.
    _message = "The username '" + username + "' ";

    switch (validity)
    {
    case USERNAME_EMPTY:
        _message = "The username can't be empty!";
        break;
    case LENGTH_INVALID:
        _message += "must be between 5 and 13 characters.";
        break;
    case DOESNT_START_LETTER:
        _message += "must start with a letter.";
        break;
    case NO_UPPER_CASE:
        _message += "must contain at least one uppercase letter.";
        break;
    case SYMBOLS:
        _message += "must not contain symbols.";
        break;
    case USERNAME_SPACES:
        _message += "must not contain spaces.";
        break;
    case USERNAME_VALID:            // Shouldn't happen, but just in case - we will handle it...
        _message += "is valid.";
        break;
    default:
        _message += "is invalid for an unknown reason.";
        break;
    }
}

PasswordInvalidException::PasswordInvalidException(const std::string& password, passwordValidity validity)
    : SignupException(""), _password(password)
{
    // The start of the message, unless the username is empty and then we'll override the start.
    _message = "The password '" + password + "' ";

    switch (validity)
    {
    case PASSWORD_EMPTY:
        _message = "The password cannot be empty.";
        break;
    case SHORT_LENGTH:
        _message += "must be between 8 and 20 characters.";
        break;
    case NO_UPPERCASE:
        _message += "must contain at least one uppercase letter.";
        break;
    case NO_LOWERCASE:
        _message += "must contain at least one lowercase letter.";
        break;
    case NO_DIGIT:
        _message += "must contain at least one digit.";
        break;
    case NO_SPECIAL:
        _message += "must contain at least one special character.";
        break;
    case PASSWORD_SPACES:
        _message += "must not contain spaces.";
        break;
    case CONTAINS_USERNAME:
        _message += "must not contain the username.";
        break;
    case PASSWORD_VALID:        // Shouldn't happen, but just in case - we will handle it...
        _message += "is valid.";
        break;
    default:
        _message += "is invalid for an unknown reason.";
        break;
    }
}

EmailInvalidException::EmailInvalidException(const std::string& email, emailValidity validity)
    : SignupException(""), _email(email)
{
    // The start of the message, unless the username is empty and then we'll override the start.
    _message = "The email '" + email + "' ";

    switch (validity)
    {
    case EMAIL_EMPTY:
        _message = "The email cannot be empty.";
        break;
    case NO_AT_SIGN:
        _message = "must contain an '@' sign.";
        break;
    case NO_DOT:
        _message = "must contain a '.' after the '@' sign.";
        break;
    case EMAIL_SPACES:
        _message = "must not contain spaces.";
        break;
    case INVALID_FORMAT:
        _message = "has an invalid format.";
        break;
    case EMAIL_VALID:       // Shouldn't happen, but just in case - we will handle it...
        _message = "is valid.";
        break;
    default:
        _message = "is invalid for an unknown reason.";
        break;
    }
}