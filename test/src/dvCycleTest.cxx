#include <cassert>
#include <dvCycle.h>
#include <iostream>

int
main()
{
  {
    using UIntCycle = dv::Cycle<unsigned int>;
    auto uintCycle = UIntCycle(10);
    assert(0 == uintCycle.GetCurrent());
    assert(1 == uintCycle.GetNext());
    assert(9 == uintCycle.GetPrevious());
    uintCycle.Increment();
    assert(1 == uintCycle.GetCurrent());
    uintCycle.Decrement();
    assert(0 == uintCycle.GetCurrent());
    uintCycle.Decrement();
    assert(9 == uintCycle.GetCurrent());
  }
  {
    using IntCycle = dv::Cycle<int>;
    auto uintCycle = IntCycle(-5, 11, -5);
    assert(-5 == uintCycle.GetCurrent());
    assert(-4 == uintCycle.GetNext());
    assert(+5 == uintCycle.GetPrevious());
    uintCycle.Increment();
    assert(-4 == uintCycle.GetCurrent());
    uintCycle.Decrement();
    assert(-5 == uintCycle.GetCurrent());
    uintCycle.Decrement();
    assert(+5 == uintCycle.GetCurrent());
  }
  return EXIT_SUCCESS;
}
