#ifndef __ANOLE_OBJECTS_FUNC_HPP__
#define __ANOLE_OBJECTS_FUNC_HPP__

#include "object.hpp"

namespace anole
{
class Code;

class FunctionObject : public Object
{
  public:
    FunctionObject(SPtr<Scope> pre_scope, SPtr<Code> code, Size base, Size parameter_num);

    SPtr<Scope> scope();
    SPtr<Code>  code();
    Size        base();

  public:
    String to_str() override;
    Address load_member(const String &name) override;
    void call(Size num) override;
    bool is_callable() override;

    void collect(std::function<void(Scope *)>) override;

  private:
    SPtr<Scope> scope_;
    SPtr<Code> code_;
    Size base_;
    Size parameter_num_;
};
}

#endif
