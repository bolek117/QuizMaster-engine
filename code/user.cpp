#include "user.h"

User::~User()
{
    if (results != NULL)
        delete results;
}

void User::IsAdmin(bool isAdmin)
{
    this->isAdmin = isAdmin;
}

bool User::IsAdmin()
{
    return isAdmin;
}

void User::Nickname(string nickname)
{
    this->Text(nickname);
}

string User::Nickname()
{
    return this->Text();
}

void User::Password(string password)
{
    this->password = password;
}

string User::Password()
{
    return password;
}

QMap<string, unsigned int> *User::Results()
{
    return results;
}
