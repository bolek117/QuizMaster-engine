#ifndef TEST_H
#define TEST_H

#include "commonHeaders.h"

class Quiz : public Element
{
private:
    vector < Question * > questionsList;
    Question* activeQuestion;

public:
    Quiz(bool withoutQuestions=false);
    Quiz(unsigned int id);
    Quiz(unsigned int id, string name);
    ~Quiz();

    vector<Question *> *GetQuestions();
    Question* GetQuestion(unsigned int id);
    void SetQuestions(vector < Question * > sourceList);
    Question *AddNewQuestion(Question *question=NULL);

    //unsigned int InsertQuestion(Question *question);
    void DeleteQuestion(unsigned int id);

    unsigned int SaveToFile();
    Quiz* ReadFromFile(string filepath);

    Question* SearchForQuestion(unsigned int id);

    Question* ActiveQuestion();
    void ActiveQuestion(Question *question);
};

#endif // TEST_H
