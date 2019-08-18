#include <stdio.h>
#include <stdlib.h>


void takePic() {
        system("raspistill -o Pub.jpg -t 1 -w 640 -h 480");
}
