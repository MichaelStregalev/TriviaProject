#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& name)
{
    m_username = name;
}

LoggedUser::LoggedUser(const LoggedUser& other)
{
    m_username = other.m_username;
}

std::string LoggedUser::getUsername() const
{
    return m_username;
}

bool LoggedUser::operator<(const LoggedUser& other) const
{
    return m_username < other.m_username;
}