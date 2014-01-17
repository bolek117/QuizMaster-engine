#include "commonHeaders.h"
#include "question.h"

Quiz::Quiz(bool withoutQuestions)
{
    if (withoutQuestions == false)
        ActiveQuestion(AddNewQuestion());
}

Quiz::Quiz(unsigned int id)
{
    ActiveQuestion(AddNewQuestion());
    this->Id(id);
}

Quiz::Quiz(unsigned int id, string name)
{
    ActiveQuestion(AddNewQuestion());
    this->Id(id);
    this->Text(name);
}

Quiz::~Quiz()
{
    vector < class Question* >::iterator it;
    for(it = questionsList.begin();it != questionsList.end();it++)
    {
        if (*it != NULL)
            delete *it;
    }
}

vector < Question * >* Quiz::GetQuestions()
{
    return &questionsList;
}

Question* Quiz::GetQuestion(unsigned int id)
{
    return questionsList[id];
}

Question* Quiz::AddNewQuestion(Question *question)
{
    unsigned int lastId=1;
    bool isNew=false;
    if (GetQuestions()->size() > 0)
        lastId = GetQuestions()->back()->Id()+1;

    if (question == NULL)
    {
        isNew = true;
        question = new Question();
    }

    if (question != NULL)
    {
        question->Parent((void*)this);
        GetQuestions()->push_back(question);

        question->Id(lastId);

        if (isNew)
        {
            stringstream buffer;
            buffer.str("");
            buffer << "Question " << lastId;
            question->Text(buffer.str());
        }

        return question;
    }
    else
    {
        QMessageBox msg;
        msg.setText("Memory allocation error!");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
    return NULL;
}

void Quiz::DeleteQuestion(unsigned int id)
{
    for(unsigned int i=0;i<questionsList.size();i++)
    {
        if (questionsList[i]->Id() == id)
        {
            questionsList.erase(questionsList.begin()+i);
            break;
        }
    }
}

unsigned int Quiz::SaveToFile()
{
    ofstream file;
    string filename = "quiz/";
    filename.append(this->Text());
    filename.append(".xml");
    file.open(filename.c_str(), ios_base::trunc);

    if (!file.is_open())
        return 0;

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<quizes>\n";
    file << "\t<quiz id=\"" << this->Id() << "\" name=\"" << this->Text() << "\">\n";
    for(unsigned int i=0;i<questionsList.size();i++)
    {
        file << "\t\t<question id=\"" << this->questionsList[i]->Id() << "\" text=\"" << this->questionsList[i]->Text() << "\">\n";

        for(unsigned int j=0;j<questionsList[i]->GetAnswers()->size();j++)
        {
            file << "\t\t\t<answer id=\"" << questionsList[i]->GetAnswer(j)->Id() << "\" text=\"" << questionsList[i]->GetAnswer(j)->Text() << "\" isProper=\"" << questionsList[i]->GetAnswer(j)->IsProper() << "\"/>\n";
        }

        file << "\t\t</question>\n";
    }

    file << "\t</quiz>\n";
    file << "</quizes>";
    file.close();
    return 1;
}

Quiz* Quiz::ReadFromFile(string filePath)
{
    QDomDocument document;
    QFile file(filePath.c_str());
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        msg.setText("Unable to open file!");
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

    QDomNodeList quizList = root.elementsByTagName("quiz");
    if (quizList.isEmpty())
    {
        msg.setText("File is empty!");
        msg.exec();
        file.close();
        return NULL;
    }

    for(unsigned int i=0;i<(unsigned int)quizList.count();i++)
    {
        QDomNode quizNode = quizList.at(i);
        if (quizNode.isElement())
        {
            QDomElement quizElement = quizNode.toElement();
            this->Text(quizElement.attribute("name").toStdString());

            QDomNodeList questionList = quizElement.elementsByTagName("question");

            for (unsigned int j=0;j<(unsigned int)questionList.count();j++)
            {
                QDomNode questionNode = questionList.at(j);
                if (questionNode.isElement())
                {
                    QDomElement questionElement = questionNode.toElement();
                    Question *question = new Question();
                    if (question == NULL)
                    {
                        msg.setText("Memory allocation error");
                        msg.exec();
                        file.close();
                        return NULL;
                    }
                    else
                    {
                        if (j == 0)
                            ActiveQuestion(question);

                        question->Id(j+1);
                        question->Text(questionElement.attribute("text").toStdString());
                        AddNewQuestion(question);

                        QDomNodeList answerList = questionElement.elementsByTagName("answer");
                        for (unsigned int k=0;k<(uint)answerList.count();k++)
                        {
                            QDomNode answerNode = answerList.at(k);
                            if (answerNode.isElement())
                            {
                                QDomElement answerElement = answerNode.toElement();
                                Answer *answer = new Answer();

                                if (answer == NULL)
                                {
                                    msg.setText("Memory allocation error!");
                                    msg.exec();
                                    file.close();
                                    return NULL;
                                }
                                else
                                {
                                    answer->Id(k+1);
                                    answer->Text(answerElement.attribute("text").toStdString());
                                    answer->IsProper((bool)answerElement.attribute("isProper").toInt());
                                    answer->Order(k+1);
                                    answer->Parent(question);
                                    question->AddNewAnswer(answer);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    file.close();
    return this;
}

Question* Quiz::SearchForQuestion(unsigned int id)
{
    for (unsigned int i=0;i<questionsList.size();i++)
    {
        if (GetQuestion(i)->Id() == id)
            return GetQuestion(i);
    }
    return NULL;
}

Question* Quiz::ActiveQuestion()
{
    return this->activeQuestion;
}

void Quiz::ActiveQuestion(Question *question)
{
    this->activeQuestion = question;
}

