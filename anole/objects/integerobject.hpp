#ifndef __ANOLE_OBJECTS_INTEGER_HPP__
#define __ANOLE_OBJECTS_INTEGER_HPP__

#include "object.hpp"

namespace anole
{
class IntegerObject : public Object
{
  public:
    IntegerObject(int64_t value) noexcept
      : Object(ObjectType::Integer)
      , value_(value)
    {
        // ...
    }

    int64_t value() const noexcept
    {
        return value_;
    }

  public:
    bool to_bool() override;
    String to_str() override;
    String to_key() override;
    Object *neg() override;
    Object *add(Object *) override;
    Object *sub(Object *) override;
    Object *mul(Object *) override;
    Object *div(Object *) override;
    Object *mod(Object *) override;
    Object *ceq(Object *) override;
    Object *cne(Object *) override;
    Object *clt(Object *) override;
    Object *cle(Object *) override;
    Object *bneg() override;
    Object *bor(Object *) override;
    Object *bxor(Object *) override;
    Object *band(Object *) override;
    Object *bls(Object *) override;
    Object *brs(Object *) override;
    Address load_member(const String &name) override;

  private:
    int64_t value_;
};
}

#endif
