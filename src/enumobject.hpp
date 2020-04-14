#pragma once

#include "scope.hpp"
#include "object.hpp"

namespace anole
{
class EnumObject : public Object
{
  public:
    EnumObject(SPtr<Scope> scope)
      : scope_(scope) {}

    SPtr<ObjectPtr> load_member(const std::string &name) override;

  private:
    SPtr<Scope> scope_;
};
}
