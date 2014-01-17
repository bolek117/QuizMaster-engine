#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "commonHeaders.h"
#include "loginwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    

private:
    Ui::MainWindow *ui;
    //vector < Quiz * > quizList;
    //vector < User * > usersList;
    QuizList* quizList;
    UsersList* usersList;
    stringstream buffer;
    QLabel statusBarLabel;
    LoginWindow* parentWindow;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vector < Quiz * > GetQuizsList();

    /*** quiz stuff ***/
    void FillQuestionForm(Question *question=NULL, bool newQuestion=true);
    void FillQuizForm(Quiz *quiz=NULL);
    void FillQuizList();

    void closeEvent(QCloseEvent*);

    /*** UI stuff ***/    
    void UpdateSaveButton();
    void UpdateStatusBar();
    void ParentWindow(LoginWindow *parentWindow);
    LoginWindow* ParentWindow();

    /*** question stuff ***/
    Answer* AddAnswer(Answer *answer=NULL, Question *question=NULL);
    Answer* SearchForAnswer(unsigned int answerId, Question* question=NULL);

    /*** users stuff ***/
    unsigned int AddUsersFromFile(string path);
    unsigned int SaveUsersToFile(bool msg=true);
    QList <QTreeWidgetItem*> FillUsersList();
    User* FillUserForm(User *user=NULL);
    User* SearchForUser(unsigned int id);
    void ActiveUser(User* user);
    User* ActiveUser();
    
private slots:
    void on_AnswersTable_cellChanged(int row, int column);
    void on_AddAnswerButton_clicked();
    void on_AddNewQuestionButton_clicked();
    void on_SelectQuestionBox_activated(int index);
    void on_QuestionEdit_editingFinished();
    void on_AddNewQuiz_clicked();
    void on_EditSelectedQuiz_clicked();
    void on_QuizNameBox_editingFinished();
    void on_Mode_currentChanged(int index);
    void on_DeleteSelectedQuiz_clicked();
    void on_QuizesList_itemClicked(QListWidgetItem *item);
    void on_UsersTree_itemClicked(QTreeWidgetItem *item, int column);
    void on_UserRoleButton_clicked();
    void on_UserPasswordBox_editingFinished();
    void on_AddUserButton_clicked();
    void on_UserNameBox_editingFinished();
    void on_DeleteUserButton_clicked();
};

#endif // MAINWINDOW_H
