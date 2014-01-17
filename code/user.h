#ifndef USER_H
#define USER_H

#include "commonHeaders.h"

class User : public Element
{
private:
    bool isAdmin;
    string password;
    QMap < string, unsigned int > *results;

public:
    User(unsigned int id=0, string nickname="", string password="", bool isAdmin=false) : Element(id, nickname), isAdmin(isAdmin), password(password)
    {
        results = new QMap < string, unsigned int > ();

        if (results == NULL)
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }
    }

    //User(string nickname, bool isAdmin, unsigned int id) : isAdmin(isAdmin), nickname(nickname), Element(id) {}
    ~User();

    void IsAdmin(bool isAdmin);
    bool IsAdmin();

    void Password(string password);
    string Password();

    void Nickname(string nickname);
    string Nickname();

    QMap < string, unsigned int> *Results();
    //void Results(Solution *results);
};

#endif // USER_H
