#ifndef ANSWER_H
#define ANSWER_H

#include "commonHeaders.h"

class Answer : public Element
{
private:
    bool isProper;
    unsigned int order;

public:
    Answer() : isProper(false) {}
    Answer(bool isProper) : isProper(isProper) {}
    Answer(unsigned int id, string text, unsigned int order, bool isProper=false, void* parent=NULL) : Element(id, text, parent), isProper(isProper), order(order) {}

    void IsProper(bool value);
    bool IsProper();

    void Order(unsigned int order);
    unsigned int Order();
};

#endif // ANSWER_H
