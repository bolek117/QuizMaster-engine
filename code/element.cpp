#include "commonHeaders.h"

string Element::Text()
{
    return this->text;
}

void Element::Text(string value)
{
    this->text = value;
}

unsigned int Element::Id()
{
    return this->id;
}

void Element::Id(unsigned int value)
{
    this->id = value;
}

void* Element::Parent()
{
    return this->parent;
}

void Element::Parent(void* pointer)
{
    this->parent = pointer;
}
