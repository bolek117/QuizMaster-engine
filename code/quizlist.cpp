#include "quizlist.h"

QuizList::QuizList()
{
    quizList = new vector<Quiz*>;

    if (quizList == NULL)
    {
        QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
        msg.exec();
        return;
    }
}

QuizList::~QuizList()
{
    if (quizList != NULL)
    {
        for (unsigned int i=0;i<(unsigned int)quizList->size();i++)
            delete quizList->operator [](i);

        delete quizList;
    }
}

Quiz* QuizList::AddNewEmptyQuiz(Quiz *quiz)
{
    unsigned int id = quizList->size()+1;
    bool isNew = false;

    if (quiz == NULL)
    {
        quiz = new Quiz(id);
        isNew = true;
    }

    if (quiz != NULL)
    {
        quiz->Id(id);
        quizList->push_back(quiz);

        if (quizList->size() > 0)
        {
            if (isNew)
            {
                stringstream buffer;
                buffer.str("");
                buffer << "New quiz " << quiz->Id();
                quiz->Text(buffer.str());
            }

            ActiveQuiz(quiz);
            return quiz;
        }
        else
        {
            QMessageBox msg;
            msg.setText("Unknown error occured.");
            msg.setIcon(QMessageBox::Warning);
            msg.exec();
        }
    }
    else
    {
        QMessageBox msg;
        msg.setText("Memory allocation error.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
    return NULL;
}

vector < Quiz * >* QuizList::Get()
{
    return quizList;
}

Quiz* QuizList::GetQuiz(unsigned int i)
{
    return quizList->operator [](i);
}

Quiz* QuizList::SearchForQuiz(unsigned int id)
{
    for(unsigned int i=0;i<quizList->size();i++)
        if (GetQuiz(i)->Id() == id)
            return GetQuiz(i);

    return NULL;
}

Quiz* QuizList::ActiveQuiz()
{
    return this->activeQuiz;
}

void QuizList::ActiveQuiz(Quiz *quiz)
{
    activeQuiz = quiz;
}

vector < Quiz*> *QuizList::AddQuizesFromFolder(string path)
{
    QDir dir(path.c_str());
    QStringList list = dir.entryList(QStringList("*.xml"));

    Quiz *quiz;

    if (list.size() == 0)
    {
        quiz = new Quiz();
        quiz->Id(1);
        quiz->Text("New quiz 1");
        quizList->push_back(quiz);
    }

    for(unsigned int i=0;i<(unsigned int)list.size();i++)
    {
        quiz = new Quiz(true);
        if (quiz != NULL)
        {
            QString filePath = list.at(i);
            stringstream buffer;
            buffer.str("");
            buffer << "quiz/" << filePath.toStdString();

            quiz->ReadFromFile(buffer.str());
            quiz->Id(i);

            if (quiz == NULL)
            {
                QMessageBox msg("Error", "Memory allocation error!", QMessageBox::Warning, 0,0,0);
                msg.exec();
                return NULL;
            }

            quizList->push_back(quiz);
        }
    }
    ActiveQuiz(quizList->front());
    return quizList;
}
