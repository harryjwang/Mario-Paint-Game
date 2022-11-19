/*
 * Harry J. Wang
 * 
 */

#include <iostream> //used for inputs
#include <fstream> //used for streams
#include <string> //used for strings
using namespace std; //access std::

//function prototypes
void drawUD(fstream&, string, int, char, bool);
void drawLR(fstream&, string, int, char, bool);
void printFile();
bool checkDistance(string);

int main()
{

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////          DO  NOT CHANGE CODE BELOW THIS               //////////////
/////////////////////////////////////////////////////////////////////////////////////////

    ifstream infile("paint_base.txt");

    ofstream outfile("paint.txt", ios::binary);

    string c;

    if (infile)

        while (getline(infile, c))

            outfile << c << "\n";

    infile.close();

    outfile.close();

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////          DO  NOT CHANGE CODE ABOVE THIS               //////////////
/////////////////////////////////////////////////////////////////////////////////////////

    //START OF EDITABLE MAIN FUNCTION
    //START of SETUP process

    //declare variables used in the program
    ifstream reading; //stream to read file
    fstream writing; //stream to write to file

    string fileInput; //file name for input file (user input)

    string lookLine; //used for getline()
    long cut; //used to parse chars from each line

    string commands[5]; //string array to hold variables below
    bool pen_status; //is pen on/off
    string direction; //direction of movement
    int chars_moved; //distance moved
    bool bold_status; //decides if marking is bold or regular

    //if true, prints out what's written in the file so far to console
    bool print_status;

    //used to test for and get rid of completely blank lines
    char letter;

    //used to track where the cursor is and stays in 50x50
    int LRtrack = 1;
    int UDtrack = 1;
    
    //open the input file commands.txt and making sure it opened correctly
    do //ask at least once for the name of the input file
    {
        //Request commands input file name and save into string
        cout << "Enter input file name for commands:\n";
        cin >> fileInput;

        //open the input file given by user
        reading.open(fileInput, ios::binary);
        //if not opened correctly, tell user
        if(reading.fail())
            cout << "Input file not found/opened\n";

    } while (reading.fail()); //as long as input file can't be opened, keep asking for name
    
    //open the output file paint.txt (which should be blank except for 50 rows of 50 chars)
    writing.open("paint.txt", ios:: in | ios:: out);
    //check if output file opened
    if(writing.fail())
    {
        cout << "Output file not found/opened\n"; //tell user output didn't open;
        //since output file has to work and there's only one
        //if output file doesn't open then program can't run so exit program
        return 0;
    }
            
    //SETUP process FINISHED.


    //START of parsing and writing

    //keeps implementing getline() until it's impossible aka input file ends
    while (getline(reading, lookLine))
    {
        //parses each commands[i] for pen_status, direction, chars_moved, and bold/print status respectively (max 5 commands)
        for (int count = 0; count < 5; count++)
        {
            cut = lookLine.find(','); //find the first comma to cut to
            commands[count] = lookLine.substr(0, cut); //copy whatever is in front of the comma
            lookLine = lookLine.substr(cut + 1); //break off the command and then move to the char after the comma
            if (lookLine == "")
            {
                continue;
            }
        }

/*NOTE: this ^ works for when there's too many comma,not enough commas, as well as spaces and too many/not enough characters
 because if there's two commas (like: 1,,2) then after parsing the first digit (1) then the for loop parses nothing. 
Later on, there's a condition where if any of the commands don't fit the parameters of the variable (e.g. direction variable 
is a number or not one of the directions) then this specific run in the while loop is skipped using continue. */

        //set bool pen_status according to the first command value
        if (commands[0] == "1") //pen is off page
        {
            pen_status = false;
            
            //if pen is off then nothing is written so bold has to be false
            if (commands[3] == "B" || commands[4] == "B")
            {
                continue;
            }
            //checks if there's a "P" after chars_moved
            if (commands[3] == "P")
            {
                print_status = true;
            }
            else
            {
                print_status = false;
            }
            
        }
        else if (commands[0] == "2") //pen is on page
        {
            pen_status = true;

            //check for bold and print statuses since the pen is up
            //only has print_status
            if (commands[3] == "P")
            {
                print_status = true;
            }
                //has bold_status
            else if (commands[3] == "B")
            {
                bold_status = true;
                //has print_status as well
                if (commands[4] == "P")
                {
                    print_status = true;
                }
            }
            //has neither bold_status nor print_status
            //good practice just to make sure that statuses are defined
            else
            {
                bold_status = false;
                print_status = false;
            }
        }
        //if not 1 or 2 then skip command entirely
        else if (commands[0] != "1" || commands[0] != "2")
        {
            continue;
        }

        //set direction (N/S/E/W) equal to a variable
        direction = commands[1];

        //set chars_moved equal to command[2] converted to an int
        if (checkDistance(commands[2]))
        {
            if(stoi(commands[2]) >= 0 && stoi(commands[2]) < 51)
            {
                chars_moved = stoi(commands[2]);
            }
            else
            {
                continue;
            }
        }


        //depending on bold status, sets char letter equal to a symbol
        if (bold_status)
        {
            letter = '#';
        }
        else
        {
            letter = '*';
        }

        //depending on up/down or left/right movement, calls 1 of 2 different functions
        if (direction == "N" || direction == "S") //if commands[1] == N || S
        {
            //if the current position moved up chars_moved number of spaces up goes
            //above the first line then skip the command
            if(direction == "N" && (UDtrack - chars_moved) < 1)
            {
                continue;
            }
            //otherwise, add chars_moved to UDtrack so UD track is the current line
            else if (direction == "N")
            {
                UDtrack -= chars_moved;
            }
            //do the same for if direction is south, but check that the sum of UDtrack
            //and chars_moved doesn't pass the lowest line
            if(direction == "S" && (UDtrack + chars_moved) > 50)
            {
                continue;
            }
            //add chars_moved to UDtrack to update position
            else if (direction == "S")
            {
                UDtrack += chars_moved;
            }
            drawUD(writing, direction, chars_moved, letter, pen_status);
        }
        if (direction == "W" || direction == "E") //if commands[1] == W || E
        {
            //if the current position moved up chars_moved number of spaces to the left
            //goes to the left of first char then skip the command
            if(direction == "W" && (LRtrack - chars_moved) < 1)
            {
                continue;
            }
            //otherwise, add chars_moved to LRtrack so LRtrack is the current char
            else if (direction == "W")
            {
                LRtrack -= chars_moved;
            }
            //do the same for if direction is east, but check that the sum of LRtrack
            //and chars_moved doesn't pass the rightmost char
            if(direction == "E" && (LRtrack + chars_moved) > 50)
            {
                continue;
            }
            //add chars_moved to LRtrack to update position
            else if (direction == "E")
            {
                LRtrack += chars_moved;
            }
            
            drawLR(writing, direction, chars_moved, letter, pen_status);
        }

        //during each run of while loop, if print_status enabled, print out
        //current progress to console
        if(print_status)
        {
            printFile();
        }
    }
}

//FINISHED parsing and writing
//END OF MAIN FUNCTION


//BEGINNING OF FUNCTION BODIES
void drawUD(fstream &out, string path, int move, char alpha, bool pen)
{
    //if the pen_status variable is true pen is down
    if(pen)
    {
        //if direction string is N
        if(path == "N")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line up
                out.seekp(-51L, ios::cur);
                //if the char at the location is # then keep it same
                if(out.peek() == '#')
                {
                    out << '#';
                }
                    //if there's nothing there, print what is supposed to be there
                else
                {
                    out.put(alpha);
                }
                //code to move cursor back one (since printing out a value moves it forward)
                out.seekp(-1L, ios::cur);
            }
        }
            //if direction string is S
        else if(path == "S")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line down
                out.seekp(51L, ios::cur);
                //if the char at the location is # then keep it same
                if(out.peek() == '#')
                {
                    out << '#';
                }
                    //if there's nothing there, print what is supposed to be there
                else
                    out.put(alpha);
                //code to move cursor back one (since printing out a value moves it forward)
                out.seekp(-1L, ios::cur);
            }
        }
    }

        //if pen_status value is false, pen is up and not writing
    else
    {
        //if direction string is N
        if(path == "N")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line up
                out.seekp(-51L, ios::cur);
                out.clear();
            }
        }
            //if direction string is S
        else if(path == "S")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line down
                out.seekp(51L, ios::cur);
                //make sure area is blank after
                out.clear();
            }
        }
    }
}

void drawLR(fstream &out, string path, int move, char alpha, bool pen)
{
    //if pen_status value is true, pen is down
    if(pen)
    {
        //if direction string is E
        if(path == "E")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line to the right
                out.seekp(1L, ios::cur);
                //if the char at the location is # then keep it same
                if(out.peek() == '#')
                {
                    out << '#';
                }
                    //if there's nothing there, print what is supposed to be there
                else
                {
                    out.put(alpha);
                }
                //code to move cursor back one (since printing out a value moves it forward)
                out.seekp(-1L, ios::cur);
            }
        }
            //if direction string is W
        else if(path == "W")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line to the left
                out.seekp(-1L, ios::cur);
                //if the char at the location is # then keep it same
                if(out.peek() == '#')
                {
                    out << '#';
                }
                    //if there's nothing there, print what is supposed to be there
                else
                {
                    out.put(alpha);
                }
                //code to move cursor back one (since printing out a value moves it forward)
                out.seekp(-1L, ios::cur);
            }
        }
    }
        //Pen reading up
    else
    {
        //if direction string is E
        if(path == "E")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line up
                out.seekp(1L, ios::cur);
                out.clear();
            }
        }
            //if direction string is W
        else if(path == "W")
        {
            //move chars equal to variable distance value
            for (int count = 0; count < move; count++)
            {
                //find the location a line up
                out.seekp(-1L, ios::cur);
                out.clear();
            }
        }
    }
}

//void function to return to console the lines of the file that aren't completely
//blank (aka lines that have at least one char in them)
void printFile()
{
    //create new local istream to open paint.txt
    ifstream takeLine;
    //create local string to hold a line of reading from file each run through loop
    string printLine;
    //open paint.txt with istream
    takeLine.open("paint.txt", ios::in | ios::binary);
    takeLine.seekg(0, ios::beg);
    //while loop until the end of the file
    while (!takeLine.eof())
    {
        //read a line and save to printLine
        getline(takeLine, printLine);
        //if doesn't fulfill the checkLine function then
        if(printLine != " ")
        {
            cout << printLine << endl;
        }
    }
    //close the istream
    takeLine.close();
}

bool checkDistance(string move)
{
    for (int i = 0; i < move.length(); i++)
    {
        if(!isdigit(move[i]))
        {
            return false;
        }
    }
    return true;
}
//FUNCTION BODIES END
//END OF CPP FILE