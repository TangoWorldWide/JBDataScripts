// playerHours.cpp
// by 9yz
// orig. 5/13/21 - updated 7/9/21 to use args
// Takes a .csv from the datatracker program and outputs the number of times each map was recorded * the number of players online, per map. Will ignore the first line.

// ./playerhours <csv path> -wdc [number]
// -w: proccesses the last week of data (the last 2,016 entries)
// -d: proccesses the last day of data (288 entries)
// -c <number>: procces all data starting at line <number>. 0 for the beginning of the file.
// ex. ./playerhours results.csv -c 10562

// Each line in the csv should be in this format: timestamp (ignored),T players,CT players,mapname,T steamids (ignored),CT steamids (ignored).

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

//delimitors for columns and lines. you can probbably leave these at default unless you're using a weird csv format.
const char columnDelimitor = ','; 
const char lineDelimitor = '\n'; 


void seekToNextColumn(ifstream &);
void seekToNextLine(ifstream &);

bool loadCSV(string, vector <int> &, vector <string> &);
void analyze(vector <int> &, vector <string> &, vector <int> &, vector <string> &, int);
void sort(vector <int> &, vector <string> &);

int main(int argc, char** argv){

    //input validation, gotta put this first or the variables fuck it up
    if(argc > 4 && argc < 3){ //make sure we have the correct number of args
        cout << "Incorrect number of parameters entered.\n";
        return 1;
    }

    vector <int> playerCount{}; //input playercount
    vector <string> mapNames{}; //input mapnames
    vector <int> playerCountOut{}; //output playercount
    vector <string> mapNamesOut{}; //output mapnames

    string filePath(argv[1]); //getting the first param and casting it to string
    string startLocationParam(argv[2]); //getting what param (-wdc) to start with
    int startLocation; //if positive, start at that location, if -2, start at a week ago, if -1, start at a day ago

    char* end; //what the fuck does this do


    //more input validation
    if(startLocationParam == "-d"){
        startLocation = -1;
    }
    else if(startLocationParam == "-w"){
        startLocation = -2;
    }
    else if(startLocationParam == "-c"){
        if(argc == 4){ //anti-segfault: make sure there's actually a 3rd param

            if( strtol(argv[3], &end, 10) < 0){
                cout << "Invalid start position entered. Must be positive.\n";
            return 1;
            }

            startLocation = strtol(argv[3], &end, 10); //cast the final param from c-style string to int
        }
        else{
            cout << "-c requires a line number to start at.\n";
            return 1;
        }
    }
    else{
        cout << "Invalid parameter " << startLocationParam << " entered. Acceptable values are -w, -d, -c <number>.\n";
        return 1;
    }



    //actually running the program
    if( loadCSV(filePath, playerCount, mapNames) ){ //prompt for csv, load it into vectors. if it returns true, there was an error and we should exit the program.
        return 1;
    }

    analyze(playerCount, mapNames, playerCountOut, mapNamesOut, startLocation);

    sort(playerCountOut, mapNamesOut);

    for (int i = 0; i < playerCountOut.size(); i++) //run through all the recorded maps
    {
        cout << mapNamesOut[i] << ": " << playerCountOut[i] << endl; //and print them all out
    }

    return 0;
}




void seekToNextLine(ifstream &fileIn){
    string junk;
    getline(fileIn, junk, lineDelimitor); //reads from filein until we hit lineDelimitor. puts everything it reads into junk
    return;
}

void seekToNextColumn(ifstream &fileIn){
    string junk;
    getline(fileIn, junk, columnDelimitor); //reads from filein until we hit columnDelimitor. puts everything it reads into junk
    return;
}

bool loadCSV(string filePath, vector <int> &playerCount, vector <string> &mapNames){ //Prompts and loads the input CSV file. returns true if there's an error
    string num1, num2; // for storing the numbers before we add to vector
    string name1; //for storing the mapname before adding to vector
    string junk;
    
    ifstream fileIn(filePath); 

    cout << "loading file...\n";

    if( fileIn.fail() ){ //make sure the file opened properly
        cerr << "could not open file. \n";
        return true;
    }

    seekToNextLine(fileIn); //first line is just header info, we don't need to copy it.


    while( !fileIn.eof() ){ //checks for eof
        seekToNextColumn(fileIn); //we skip the first column (timestamp)

        getline(fileIn, num1, columnDelimitor); //get the first number (t players). get tata from, put data in, stop at this character
        getline(fileIn, num2, columnDelimitor); //get 2nd number (ct players)
        playerCount.push_back( stoi(num1) + stoi(num2) ); //add numbers together and put into vector

        getline(fileIn, name1, columnDelimitor); //get mapname
        mapNames.push_back(name1); //add to vector

        seekToNextLine(fileIn); //we're done getting data from this line, move to the next

    }

    fileIn.close();

    cout << "done loading!\n";
    cout << playerCount.size() << " entries loaded.\n";

    return false;
}

void analyze(vector <int> &playerCount, vector <string> &mapNames, vector <int> &playerCountOut, vector <string> &mapNamesOut, int startPosition){ //runs through all of the entries in the vector and counts up the total players for each map
    bool success = false;

    if(startPosition == -1){
        startPosition = playerCount.size() - 288; //start at 288 entries ago (1 day) 
    }
    else if(startPosition == -2){
        startPosition = playerCount.size() - 2016; //start at 2016 entries ago (1 week)
    } //if it's any other number we can just start there.

    cout << "Starting at line " << startPosition << endl;
    cout << "Analylizing...\n";

    for (int i = startPosition; i < playerCount.size(); i++) //go through all of the entries
    {
        for (int j = 0; j < mapNamesOut.size(); j++) //and go through all of the unique maps we've recorded so far
        {
            if( mapNames[i] == mapNamesOut[j] ){ //and if the entry has a map we've seen before
                playerCountOut[j] += playerCount[i]; //then add the players from the entry to the recorded map
                success = true;
            }
        }
        if(success == false){ //if we went through all the recorded maps and we haven't recorded this map yet...
            playerCountOut.push_back(playerCount[i]); //record the new map
            mapNamesOut.push_back(mapNames[i]); //and record its players
        }
        success = false;
        
    }

    cout << "done analyzing!\n";

    return;

}

void sort(vector <int> &playerCountOut, vector <string> &mapNamesOut){ //sorts the entries from most players to least players
    bool success = true;
    while(success) //keep looping until it's sorted
    { 
        success = false;

        for (int i = 0; i < playerCountOut.size() - 1; i++) //run through all of the entries
        {
            if( playerCountOut[i] > playerCountOut[i+1] ){ //if the lower number is bigger than the upper one,

                swap( playerCountOut[i],playerCountOut[i+1] ); //swap them
                swap( mapNamesOut[i], mapNamesOut[i+1] ); //and swap the names
                success = true; //if we manage to get through the whole list without making it true, it's sorted.
            }
        }
        
    }

    return;
}




