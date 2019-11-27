#pragma once

#include <list>
#include <memory>
#include <type_traits>
#include "instruction.hpp"

namespace ice_language
{
class Code
{
  public:
    template <typename Ins>
    void add_ins()
    {
        instructions_.push_back(std::make_shared<Ins>());
    }

    template <typename Ins, typename T>
    void add_ins(T value)
    {
        if constexpr (std::is_same<T, nullptr_t>::value)
        {
            instructions_.push_back(std::make_shared<Ins>(nullptr));
        }
        else
        {
            instructions_.push_back(std::make_shared<Ins>(std::make_shared<T>(value)));
        }
    }

    std::list<std::shared_ptr<Instruction>> &get_instructions()
    {
        return instructions_;
    }

  private:

    std::list<std::shared_ptr<Instruction>> instructions_;
};
}
