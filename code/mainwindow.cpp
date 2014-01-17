#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "commonHeaders.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    quizList = new QuizList();
    usersList = new UsersList();

    if (quizList == NULL || usersList == NULL)
    {
        QMessageBox msg("Error", "Critical error", QMessageBox::Warning, 0,0,0);
        msg.exec();
        exit(1);
    }

    FillQuizList();
    FillQuizForm(quizList->Get()->front());

    QWidget *statusBarWidget = new QWidget();
    statusBarLabel.setParent(statusBarWidget);

    UpdateStatusBar();
    statusBar()->addPermanentWidget(statusBarWidget, 1);

    usersList->AddUsersFromFile("users.xml");
    QStringList labels;
    labels.append("Quiz text");
    labels.append("Obtained points");
    ui->SolvedQuizesList->setHorizontalHeaderLabels(labels);
    FillUsersList();
    ui->UsersTree->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
    for(unsigned int i=0;i<quizList->Get()->size();i++)
        delete quizList->GetQuiz(i); // all additional deletes are performed by destructors of classes located on level below

    for(unsigned int i=0;i<usersList->Get()->size();i++)
        delete usersList->GetUser(i);

    LoginWindow *lw = ParentWindow();
    if (lw != NULL)
        lw->setVisible(true);
}

void MainWindow::closeEvent(QCloseEvent*)
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

/*** *** User edit *** ***/
QList <QTreeWidgetItem*> MainWindow::FillUsersList()
{
    ui->UsersTree->setColumnCount(1);
    ui->UsersTree->clear();

    QList <QTreeWidgetItem*> items;
    items.append(new QTreeWidgetItem(QStringList("Admins"), roles::TOPELEMENT));
    items.append(new QTreeWidgetItem(QStringList("Users"), roles::TOPELEMENT));

    ui->UsersTree->insertTopLevelItems(0, items);

    QList <QTreeWidgetItem*> result;
    vector < User * >::iterator it;
    unsigned int i=0;
    for(it=usersList->Get()->begin();it!=usersList->Get()->end();it++)
    {
        i++;
        User *user = *it;
        user->Id(i);
        QTreeWidgetItem *item = new QTreeWidgetItem();

        if (item == NULL)
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
            return result;
        }
        else
        {
            item->setText(0, user->Nickname().c_str());
            item->setData(0, roles::ID, qVariantFromValue(user->Id()));
            unsigned int role=1;
            if (user->IsAdmin() == true)
                role = 0;

            items[role]->addChild(item);
            result.append(item);
            if(it == usersList->Get()->begin())
            {
                ui->UsersTree->setCurrentItem(item);
                on_UsersTree_itemClicked(item, 0);
            }
        }
    }
    ui->UsersTree->expandAll();
    return result;
}

User* MainWindow::FillUserForm(User *user)
{
    ui->UserRoleBox->clear();
    ui->UserRoleBox->insertItem(0, "User", qVariantFromValue(1));
    ui->UserRoleBox->insertItem(1, "Administrator", qVariantFromValue(2));

    if (user == NULL)
    {
        buffer.str("");
        buffer << "New user " << usersList->Get()->size()+1;
        user = new User(usersList->Get()->size()+1, buffer.str(), "", false);

        if (user == NULL)
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
            usersList->ActiveUser(NULL);
            return NULL;
        }
        else
            usersList->Add(user);
    }

    ui->UserNameBox->setText(user->Nickname().c_str());
    ui->UserPasswordBox->setText(user->Password().c_str());
    if (user->IsAdmin())
        ui->UserRoleBox->setCurrentIndex(1);
    else
        ui->UserRoleBox->setCurrentIndex(0);

    ui->SolvedQuizesList->clear();
    QList <string> list = user->Results()->keys();
    if (list.count() == 0)
        ui->SolvedQuizesList->setRowCount(0);

    for (unsigned int i=0;i<(unsigned int)list.count();++i)
    {
        ui->SolvedQuizesList->setRowCount(i+1);
        QTableWidgetItem *item = new QTableWidgetItem(list[i].c_str());

        if (item == NULL)
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
            return NULL;
        }
        else
            ui->SolvedQuizesList->setItem(i, 0, item);

        buffer.str("");
        buffer << user->Results()->value(list[i]);
        item = new QTableWidgetItem(buffer.str().c_str());

        if (item == NULL)
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
            return NULL;
        }
        else
            ui->SolvedQuizesList->setItem(i, 1, item);
    }

    usersList->SaveUsersToFile(false);
    usersList->ActiveUser(user);
    return user;
}

/*** *** Quiz edit *** ***/
/*** Quiz Stuff ***/
void MainWindow::FillQuizForm(Quiz *quiz)
{
    if (quiz == NULL)
    {
        quiz = quizList->AddNewEmptyQuiz();
    }

    if (quiz != NULL)
    {
        ui->QuizNameBox->setText(quiz->Text().c_str());
        quizList->ActiveQuiz(quiz);
        FillQuestionForm(quiz->GetQuestions()->front());
    }
}

void MainWindow::FillQuizList()
{
    for (unsigned int i=0;i<quizList->Get()->size();i++)
        if (quizList->GetQuiz(i) != NULL)
            delete quizList->GetQuiz(i);

    quizList->Get()->clear();
    ui->QuizesList->clear();

    quizList->AddQuizesFromFolder();
    QListWidgetItem *item;

    for (unsigned int i=0;i<quizList->Get()->size();i++)
    {
        item = new QListWidgetItem(quizList->GetQuiz(i)->Text().c_str());
        if (item != NULL)
        {
            item->setData(roles::ID, qVariantFromValue(quizList->GetQuiz(i)->Id()));
            ui->QuizesList->addItem(item);
        }
        else
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }
    }
}

/*** Question Stuff ***/
void MainWindow::FillQuestionForm(Question *question, bool newQuestion)
{
    if (question == NULL)
        question = quizList->ActiveQuiz()->AddNewQuestion();

    if (question != NULL)
    {
        if (newQuestion == true)
        {
            ui->SelectQuestionBox->clear();
            for (unsigned int i=0;i<quizList->ActiveQuiz()->GetQuestions()->size();i++)
            {
                Question* question_tmp = quizList->ActiveQuiz()->GetQuestion(i);
                ui->SelectQuestionBox->addItem(question_tmp->Text().c_str());
                ui->SelectQuestionBox->setItemData(ui->SelectQuestionBox->findText(question_tmp->Text().c_str()), qVariantFromValue(question_tmp->Id()), roles::ID);
            }
        }

        Quiz* parent;
        parent = (Quiz*)question->Parent();

        ui->SelectQuestionBox->setCurrentIndex(ui->SelectQuestionBox->findData(qVariantFromValue(question->Id()), roles::ID));

        ui->AnswersTable->clearContents();
        ui->AnswersTable->setRowCount(0);
        ui->QuestionEdit->setText(question->Text().c_str());

        parent = (Quiz*)question->Parent();
        parent->ActiveQuestion(question);
        UpdateSaveButton();

    }
    else
        return;

    if (question->GetAnswers()->size() == 0)
    {
        AddAnswer();
    }
    else
    {
        for(unsigned int i=0;i<question->GetAnswers()->size();i++)
        {
            AddAnswer(question->GetAnswer(i));
        }
    }
}

Answer* MainWindow::AddAnswer(Answer* answer, Question *question)
{
    if (question == NULL)
    {
        if (answer == NULL)
            question = quizList->ActiveQuiz()->ActiveQuestion();
        else
            question = (Question*)answer->Parent();
    }

    if (answer == NULL)
        answer = question->AddNewAnswer();

    if (answer != NULL)
    {
        unsigned int row = answer->Order()-1;
        ui->AnswersTable->insertRow(row);

        QVariant id(answer->Id());
        QTableWidgetItem *item = new QTableWidgetItem();

        // isProper column0
        item->setData(roles::ID, id);
        item->setData(Qt::DisplayRole, answer->IsProper());
        ui->AnswersTable->setItem(row, answersColumns::ISPROPER, item);

        ui->AnswersTable->editItem(item);
        // text column
        item = new QTableWidgetItem(answer->Text().c_str());
        item->setData(roles::ID, id);
        ui->AnswersTable->setItem(row, answersColumns::TEXT, item);

        ui->AnswersTable->selectRow(row);

        UpdateSaveButton();
        UpdateStatusBar();
        return answer;
    }
    else
        return NULL;
}

/*** UI Stuff ***/
void MainWindow::UpdateSaveButton()
{
    if (quizList->ActiveQuiz()->ActiveQuestion()->GetAnswers()->size() > 1)
        ui->AddNewQuestionButton->setEnabled(true);
    else
        ui->AddNewQuestionButton->setEnabled(false);
}

void MainWindow::UpdateStatusBar()
{
    stringstream text;
    unsigned int points=0;
    unsigned int actualQuestionIndex=1;
    for(unsigned int i=0;i<quizList->ActiveQuiz()->GetQuestions()->size();i++)
    {
        Quiz *quiz = quizList->ActiveQuiz();
        if (quiz->GetQuestion(i)->Id() == quiz->ActiveQuestion()->Id())
            actualQuestionIndex = i+1;

        for(unsigned int j=0;j<quiz->GetQuestion(i)->GetAnswers()->size();j++)
            if (quiz->GetQuestion(i)->GetAnswer(j)->IsProper() == true)
                points++;
    }

    text << "Question " << actualQuestionIndex << "/" << quizList->ActiveQuiz()->GetQuestions()->size() << ", Max. points: " << points;
    statusBarLabel.setText(text.str().c_str());
    if (quizList->ActiveQuiz()->SaveToFile() == 0)
    {
        QMessageBox msg;
        msg.setText("Save to file failed.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
}

void MainWindow::ParentWindow(LoginWindow *parentWindow)
{
    this->parentWindow = parentWindow;
}

LoginWindow* MainWindow::ParentWindow()
{
    return parentWindow;
}

/*** Quiz Slots Stuff ***/
void MainWindow::on_AddAnswerButton_clicked()
{
    AddAnswer();
}

void MainWindow::on_AnswersTable_cellChanged(int row, int column)
{
    unsigned int numberOfAnswers = quizList->ActiveQuiz()->ActiveQuestion()->GetAnswers()->size();
    if (numberOfAnswers > 0)
    {
        QTableWidgetItem *item = ui->AnswersTable->item(row, column);
        QString itemText = item->text().trimmed();
        unsigned int id = item->data(roles::ID).toUInt();
        Answer *answer = quizList->ActiveQuiz()->ActiveQuestion()->SearchForAnswer(id);

        if (answer != NULL)
        {
            switch(column)
            {
            case answersColumns::TEXT:
            {
                if (itemText == "" && quizList->ActiveQuiz()->ActiveQuestion()->GetAnswers()->size() > 1)
                {
                    quizList->ActiveQuiz()->ActiveQuestion()->DeleteAnswer(id);
                    ui->AnswersTable->removeRow(row);
                    UpdateSaveButton();
                }
                else
                    answer->Text(itemText.toStdString());

            }
                break;

            case answersColumns::ISPROPER:
            {
                if (itemText.toLower() == "true")
                    answer->IsProper(true);
                else
                    answer->IsProper(false);
            }
                break;
            }
            UpdateStatusBar();
        }
    }
}

void MainWindow::on_AddNewQuestionButton_clicked()
{
    FillQuestionForm(NULL);
}

void MainWindow::on_SelectQuestionBox_activated(int index)
{
    Question* question = quizList->ActiveQuiz()->SearchForQuestion(ui->SelectQuestionBox->itemData(index, roles::ID).toUInt());
    if (question != quizList->ActiveQuiz()->ActiveQuestion())
        FillQuestionForm(question, false);
}

void MainWindow::on_QuestionEdit_editingFinished()
{
    QString text = ui->QuestionEdit->text().trimmed();
    if(text.length() != 0)
    {
        ui->SelectQuestionBox->setItemText(ui->SelectQuestionBox->currentIndex(), text);
        quizList->ActiveQuiz()->ActiveQuestion()->Text(text.toStdString());
        quizList->ActiveQuiz()->SaveToFile();
    }
    else
        ui->QuestionEdit->setText(quizList->ActiveQuiz()->ActiveQuestion()->Text().c_str());
}

void MainWindow::on_AddNewQuiz_clicked()
{
    FillQuizForm(quizList->AddNewEmptyQuiz());
    FillQuizList();
}

void MainWindow::on_EditSelectedQuiz_clicked()
{
    on_QuizesList_itemClicked(ui->QuizesList->currentItem());
}

void MainWindow::on_QuizNameBox_editingFinished()
{
    QString text = ui->QuizNameBox->text().trimmed();
    if (text.length() != 0 && text.toStdString() != quizList->ActiveQuiz()->Text().c_str())
    {
        for (unsigned int i=0;i<quizList->Get()->size();i++)
        {
            if (quizList->GetQuiz(i)->Text() == text.toStdString())
            {
                QMessageBox msg("Error", "Quiz with this name already exists!", QMessageBox::Warning, 0,0,0);
                msg.exec();
                ui->QuizNameBox->setText(quizList->ActiveQuiz()->Text().c_str());
                return;
            }
        }

        QDir dir("quiz");

        QString oldName = quizList->ActiveQuiz()->Text().c_str();
        oldName.append(".xml");
        QString path = dir.absoluteFilePath(oldName);

        string newPath = path.toStdString().substr(0, path.toStdString().rfind("/")+1);
        newPath.append(text.toStdString());
        newPath.append(".xml");

        if (dir.rename(path, newPath.c_str()) != false)
        {
            if (quizList->ActiveQuiz()->SaveToFile() == 0)
            {
                QMessageBox msg("Error", "Unable to save file", QMessageBox::Warning, 0,0,0);
                msg.exec();
                ui->QuizNameBox->setText(quizList->ActiveQuiz()->Text().c_str());
            }
            else
            {
                quizList->ActiveQuiz()->Text(text.toStdString());
                ui->QuizesList->currentItem()->setText(text);
            }
        }
        else
        {
            if (path.toStdString() != newPath)
            {
                QMessageBox msg("Error", "Unable to change quiz name!", QMessageBox::Warning, 0,0,0);
                msg.exec();
                ui->QuizNameBox->setText(quizList->ActiveQuiz()->Text().c_str());
            }
        }

    }
    else
        ui->QuizNameBox->setText(quizList->ActiveQuiz()->Text().c_str());
}

void MainWindow::on_Mode_currentChanged(int index)
{
    if (index == 0)
    {
        UpdateStatusBar();
    }
    else
    {
        statusBarLabel.setText("");
    }
}

void MainWindow::on_DeleteSelectedQuiz_clicked()
{
    QMessageBox prompt("Confirm", "Do you really want to delete this quiz? XML file will be deleted too. This operation cannot be reverted.", QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No, 0);
    prompt.exec();

    if (prompt.result() == QMessageBox::Yes)
    {
        unsigned int quizId = ui->QuizesList->currentItem()->data(roles::ID).toUInt();
        Quiz *quiz = quizList->SearchForQuiz(quizId);
        if (quiz != NULL)
        {
            for (unsigned int i=0;i<quizList->Get()->size();i++)
            {
                if (quiz == quizList->GetQuiz(i))
                {
                    QString text;
                    text.append("quiz/");
                    text.append(quiz->Text().c_str());
                    text.append(".xml");
                    QFile file(text);
                    file.remove();
                    QListWidgetItem *item = ui->QuizesList->currentItem();
                    delete item;
                    delete quizList->GetQuiz(i);
                    quizList->Get()->erase(quizList->Get()->begin()+i);
                    return;
                }
            }
        }
        else
        {
            QMessageBox msg("Error", "Quiz does not exist!", QMessageBox::Warning, 0,0,0);
            msg.exec();
            return;
        }
    }
}

void MainWindow::on_QuizesList_itemClicked(QListWidgetItem *item)
{
    unsigned int id=item->data(roles::ID).toUInt();
    Quiz* quiz = quizList->SearchForQuiz(id);
    if (quiz != NULL)
        FillQuizForm(quiz);
    else
    {
        QMessageBox msg;
        msg.setText("Quiz not found.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
}

/*** Users Slots Stuff ***/
void MainWindow::on_UsersTree_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() != roles::TOPELEMENT)
    {
        User *user = usersList->SearchForUser(item->data(column, roles::ID).toUInt());
        if (user == NULL)
        {
            QMessageBox msg("Error", "Such user does not exist!", QMessageBox::Warning, 0,0,0);
            msg.exec();
            usersList->ActiveUser(NULL);
            return;
        }
        else
        {
            FillUserForm(user);
            usersList->ActiveUser(user);
        }
    }
}

void MainWindow::on_UserRoleButton_clicked()
{
    User *user = usersList->ActiveUser();
    if (user == NULL)
    {
        QMessageBox msg("Error", "Such user does not exists!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }
    else
    {
        if (ui->UserRoleBox->currentText().toLower() == "administrator")
            user->IsAdmin(true);
        else
            user->IsAdmin(false);

        if (ui->UserPasswordBox->text().trimmed() != "")
            user->Password(ui->UserPasswordBox->text().trimmed().toStdString());
        else
        {
            user->Password("pass");
            QMessageBox msg("Empty field detected", "Password field cannot be empty. Password set to \"pass\", change it immediately!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }

        QDomDocument document;
        QFile file("users.xml");
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            msg.setText("Unable to open file for reading/writing!");
            msg.exec();
            return;
        }

        if (!document.setContent(&file))
        {
            msg.setText("Unable to parse file!");
            msg.exec();
            file.close();
            return;
        }

        file.close();

        QDomElement root = document.firstChildElement();
        if (root.isNull())
        {
            msg.setText("File is empty!");
            msg.exec();
            return;
        }

        QDomNodeList userList = root.elementsByTagName("user");
        if (userList.isEmpty())
        {
            msg.setText("File is empty!");
            msg.exec();
            file.close();
            return;
        }

        for(unsigned int i=0;i<(unsigned int)userList.count();i++)
        {
            QDomNode userNode = userList.at(i);
            if (userNode.isElement())
            {
                QDomElement userElement = userNode.toElement();

                if (userElement.attribute("id").toUInt() == user->Id())
                {
                    userElement.setAttribute("nickname", user->Nickname().c_str());
                    userElement.setAttribute("password", user->Password().c_str());
                    if (user->IsAdmin() == true)
                        userElement.setAttribute("isAdmin", 1);
                    else
                        userElement.setAttribute("isAdmin", 0);

                    QString pass = userElement.attribute("password");
                    QString xmlToSave = document.toString();

                    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
                    {
                        msg.setText("Unable to open file for reading/writing!");
                        msg.exec();
                        return;
                    }

                    file.write(xmlToSave.toStdString().c_str());
                    file.close();
                }
            }
        }

        QList <QTreeWidgetItem*> items = FillUsersList();
        for(unsigned int i=0;i<(unsigned int)items.count();++i)
        {
            if (items[i]->data(0, roles::ID).toUInt() == user->Id())
            {
                ui->UsersTree->setCurrentItem(items[i], 0);
                on_UsersTree_itemClicked(items[i], 0);
                break;
            }
        }

        QMessageBox msgComp("Update complete", "User data update completed", QMessageBox::Information, 0,0,0);
        msgComp.exec();
        return;
    }
}

void MainWindow::on_UserPasswordBox_editingFinished()
{
    User *user = usersList->ActiveUser();

    if (user == NULL)
    {
        QMessageBox msg("Error", "Such user does not exists!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }

    if (ui->UserPasswordBox->text().trimmed() == "")
    {
        QMessageBox msg("Error", "Password field cannot be empty!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        ui->UserPasswordBox->setText(user->Password().c_str());
        return;
    }
}

void MainWindow::on_AddUserButton_clicked()
{
    FillUserForm();
    QList <QTreeWidgetItem*> items = FillUsersList();
    ui->UsersTree->setCurrentItem(items.last(), 0);
    on_UsersTree_itemClicked(items.last(), 0);
}

void MainWindow::on_UserNameBox_editingFinished()
{
    User *user = usersList->ActiveUser();

    if (user == NULL)
    {
        QMessageBox msg("Error", "Such user does not exists!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }

    if (ui->UserNameBox->text().trimmed() == "")
    {
        QMessageBox msg("Error", "Nickname field cannot be empty!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        ui->UserNameBox->setText(user->Nickname().c_str());
        return;
    }

}

void MainWindow::on_DeleteUserButton_clicked()
{
    User *user = usersList->ActiveUser();
    if (user == NULL)
    {
        QMessageBox msg("Error", "Such user does not exists!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }
    else
    {
        for(unsigned int i=0;i<(unsigned int)usersList->Get()->size();++i)
        {
            if (usersList->GetUser(i) == user)
            {
                usersList->Get()->erase(usersList->Get()->begin()+i);
                delete user;
                FillUsersList();
                usersList->SaveUsersToFile();
                return;
            }
        }
    }
}
