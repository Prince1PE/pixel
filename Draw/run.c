#include "PixelArt.h"


int main(int argc, char* argv[])    //Takes command line input
{
    int x = 60; //Sets default to 50 by 100
    int y = 100;

    if (argc == 2)  //If the user entered 1 argument
    {
        x = atoi(argv[1]);  //convert it to an integer
    }


    else if( argc == 3 ) {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }
    else if( argc > 3 ) {   
        printf("Too many arguments supplied.\n");
        return -1;
    }

    if (x < 5 || y < 5) //Makes sure there values entered are higher than 5
    {
        printf("You need to put values higher than 5\n");
        return -1;
    }
    else if(x > 60 || y > 104)  //Stops you from enterring too high of a value
    {
        printf("The values you have enterred are too high\n");
        return -1;
    }
    system("wmctrl -F -r :ACTIVE: -b add,fullscreen");// Sets the terminal to fullscreen
    Play(x, y);
    return 0;
}
