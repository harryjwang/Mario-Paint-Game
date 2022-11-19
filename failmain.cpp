#include <iostream>
#include<fstream>
#include <string>
using namespace std;


//function prototypes for drawing
void drawUD(fstream &, bool, int, string, char);
void drawLR(fstream &, bool, int, string, char);
void printFile();
bool checkSpace(string);

int main()
{
   //this down here basically copies the paint_base.txt boundaries (51x50) into a blank new
   //file called paint.txt so that we can actually draw on that new file.

   /////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////          DO  NOT CHANGE CODE BELOW THIS               //////////////
   /////////////////////////////////////////////////////////////////////////////////////////

   ifstream infile("paint_base.txt");
   ofstream outfile("paint.txt");
   char c;
   if (infile)
      while (infile.get(c))
         outfile << c;

   infile.close();
   outfile.close();

   /////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////          DO  NOT CHANGE CODE ABOVE THIS               //////////////
   /////////////////////////////////////////////////////////////////////////////////////////

   //CODE STARTS HERE

   //declaring variables
   fstream drawing; //to draw on paint.txt
   ifstream reading; //to read commands from
   bool pen_status, bold_status, print_status; //test if pen's status and mark
   int num_characters; //number of char movements
   string direction; //direction of movement
   string check; //used for get() to check each char in the command
   int index; //to read lines
   string commands[4]; //to store the commands to a string array
   char letter;

   //open both input and output files
   drawing.open("paint.txt", ios::in | ios::out | ios::binary);
   reading.open("commands.txt", ios::in | ios::binary);


   //while loop for as long as the command file can be read
   //outputs to a string
   while(getline(reading,check))
   {
      bold_status = false;
      //for loop to set all 5 (max) commands in a line equal to a string array
      for(int i = 0; i < 5; i++)
         {
            index = check.find(','); //find the first comma
            commands[i] = check.substr(0,index); //copy everything in front of the index
            check = check.substr(index+1); //break off the name and comma from the string
         }
         /*conditions for the different commands
         commands[0] = pen up or pen down
         commands[1] = direction
         commands[2] = movement
         commands[3] = maybe Bold or Print
         commands[4] = maybe print */
         if (commands[0] == "1")
            pen_status = false; //pen off
         else if (commands[0] == "2")
            pen_status = true; //pen on

         num_characters = stoi(commands[2]); //distance moved
         direction = commands[1]; //direction moved
         //if the 4th and/or 5th commands have the letters B or P
         if (commands[3] == "B")
         {
            bold_status = true;
            if (commands[4] == "P")
            {
               print_status = true;
            }
         }
         else if (commands[3] == "P")
         {
            print_status = true;
            bold_status = false;
         }
         else
         {
            bold_status = false;
            print_status = false;
         }

         if(bold_status)
            letter = '#';
         else
            letter = '*';
         if (direction == "N" || direction == "S") //move up/down
         {
            drawUD(drawing, pen_status, num_characters, direction, letter);
         }
         else
         {
            drawLR(drawing, pen_status, num_characters, direction, letter); //move left/right
         }
         if(print_status)
         {
            printFile();
         }
   }
}
//FUNCTION bodies
//function 1 (up/down movement)
void drawUD(fstream &drawing, bool pen_status, int num_characters, string direction, char letter)
{
   //if first number = 1 aka pen is not drawing
   if(pen_status == false)
   {
      if(direction == "N")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(-52L, ios::cur);
            drawing.clear();
         }
      }
      else if (direction == "S")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(52L, ios::cur);
            drawing.clear();
         }
      }
   }
   //if first number = 2 aka pen is drawing
   if(pen_status == true)
   {
      //if the direction indicated is north
      if(direction == "N")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(-53L, ios::cur);
            if(drawing.peek() == '#'){
               drawing << '#';
            }
            else
            {
               drawing << letter;
            }
         }
      }
      else if (direction == "S")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(51L, ios::cur);
            if(drawing.peek() == '#'){
               drawing << '#';
            }
            else
            {
               drawing << letter;
            }
         }
      }
   }
}


//function 2 (left/right movement)
void drawLR(fstream &drawing, bool pen_status, int num_characters, string direction, char letter)
{
   //if first number = 1 aka pen is not drawing
   if(pen_status == false)
   {
       if(direction == "E")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(1L, ios::cur);
            drawing.clear();
         }
      }
      else if (direction == "W")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(-1L, ios::cur);
            drawing.clear();
         }
      }
   }
   //if first number = 2 aka pen is drawing
   else if(pen_status == true)
   {
      //if the direction indicated is north
      if(direction == "E")
      {
         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(0L, ios::cur);
            if(drawing.peek() == '#'){
               drawing << '#';
            }
            else
            {
               drawing << letter;
            }
         }
      }
      else if (direction == "W")
      {

         for(int count = 0; count < num_characters; count++)
         {
            drawing.seekp(-2L, ios::cur);
            if(drawing.peek() == '#'){
               drawing << '#';
            }
            else
            {
               drawing << letter;
            }
         }
      }

   }
}


//function 3 (prints out file)
void printFile()
{
   ifstream printOut; //create ifstream
   string outPutLine; //getLine into this string
   printOut.open("paint.txt", ios::in | ios::binary); //open to paint.txt;
   while (!printOut.eof())
   {
      //while the file doesn't finish, get a line from paint.txt
      //and check that it's not empty; then output it to console
      getline(printOut, outPutLine);
      if (!checkSpace(outPutLine))
         cout << outPutLine << endl;
   }
   printOut.close(); //close the ifstream after
}

bool checkSpace(string line)
{
   for(unsigned long i = 0; i < line.size(); i++)
   {
      if (line[i] != ' ')
         return false;
   }
   return true;
}
