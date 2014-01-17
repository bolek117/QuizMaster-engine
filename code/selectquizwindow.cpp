#include "selectquizwindow.h"
#include "ui_selectquizwindow.h"
#include "userwindow.h"
#include "loginwindow.h"
#include "quizlist.h"
#include "commonHeaders.h"

SelectQuizWindow::SelectQuizWindow(QWidget *parent) : QWidget(parent), ui(new Ui::SelectQuizWindow)
{
    ui->setupUi(this);
    quizList = new QuizList();

    if (quizList == NULL)
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
    }
    else
    {
        quizList->AddQuizesFromFolder();
        if (quizList->Get()->size() == 0)
        {
            ui->SelectQuizBox->clear();
            ui->SelectQuizBox->setEnabled(false);
            ui->BeginQuizButton->setEnabled(false);
            QListWidgetItem *item = new QListWidgetItem("[There is no quizes to solve]");
            ui->SelectQuizBox->addItem(item);
        }
        else
        {
            ui->SelectQuizBox->clear();
            ui->SelectQuizBox->setEnabled(true);
            ui->BeginQuizButton->setEnabled(true);

            QListWidgetItem *item;
            for (unsigned int i=0;i<(unsigned int)quizList->Get()->size();++i)
            {
                item = new QListWidgetItem(quizList->GetQuiz(i)->Text().c_str());
                if (item == NULL)
                {
                    QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
                    msg.exec();
                }
                else
                {
                    item->setData(roles::ID, qVariantFromValue(quizList->GetQuiz(i)->Id()));
                    ui->SelectQuizBox->addItem(item);
                }
            }
        }
    }
}

SelectQuizWindow::~SelectQuizWindow()
{
    delete ui;
    delete quizList;
}

void SelectQuizWindow::closeEvent(QCloseEvent*)
{
    LoginWindow *lw = new LoginWindow();
    if (lw != NULL)
        lw->show();
    else
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }
}

void SelectQuizWindow::ActiveUser(User* user)
{
    this->activeUser = user;
}

User* SelectQuizWindow::ActiveUser()
{
    return activeUser;
}

void SelectQuizWindow::on_BeginQuizButton_clicked()
{
    UserWindow *userWindow=NULL;
    unsigned int id = ui->SelectQuizBox->currentItem()->data(roles::ID).toUInt();
    Quiz* quiz = quizList->SearchForQuiz(id);
    if (quiz != NULL)
        userWindow = new UserWindow(ActiveUser(), quiz);
    else
    {
        QMessageBox msg("Error", "Such quiz does not exist!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }

    if (userWindow == NULL)
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
    }
    else
    {
        if (quiz != NULL)
        {
            this->setVisible(false);
            userWindow->show();
        }
        else
        {
            QMessageBox msg("Error", "Such quiz does not exist!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }
    }
}
