
#include "FrameWork.h"

int main(){
    FrameWork * frameWork = new FrameWork();

    if (frameWork->Initialize()) {
        frameWork->Run();
    }

    delete frameWork;

    return 0;
}