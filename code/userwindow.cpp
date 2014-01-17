#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(User *activeUser, Quiz *activeQuiz, QWidget *parent) : QWidget(parent), ui(new Ui::UserWindow), activeQuiz(activeQuiz), activeUser(activeUser), lastEdited(9999)
{
    ui->setupUi(this);

    if (activeQuiz == NULL || activeUser == NULL)
    {
        QMessageBox msg("Error", "Such quiz or user does not exist!", QMessageBox::Warning, 0,0,0);
        msg.exec();
    }
    else
    {
        mapper = new QSignalMapper(this);

        if (mapper != NULL)
        {
            for (unsigned int i=0;i<ActiveQuiz()->GetQuestions()->size();++i)
            {
                vector < bool > tmp;
                for (unsigned int j=0;j<ActiveQuiz()->GetQuestion(i)->GetAnswers()->size();++j)
                {
                    tmp.push_back(false);
                }
                answers.push_back(tmp);
            }

            FillQuestion();
        }
        else
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }
    }
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::closeEvent(QCloseEvent*)
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

void UserWindow::ActiveQuiz(Quiz* quiz)
{
    this->activeQuiz = quiz;
}

Quiz* UserWindow::ActiveQuiz()
{
    return activeQuiz;
}

void UserWindow::ActiveUser(User* user)
{
    this->activeUser = user;
}

User* UserWindow::ActiveUser()
{
    return activeUser;
}

void UserWindow::FillQuestion(unsigned int index)
{
    if (index < ActiveQuiz()->GetQuestions()->size())
    {
        Question* question = ActiveQuiz()->GetQuestion(index);
        if (question != NULL)
        {
            lastEdited = 9999;
            QLayoutItem *child;
            while ((child = ui->AnswerLayout->takeAt(0)) != 0)
            {
                delete child->widget();
                delete child;
            }

            ActiveQuiz()->ActiveQuestion(question);

            ui->QuesitonGroup->setTitle(question->Text().c_str());

            for(unsigned int i=0;i<(unsigned int)question->GetAnswers()->size();i++)
            {
                QCheckBox *checkBox = new QCheckBox(question->GetAnswer(i)->Text().c_str(), ui->AnswerLayout->widget());

                if(checkBox != NULL)
                {
                    if (answers[index][i] == true)
                        checkBox->setChecked(true);

                    ui->AnswerLayout->addWidget(checkBox);
                    mapper->setMapping(checkBox, i);
                    connect(checkBox, SIGNAL(stateChanged(int)), mapper, SLOT (map()));
                }
                else
                {
                    QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
                    msg.exec();
                }
            }

            connect(mapper, SIGNAL(mapped(int)), this, SLOT(checkBoxclicked(int)));

            unsigned int count = ActiveQuiz()->GetQuestions()->size()-1;
            double value = (double)index/count;
            ui->ProgressBar->setValue((unsigned int)(value*100));

            if (index != 0)
                ui->prevButton->setEnabled(true);
            else
                ui->prevButton->setEnabled(false);

            if (index >= ActiveQuiz()->GetQuestions()->size()-1)
                ui->nextButton->setText("End quiz");
            else
                ui->nextButton->setText("Next");
        }
        else
        {
            QMessageBox msg("Error", "Such question does not exist!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }
    }
    else
    {
        QMessageBox msg("Error", "Such question does not exist!", QMessageBox::Warning, 0,0,0);
        msg.exec();
    }
}

void UserWindow::CheckQuiz()
{
    totalPoints = 0;
    for(unsigned int i=0;i<ActiveQuiz()->GetQuestions()->size();++i)
    {
        bool answeredProper=true;
        bool isAnyTrue=false;

        for(unsigned int j=0;j<ActiveQuiz()->GetQuestion(i)->GetAnswers()->size();++j)
        {
            Answer* answer = ActiveQuiz()->GetQuestion(i)->GetAnswer(j);

            if (answer->IsProper() != answers[i][j])
                answeredProper = false;

            if (answers[i][j] == true)
                isAnyTrue = true;
        }

        if (answeredProper == true && isAnyTrue == true)
            totalPoints++;
    }

    QLayoutItem *child;
    while ((child = ui->AnswerLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    ui->Progress->setVisible(false);
    ui->QuesitonGroup->setTitle("Results");
    QLabel *label = new QLabel(this);
    if (label != NULL)
    {
        stringstream text;
        text.str("");
        text << "Your result: " << totalPoints << "/" << answers.size();
        label->setText(text.str().c_str());
        ui->AnswerLayout->addWidget(label);

        UsersList *users = new UsersList;
        if (users != NULL)
        {
            users->AddUsersFromFile();
            for (unsigned int i=0;i<users->Get()->size();++i)
            {
                if (users->GetUser(i)->Text() == ActiveUser()->Text())
                {
                    ActiveUser(users->GetUser(i));
                    ActiveUser()->Results()->insert(ActiveQuiz()->Text(), totalPoints);
                    users->SaveUsersToFile(false);
                    return;
                }
            }
        }
        else
        {
            QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
            msg.exec();
        }
    }
    else
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
    }
}

void UserWindow::checkBoxclicked(int index)
{
    for (unsigned int i=0;i<ActiveQuiz()->GetQuestions()->size();++i)
    {
        if (ActiveQuiz()->GetQuestion(i) == ActiveQuiz()->ActiveQuestion())
        {
            for(unsigned int j=0;j<answers[i].size();j++)
            {
                if (ActiveQuiz()->ActiveQuestion()->GetAnswer(j)->Id() == (unsigned int)index+1 && (unsigned int)index != lastEdited)
                {
                    answers[i][index] = !answers[i][index];
                    lastEdited = index;
                    return;
                }
            }
            return;
        }
    }
}

void UserWindow::on_nextButton_clicked()
{
    if (ui->nextButton->text().toLower() != "next")
    {
        CheckQuiz();
    }
    else
    {
        for(unsigned int i=0;i<ActiveQuiz()->GetQuestions()->size();++i)
        {
            if (ActiveQuiz()->GetQuestion(i)->Id() == ActiveQuiz()->ActiveQuestion()->Id())
            {
                if (i+1 < ActiveQuiz()->GetQuestions()->size())
                    FillQuestion(i+1);
                return;
            }
        }
    }
}

void UserWindow::on_prevButton_clicked()
{
    for(int i=0;i<(int)ActiveQuiz()->GetQuestions()->size();++i)
    {
        if (ActiveQuiz()->GetQuestion(i)->Id() == ActiveQuiz()->ActiveQuestion()->Id())
        {
            if (i-1 >= 0)
                FillQuestion(i-1);
            return;
        }
    }
}
