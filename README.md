# coroutinetool
functional c++ tiny lib based on c++20 coroutine,for learning and utility purpose.

currently a async shell executor implemented.You could run system command in place with calling `fgets` etc...  

## Start

```c++
#include "async_shell_executor.h"
#include <iostream>

using namespace std;
using namespace coroutinetool;

int main(int argc, char **argv)
{
    auto ae = AsyncExecutor::Run("ipconfig");
    while (ae)
        cout << ae();
    cout << "return code is: " << ae.Code() << endl;
    ae = AsyncExecutor::Run("ipconfig");
    while (ae)
        cout << ae();
    cout << "return code is: " << ae.Code() << endl;
}
```
