#ifndef ELEMENT_H
#define ELEMENT_H

#include "commonHeaders.h"

class Element
{
private:
    string text;
    unsigned int id;
    void* parent;

public:
    Element(unsigned int id=0, string text="", void* parent=NULL) : text(text), id(id), parent(parent) {}
    Element(string text) : text(text), id(0), parent(NULL) {}

    string Text();
    void Text(string value);

    unsigned int Id();
    void Id(unsigned int value);

    void* Parent();
    void Parent(void* pointer);
};

#endif // ELEMENT_H
