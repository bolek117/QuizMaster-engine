#include "commonHeaders.h"

Question::Question()
{
    answersList = new vector < Answer*>;
}

Question::~Question()
{
    vector < Answer * >::iterator it;
    for(it = answersList->begin();it != answersList->end();it++)
        delete *it;

    delete answersList;
}

vector < Answer * > *Question::GetAnswers()
{
    return answersList;
}

Answer* Question::GetAnswer(unsigned int i)
{
    unsigned int size = answersList->size();
    if (i < size)
        return answersList->operator [](i);
    else
        return NULL;
}

void Question::SetAnswers(vector < Answer * > *sourceList)
{
    this->answersList = sourceList;
}

Answer* Question::AddNewAnswer(Answer *answer)
{
    if (answer == NULL)
    {
        answer = new Answer();
        if (answer != NULL)
        {
            answer->Parent(this);

            unsigned int id = 1;
            if (GetAnswers()->size() != 0)
                id = GetAnswers()->back()->Id()+1;

            answer->Id(id);
            stringstream buffer; buffer.str(""); buffer << "Answer " << id;
            answer->Text(buffer.str());
            answer->IsProper(false);
            answer->Order(GetAnswers()->size()+1);

            answersList->push_back(answer);
        }
        else
        {
            QMessageBox msg;
            msg.setText("Memory allocation error!");
            msg.setIcon(QMessageBox::Warning);
            msg.exec();
            return NULL;
        }

    }
    else
        answersList->push_back(answer);

    return answer;
}

void Question::DeleteAnswer(unsigned int id)
{
    Answer *answer = SearchForAnswer(id);
    if (answer != NULL)
    {
        delete answer;
        for (unsigned int i=0;i<GetAnswers()->size();i++)
            if (GetAnswer(i) == answer)
                answersList->erase(answersList->begin()+i);

        RefreshOrder();
    }
}

void Question::RefreshOrder()
{
    for(unsigned int i=0;i<answersList->size();i++)
        GetAnswer(i)->Order(i+1);
}

Answer* Question::SearchForAnswer(unsigned int answerId)
{
    for(unsigned int i=0;i<GetAnswers()->size();i++)
    {
        if (GetAnswer(i)->Id() == answerId)
            return GetAnswer(i);
    }
    return NULL;
}
