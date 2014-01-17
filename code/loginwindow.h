#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "commonHeaders.h"
#include "loginwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::LoginWindow *ui;
    UsersList *usersList;

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    
private slots:
    void on_pushButton_clicked();
};

#endif // LOGINWINDOW_H
