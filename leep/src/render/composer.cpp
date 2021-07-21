#include "composer.h"

namespace leep {

Composer::Composer()
{

}

Composer::~Composer()
{

}

void Composer::addEffect(IExecutable *effect)
{
    pp_list_.emplace_back(effect);
}

void Composer::reset()
{
    for (auto pp : pp_list_)
    {
        delete pp;
    }

    pp_list_.clear();
}

}