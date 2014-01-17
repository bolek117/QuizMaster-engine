#ifndef USERSLIST_H
#define USERSLIST_H

#include "commonHeaders.h"

class UsersList
{
private:
    vector < class User * > *usersList;
    User* activeUser;

public:
    UsersList();
    ~UsersList();

    void Add(User* user);
    vector < User* > *Get();
    User* GetUser(unsigned int i);
    User* SearchForUser(unsigned int id);
    void ActiveUser(User* user);
    User* ActiveUser();
    vector < User* > *AddUsersFromFile(string path="users.xml");
    unsigned int SaveUsersToFile(bool msg=true);

};

#endif // USERSLIST_H
