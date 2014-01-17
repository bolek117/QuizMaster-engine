#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>
#include "loginwindow.h"
#include "commonHeaders.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::UserWindow *ui;
    Quiz *activeQuiz;
    User *activeUser;
    QSignalMapper *mapper;
    vector < vector < bool > > answers;
    unsigned int totalPoints;
    unsigned int lastEdited;

public:
    explicit UserWindow(User *activeUser, Quiz *activeQuiz, QWidget *parent = 0);
    ~UserWindow();
    void closeEvent(QCloseEvent*);

    void ActiveQuiz(Quiz* quiz);
    Quiz* ActiveQuiz();

    void ActiveUser(User* user);
    User* ActiveUser();

    void FillQuestion(unsigned int index=0);
    void CheckQuiz();

private slots:
    void on_nextButton_clicked();
    void on_prevButton_clicked();
    void checkBoxclicked(int index);
};

#endif // USERWINDOW_H
