#ifndef QUESTION_H
#define QUESTION_H

#include "commonHeaders.h"

class Question : public Element
{
private:
    vector < class Answer * > *answersList;

public:
    Question();
    ~Question();

    vector < Answer * > *GetAnswers();
    Answer *GetAnswer(unsigned int i);
    void SetAnswers(vector < Answer * > *sourceList);

    Answer *AddNewAnswer(Answer *answer=NULL);
    void DeleteAnswer(unsigned int id);

    void RefreshOrder();
    Answer* SearchForAnswer(unsigned int answerId);
};

#endif // QUESTION_H
