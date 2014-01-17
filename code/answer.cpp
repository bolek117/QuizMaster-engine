#include "answer.h"

void Answer::IsProper(bool value)
{
    this->isProper = value;
}

bool Answer::IsProper()
{
    return this->isProper;
}

void Answer::Order(unsigned int order)
{
    this->order = order;
}

unsigned int Answer::Order()
{
    return this->order;
}
