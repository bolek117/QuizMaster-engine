#include "loginwindow.h"
#include "mainwindow.h"
#include "selectquizwindow.h"
#include "commonHeaders.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->notValidDataLabel->setVisible(false);

    usersList = new UsersList();
    if (usersList == NULL)
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        exit(1);
        return;
    }
    else
        usersList->AddUsersFromFile();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{
    if (ui->loginBox->text().trimmed() != "" && ui->passwordBox->text().trimmed() != "")
    {
        for(unsigned int i=0;i<usersList->Get()->size();++i)
        {
            string login = usersList->GetUser(i)->Text();
            if (ui->loginBox->text().trimmed() == login.c_str())
            {
                string pass = usersList->GetUser(i)->Password();
                if (ui->passwordBox->text().trimmed() == pass.c_str())
                {
                    if (usersList->GetUser(i)->IsAdmin() == true)
                    {
                        MainWindow *w = new MainWindow();
                        if (w != NULL)
                        {
                            w->ParentWindow(this);
                            w->show();
                            this->setVisible(false);
                            return;
                        }
                        else
                        {
                            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
                            msg.exec();
                            return;
                        }
                    }
                    else
                    {
                        SelectQuizWindow *sqw = new SelectQuizWindow();
                        if (sqw != NULL)
                        {
                            sqw->ActiveUser(usersList->GetUser(i));
                            sqw->show();
                            this->setVisible(false);
                            return;
                        }
                        else
                        {
                            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
                            msg.exec();
                            return;
                        }
                    }
                }
                else
                    ui->notValidDataLabel->setVisible(true);
            }
            else
                ui->notValidDataLabel->setVisible(true);
        }
    }
    else
        ui->notValidDataLabel->setVisible(true);
}
