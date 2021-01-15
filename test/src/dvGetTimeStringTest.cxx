#include <iostream>
#include <dvGetTimeString.h>

int main(int argc, char** argv) {

  std::cout << dv::GetTimeString() << std::endl;
  std::cout << dv::GetTimeString("%A %B %d, %I:%M %p") << std::endl;

}
