
#include "gameareaview.h"

void VGAView::cycle(){
    do{
        output();
    }
    while(input()!= MSG_EXIT);
}
