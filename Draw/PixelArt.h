#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

//Click h for the help menu

//Defining a bunch of unicode Charecters
wchar_t upperHalfBlock = 0x2580;    // ▀
wchar_t rightHalfBlock = 0x2590;    // ▐
wchar_t leftHalfBlock = 0x258C;     // ▌
wchar_t lowerHalfBlock = 0x2584;    // ▄
wchar_t underscore = '_';
wchar_t whiteBlock = 0x2586;        // ⊞
wchar_t enterSymbol = 0x2386;       // ⎆
wchar_t exitSymbol = 0x23CE;        // ⏎

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x)) //macro to move cursor
#define clear() printf("\e[1;1H\e[2J")

//Create the Node class
struct Node
{
    wchar_t type;
    int colour;
};

struct Node node;

//Allows the user to change the colours
const char* changeColours(int colour)
{
   
    switch (colour)
    {
    case 1:
        return "\033[1;30m";   //Changes to black
    case 2:
        return "\033[1;31m";   //Changes to red
    case 3:
        return "\033[1;32m";   //Changes to green
    case 4:
        return "\033[1;33m";   //Changes to yellow
    case 5:
        return "\033[1;34m";   //Changes to blue
    case 6:
        return "\033[1;35m";   //Changes to purple
    case 7:
        return "\033[1;36m";   //Changes to cyan
    case 8:
        return "\033[1;37m";   //Changes to white
    default:
        return "";
    }
}

//When it comes to the printing the individual charecters, I need to statically type out each individual charecter, otherwise it wont work properly with the clear statement
//Just trust me on this, it wont work.

void printGrid(int height, int width,struct Node ranArray[height][width])  
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // wprintf(L"%lc", ranArray[i][j].type); //This is all it should take
            //This if statement makes it work on linux machines, if on mac, delete the following and uncomment the above.
            printf("\033[0;37m");   //Changes the colour to white
            if (ranArray[i][j].type == upperHalfBlock)  //Checks if its the upper half block
            {
                printf("▀");    //Has to staticlly print the upper half block
                if (j < width - 3)
                {
                    printf("▀");
                }
            }
            else if (ranArray[i][j].type == rightHalfBlock)
            {
                printf("▐");
                
            }
            else if (ranArray[i][j].type == leftHalfBlock)
            {
                printf("▌");
            }
            else if (ranArray[i][j].type == lowerHalfBlock)
            {
                printf("▄");
                if (j < width - 3)
                {
                    printf("▄");
                }
            }
            else if (ranArray[i][j].type == whiteBlock)
            {
                printf("%s", changeColours(ranArray[i][j].colour));
                printf("▆");
            }
            else if (ranArray[i][j].type == underscore)
            {
                printf("_");
            }
            else
            {
                printf("␀");
            }

            if (j == width - 2)
            {
                continue;
            }

            // else if ((ranArray[i][j] == upperHalfBlock ||ranArray[i][j] == lowerHalfBlock) && j < width - 3)    //and this
            // {
            //     wprintf(L"%lc", ranArray[i][j]);
            // }

            else if (ranArray[i][j].type == underscore || ranArray[i][j].type == whiteBlock || ranArray[i][j].type == enterSymbol || ranArray[i][j].type == exitSymbol)
            {
                printf("\033[0;37m");
                printf("|");
            }
        }
        printf("\n");
    }
}

void PrintFinishedGrid(int height, int width, struct Node ranArray[height][width])  //Prints a preview of what the user had drawn, wihtout the lines
{
    for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if(ranArray[i][j].type == whiteBlock)
                {
                    printf("%s", changeColours(ranArray[i][j].colour));
                    printf("▆ ");
                }
                else
                {
                    printf("  ");
                }
            }
            printf("\n");
        }
}

void fill(int height, int width, struct Node ranArray[height][width], int colour, bool clear)   //Changes all blocks to a certain colour or clears the whole grid
{
    for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if(ranArray[i][j].type == whiteBlock && clear)
                {
                    ranArray[i][j].type = '_';
                    ranArray[i][j].colour = 8;
                }
                else if (ranArray[i][j].type == whiteBlock && fill)
                {
                    ranArray[i][j].colour = colour;
                }
                
                else
                {
                    continue;
                }
            }
        }
}

int Play(int height, int width)
{
    bool help = true;   //Used for the help comand
    bool loop = true;   //Used to keep looping through the code
    char quit;          //Used for the quit command
    int x = width;
    int y = height;
    // int temp;
    height += 2;
    width +=2;
    struct Node ranArray[height][width];    //Makes an array of nodes
    node.type = upperHalfBlock; //Sets the default node charecter
    int colour = 8;
    setlocale(LC_CTYPE, "");    //Allows you to enter unicode charecters

    int keystroke;
    static struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt); //storing terminal settings in oldt
    newt = oldt; //copying oldt to newt
    newt.c_lflag &= ~(ICANON | ECHO); //Turning off canoninal input bit and echo bit
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // ranArray[i][j] = node;
            if(i == 0)
            {
                ranArray[i][j].type = lowerHalfBlock;
            }    //The top row is the upper half blocks

            else if(i == height - 1)
            {
                ranArray[i][j].type = upperHalfBlock;
            } //The bottom row is lower half blocks
            else if(j == 0)
            {
                ranArray[i][j].type = leftHalfBlock;
            }    //The left row is left half
            else if(j == width - 1)
            {
                ranArray[i][j].type = rightHalfBlock;
            }   //The right row is right hlaf
            else
            {
                ranArray[i][j].type = underscore;
            }
        }
    }
    
    system("clear");

    printGrid(height, width, ranArray);
    gotoxy(x, y);
    int startNodeX;
    int startNodeY;

    while (loop)
    {
        keystroke = getchar();
        if (keystroke == 'w')
        {
            if (y == 2)
            {
                y ++;
                gotoxy(x,y);
            }
           
            y --;
            gotoxy(x,y);
        }
        else if (keystroke == 'a')
        {
            if (x == 2)
            {
                x +=2;
                gotoxy(x,y);
            }
           
            x -= 2;
            gotoxy(x,y);
        }
        else if (keystroke == 's')
        {
            if (y == height - 1)
            {
                y --;
                gotoxy(x,y);
            }
           
            y ++;
            gotoxy(x,y);
        }
        else if (keystroke == 'd')
        {
            if (x >= (width * 2) - 4)
            {
                x -=2;
                gotoxy(x,y);

            }
            x += 2;
            gotoxy(x,y);
        }
        else if (keystroke == 'g')  //Troubleshooting key
        {
            gotoxy(x,y);
            if (y == (width * 2) - 1)
            {
                continue;
            }
            else{
           
                int valx = y - 1;
                int valy = x / 2;
            }
        }
        else if (keystroke == ' ')
        {
            int valx = y - 1;
            int valy = x / 2;
            if (ranArray[valx][valy].type == '_')
            {
                ranArray[valx][valy].type = whiteBlock;
                ranArray[valx][valy].colour = colour;
                clear();
            }

            else
            {
                ranArray[valx][valy].colour = 8;
                ranArray[valx][valy].type = '_';
                clear();
            }
           
            printGrid(height, width, ranArray);
            gotoxy(x, y);
        }  
        else if (keystroke >= '1' && keystroke <= '8')
        {
            int valx = y - 1;
            int valy = x / 2;
            clear();
            printGrid(height, width, ranArray);
            printf("\n\n");
            switch (keystroke) // For Dylan
            {
            case '0':
                colour = 0;
            case '1':
                colour = 1;
                printf("%s", changeColours(colour));
                printf("\033[30;47m Changed colour to Black");
                printf("\033[30;0m");
                break;
            case '2':
                colour = 2;
                printf("%s", changeColours(colour));
                printf("Changed colour to Red");
                break;
            case '3':
                colour = 3;
                printf("%s", changeColours(colour));
                printf("Changed colour to Green");
                break;
            case '4':
                colour = 4;
                printf("%s", changeColours(colour));
                printf("Changed colour to Yellow");
                break;
            case '5':
                colour = 5;
                printf("%s", changeColours(colour));
                printf("Changed colour to Blue");
                break;
            case '6':
                colour = 6;
                printf("%s", changeColours(colour));
                printf("Changed colour to Purple");
                break;
            case '7':
                colour = 7;
                printf("%s", changeColours(colour));
                printf("Changed colour to Cyan");
                break;
            case '8':
                colour = 8;
                printf("%s", changeColours(colour));
                printf("Changed colour to White");
                break;
            case '9':
                colour = 9;
                break;
            }
            gotoxy(x, y);
        }        
        else if (keystroke == 'h')
        {
            // if(help)
            // {
                clear();
                printf("Type space to enter/remove a block\n");
                printf("Type q to quit the program\n");
                printf("Type /clear to clear the screen\n");
                printf("type /fill {colour} to fill the screen with a certain colour\n");
                printf("Use WASD to move\n\n");
                printf("****************\n");
                printf("* 1) Black     *\n");
                printf("* 2) Red       *\n");
                printf("* 3) Green     *\n");
                printf("* 4) Yellow    *\n");
                printf("* 5) Blue      *\n");
                printf("* 6) Purple    *\n");
                printf("* 7) Cyan      *\n");
                printf("* 8) White     *\n");
                printf("****************\n");
                printf("\n\n Press any key to exit");
                getchar();
                clear();
                printGrid(height, width, ranArray);
                // help = false;
            // }
            // else
            // {
            //     help = true;
            //     clear();
            //     printGrid(height, width, ranArray);
            // }
        }       
        else if (keystroke == 'q')
        {
             while(true)
            {
                system("clear");
                PrintFinishedGrid(height, width, ranArray);
                for (int i = 0; i < height; i++)
                    {
                        for (int j = 0; j < width; j++)
                        {
                            if(ranArray[i][j].type == whiteBlock)
                            {
                                printf("%s", changeColours(ranArray[i][j].colour));
                                printf("▆ ");
                            }
                            else
                            {
                                printf("  ");
                            }
                        }
                        printf("\n");
                    }
                printf("%s", changeColours(8));
                printf("Are you sure you want to stop drawing? ('y/n')");
                scanf("%c", &quit);
                if (quit == 'y' || quit == 'Y')
                {
                    system("clear");
                    PrintFinishedGrid(height, width, ranArray);
                    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
                    system("wmctrl -F -r :ACTIVE: -b remove,fullscreen");//Gets rid of the fullscreen
                    system("wmctrl -r :ACTIVE: -b toggle,maximized_vert,maximized_horz");//Maxamise to make sure the pattern is kept
                    return 0;
                }
                else if (quit == 'n' || quit == 'N')
                {
                    clear();
                    printGrid(height, width, ranArray);
                    break;
                }
                
                else
                {
                    printf("\nThat isn't a valid input\n");
                }
            }
        }
        else if(keystroke == '/')
        {
            clear();
            printGrid(height, width, ranArray);
            printf("\n\n/");
            char *buffer = (char *)malloc(100);
            char c;
            while ((c = getchar()) != '\n' && c != EOF && c != 127)
            {
                printf("%c", c);
                strncat(buffer, &c, 1);
            }
            clear();
            printGrid(height, width, ranArray);
            if(c == 127)
            {
                continue;
            }
            else if(strcmp(buffer, "clear") == 0)
            {
                fill(height, width, ranArray, 8, 1);
                clear();
                printGrid(height, width, ranArray);
            }

            else if(strstr(buffer, "fill"))
            {
                char* words;
                words = strtok(buffer, " ");
                words = strtok(NULL, " ");

                if (words == NULL)
                {
                    printf("Invalid Input\n");
                }
                else if (strlen(words) > 1)
                {
                    printf("Invalid input");
                }
                
                else if (words[0] > 48 && words[0] < 57)
                {
                    printf("%s", words);
                    colour = words[0] - 48;
                    fill(height, width, ranArray, colour, 0);
                    clear();
                    printGrid(height, width, ranArray);
                }
                
                else
                {
                    printf("Invalid Input\n");
                }
            }
            else
            {
                printf("That command doesn't exist");
            }
        
            free(buffer);
        }
    }
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt); //restoring original terminal configuration
    system("wmctrl -F -r :ACTIVE: -b remove,fullscreen");
    system("wmctrl -r :ACTIVE: -b toggle,maximized_vert,maximized_horz");
    system("clear");
    return 0;
}