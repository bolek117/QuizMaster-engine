#ifndef SELECTQUIZWINDOW_H
#define SELECTQUIZWINDOW_H
#include "commonHeaders.h"
#include <QWidget>

namespace Ui {
class SelectQuizWindow;
}

class SelectQuizWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit SelectQuizWindow(QWidget *parent = 0);
    ~SelectQuizWindow();
    void closeEvent(QCloseEvent*);
    void ActiveUser(User* user);
    User* ActiveUser();
    
private slots:
    void on_BeginQuizButton_clicked();

private:
    Ui::SelectQuizWindow *ui;
    QuizList *quizList;
    User* activeUser;
};

#endif // SELECTQUIZWINDOW_H
