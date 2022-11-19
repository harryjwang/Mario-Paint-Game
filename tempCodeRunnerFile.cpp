{
    out.seekg(0, ios::beg);
    //while loop until the end of the file
    while (!out.eof())
    {
        //read a line and save to printLine
        getline(out, printLine);
        //if doesn't fulfill the checkLine function then
        if(checkingSpace(printLine))
        {
            cout << printLine << endl;
        }
    }
}

bool checkingSpace(string printLine)