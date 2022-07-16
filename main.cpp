#include "async_shell_executor.h"
#include <iostream>

using namespace std;
using namespace coroutinetool;

int main(int argc, char **argv)
{
    auto ae = AsyncExecutor::Run("ping github.com");
    while (ae)
        cout << ae();
    cout << "return code is: " << ae.Code() << endl;
}