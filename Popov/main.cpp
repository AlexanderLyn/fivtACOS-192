#include <iostream>
#include <vector>

int main(int argc, const char * argv[])
{
  int n = 100000000, i = 0, sum = 0;
  time_t t = time(NULL);
  std::vector<int> a(n), b(n), c(n);

  std::cout<<time(NULL) - t<<" init time"<<std::endl;
  
  for (i = 0; i < n; ++i) {
    a[i] = i;
    b[i] = i;
  }
  std::cout<<time(NULL) - t<<" fill time"<<std::endl;

  for (i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
    sum += c[i];
  }
  std::cout<<time(NULL) - t<<" summ time"<<std::endl;
  
  return 0;
}

