#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <physics.h>
#include <vector>
#include <object.h>
#include <string>
#include <memory>

using namespace std;

class Component
{
public:
    Component();

    string name;
    ComponentProps properties;
};

struct CTNode
{
    Component component;
    vector<shared_ptr<CTNode>> children;

    CTNode(Component comp) : component(comp) {}
};

CTNode ciruit_mapper()
{
    
}
#endif