#pragma once

#include "code.hpp"
#include "scope.hpp"
#include "object.hpp"

namespace ice_language
{
/*
Draft

*/
class FunctionObject : public Object
{
  public:
    FunctionObject(Ptr<Scope> pre_scope,
        Code &code, std::size_t base,
        std::size_t args_size)
      : scope_(std::make_shared<Scope>(pre_scope)),
        code_(code), base_(base),
        args_size_(args_size) {}

    Ptr<Scope> scope() { return scope_; }
    Code &code() { return code_; }
    std::size_t base() { return base_; }
    std::size_t args_size() { return args_size_; }

  private:
    Ptr<Scope> scope_;
    Code &code_;
    std::size_t base_;
    std::size_t args_size_;
};
}