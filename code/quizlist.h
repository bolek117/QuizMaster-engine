#ifndef QUIZLIST_H
#define QUIZLIST_H
#include "commonHeaders.h"

class QuizList
{
private:
    vector < Quiz * > *quizList;
    Quiz* activeQuiz;

public:
    QuizList();
    ~QuizList();

    Quiz* AddNewEmptyQuiz(Quiz *quiz=NULL);
    vector < Quiz * > *Get();
    Quiz* GetQuiz(unsigned int i);
    Quiz* SearchForQuiz(unsigned int id);
    Quiz* ActiveQuiz();
    void ActiveQuiz(Quiz *quiz);
    vector < Quiz*> *AddQuizesFromFolder(string path="quiz");

};

#endif // QUIZLIST_H
