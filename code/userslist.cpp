#include "userslist.h"

UsersList::UsersList()
{
    usersList = new vector< User * >;

    if (usersList == NULL)
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }
}

UsersList::~UsersList()
{
    if (usersList != NULL)
    {
        for(unsigned int i=0;i<usersList->size();++i)
            delete usersList->operator [](i);

        delete usersList;
    }
}

void UsersList::Add(User* user)
{
    usersList->push_back(user);
}

vector < User* > *UsersList::Get()
{
    return usersList;
}

User* UsersList::GetUser(unsigned int i)
{
    return usersList->operator [](i);
}

User* UsersList::SearchForUser(unsigned int id)
{
    for(unsigned int i=0;i<usersList->size();i++)
        if (GetUser(i)->Id() == id)
            return GetUser(i);

    return NULL;
}

void UsersList::ActiveUser(User* user)
{
    this->activeUser = user;
}

User* UsersList::ActiveUser()
{
    return activeUser;
}

vector < User * > *UsersList::AddUsersFromFile(string path)
{
    QDomDocument document;
    QFile file(path.c_str());
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        msg.setText("Unable to open credentials file!");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
        return NULL;
    }

    if (!document.setContent(&file))
    {
        msg.setText("Unable to parse file!");
        msg.exec();
        file.close();
        return NULL;
    }

    QDomElement root = document.firstChildElement();
    if (root.isNull())
    {
        msg.setText("File is empty!");
        msg.exec();
        return NULL;
    }

    QDomNodeList usersList = root.elementsByTagName("user");
    if (usersList.isEmpty())
    {
        msg.setText("File is empty!");
        msg.exec();
        file.close();
        return NULL;
    }

    User* user;
    for(unsigned int i=0;i<(unsigned int)usersList.count();i++)
    {
        QDomNode userNode = usersList.at(i);
        if (userNode.isElement())
        {
            QDomElement userElement = userNode.toElement();
            bool isAdmin = (bool)userElement.attribute("isAdmin").toInt();
            user = new User(userElement.attribute("id").toUInt(), userElement.attribute("nickname").toStdString(), userElement.attribute("password").toStdString(), isAdmin);
            if (user == NULL)
            {
                msg.setText("Memory allocation error!");
                msg.exec();
                file.close();
                return NULL;
            }
            else
            {
                QDomNodeList quizesList = userElement.elementsByTagName("quiz");
                for (unsigned int j=0;j<(unsigned int)quizesList.count();j++)
                {
                    QDomNode quizNode = quizesList.at(j);
                    if (quizNode.isElement())
                    {
                        QDomElement quizElement = quizNode.toElement();
                        QMap < string, unsigned int> *results = user->Results();
                        results->insert(quizElement.attribute("text").toStdString(), quizElement.attribute("points").toUInt());
                    }
                }
                this->usersList->push_back(user);
            }
        }
    }
    return this->usersList;
}

unsigned int UsersList::SaveUsersToFile(bool msg)
{
    QDomDocument document;

    QDomElement root;
    root = document.createElement("users");
    document.appendChild(root);

    for(unsigned int i=0;i<(unsigned int)Get()->size();++i)
    {
        QDomElement user = document.createElement("user");
        User* userObj = GetUser(i);
        user.setAttribute("password", userObj->Password().c_str());
        if (userObj->IsAdmin() == true)
            user.setAttribute("isAdmin", 1);
        else
            user.setAttribute("isAdmin", 0);
        user.setAttribute("nickname", userObj->Nickname().c_str());

        root.appendChild(user);

        QList <string> keys = userObj->Results()->keys();

        for (unsigned int j=0;j<(unsigned int)keys.count();++j)
        {
            QDomElement quiz = document.createElement("quiz");
            unsigned int points = userObj->Results()->value(keys[j]);
            quiz.setAttribute("text", keys[j].c_str());
            quiz.setAttribute("points", points);

            user.appendChild(quiz);
        }
    }

    QFile file("users.xml");
    QString xmlToSave = document.toString();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QMessageBox msg;
        msg.setText("Unable to open file for writing!");
        msg.exec();
        return 0;
    }

    file.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    file.write(xmlToSave.toStdString().c_str());
    file.close();

    if (msg)
    {
        QMessageBox msg("Completed", "Users list update completed.", QMessageBox::Information, 0,0,0);
        msg.exec();
    }

    return 1;
}
