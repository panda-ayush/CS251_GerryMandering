//
// Project 1 Starter Code - Gerrymandering
// @file main.cpp (program1)
// @author Ayush Panda
// @UIN 671442036
// @course CS 251 1 PM MWF
// @date 1/23/23
// @brief description Recently gerrymandering has been a popular topic in the news and the subject of an ongoing Supreme Court case. Gerrymandering is creating voting districts in such a 
// way that gives one party an unfair advantage over the other. Political parties do this to try to help themselves stay in power. Recently, a research group came up with a mathematical definition 
// for what constitutes gerrymandering. For this project, you will write a code that determines whether the state’s districts are gerrymandered, according to the researchers’ definition. The input 
// data about states’ districts and total voters comes from two input files. Even if you disagree with this definition of gerrymandering, it is interesting to understand it better as the courts are currently debating it.  


#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include "ourvector.h"

using namespace std;

struct eligibleStruct { //struct for eligible data files
    ourvector <string> state;
    ourvector <int> voters;
};

struct disctrictStruct { //struct for district data files
    ourvector <string> state;
    ourvector < ourvector<int> > district;
    ourvector < ourvector<int> > demVote;
    ourvector < ourvector<int> > repVote;
};

void findDistrictState (string &fileLine, ourvector <string> &vectorStateName) { //find the state names and remove it from the file vector so that you can distinguish the district number, democrat votes and republican votes easier
    string stateName;
    int index = fileLine.find(",");
    stateName = fileLine.substr(0, index);
    vectorStateName.push_back(stateName);
    fileLine.erase(0, index+1);
}

void findDistrictNumber (string &fileData, ourvector <int> &tempDistrictNum) { //finds the number of districts per state and saves it to the temporary vector
    int districtNum;

    int index = fileData.find(",");

    if (fileData.substr(0, index) != "AL") { //if the file data is = AL then it pushes back 1 as the number of districts in the state
        districtNum = stoi(fileData.substr(0, index));
        tempDistrictNum.push_back(districtNum);
        fileData.erase(0, index+1);
    }
    else {
        districtNum = 1; //pass back the number of districts in each state to the temporary vector
        tempDistrictNum.push_back(districtNum);
        fileData.erase(0, index+1);
    }

}

void findDemNumber (string &fileData, ourvector <int> &tempDemNum) { //finds the number of democrat votes and pushes it back to the temporary vector
    int demNum;

    int index = fileData.find(","); //converts number from string to integer and saves everything until the next comma in the temporary democrat voters vector 
    demNum = stoi(fileData.substr(0, index));
    tempDemNum.push_back(demNum);
    fileData.erase(0, index+1);
}

void findRepNumber (string &fileData, ourvector <int> &tempRepNum) { //finds the republican votes number
    int repNum;

    int index = fileData.find(","); //saves data from begining of string till the next comma is found
    if (index != -1){
    repNum = stoi(fileData.substr(0, index));
    tempRepNum.push_back(repNum);
    fileData.erase(0, index+1);
    }
    else { //if a comma doesnt exist then it goes to the end of the string file and then clears the string once it has been pushed back to the temporary vector
        repNum = stoi(fileData.substr(0, fileData.size()-1));
        tempRepNum.push_back(repNum);
        fileData.clear();
    }
}

void sortDataRemainder (string &fileData, ourvector < ourvector<int> > &vectorDistrictNumber, 
ourvector < ourvector<int> > &vectorDemVote, ourvector < ourvector<int> > &vectorRepVote) {

    ourvector <int> tempDistrictNum; //temporary vectors that will be pushed back since the vector that holds the district numbers, democrat numbers and republican numbers
    ourvector <int> tempDemNum;
    ourvector <int> tempRepNum;

    while (fileData.size() != 0) { //loop through the filedata string and remove the district number, democrat number and republican number one by one
        
        findDistrictNumber(fileData, tempDistrictNum);

        findDemNumber(fileData, tempDemNum);

        findRepNumber(fileData, tempRepNum);

    }

    vectorDistrictNumber.push_back(tempDistrictNum); //push back the entire vector to the 2d vectors so that the index of the first vector will be the same as the district state making it easy to look up information based on the state
    vectorDemVote.push_back(tempDemNum);
    vectorRepVote.push_back(tempRepNum);

}

void loopVectorDistrict (ourvector <string> &fileVector, ourvector <string> &vectorStateName, 
ourvector < ourvector<int> > &vectorDistrictNumber, ourvector < ourvector<int> > &vectorDemVote, 
ourvector < ourvector<int> > &vectorRepVote) { //loop through temporary vector where each element is a line from the district file

    for (int i = 0; i < fileVector.size(); i++) { //for loop to pass through only the state names in the district state vector and remove that state name once it is in the distric state vector
        string element = fileVector.at(i);//passes entire line from file in as string
        findDistrictState(element, vectorStateName);
        fileVector.at(i) = element;
    }

    for (int i = 0; i < fileVector.size(); i++) { //loops through the remainder of the data once the state names have been removed
        string element2 = fileVector.at(i); //passes entire line from file in as string
        sortDataRemainder(element2, vectorDistrictNumber, vectorDemVote, vectorRepVote);
    }

}

void inputEligibleFile (string filename, ourvector <string> &vectorStateInstance, ourvector <int> &vectorVoterInstance, ourvector <string> &districtState) { //function to populate eligible data structs with data
    string tempLine; //temporary strings to hold data from the input file
    string stateString;
    int voterInt;

    ifstream infs;

    infs.open(filename);

    while(!infs.fail()) {
        getline(infs, tempLine);

        if (!tempLine.empty()) {//saves everything before the first comman in the line in the state vector and everything after the comma is saved in the voter instance vector
            for (int i = 0; i < districtState.size(); i++) {
                int index = tempLine.find(",");
                string temp = tempLine.substr(0, index);   
                if (temp == districtState.at(i)) {
                    stateString = tempLine.substr(0, index);
                    voterInt = stoi(tempLine.substr(index+1, tempLine.size()));
                    vectorStateInstance.push_back(stateString);
                    vectorVoterInstance.push_back(voterInt);
                }

            }

        }
        else {
            break;
        }


    }

    infs.close();
    
}

void inputDisctrictFile (string fileNameDistrict, ourvector <string> &vectorStateName, ourvector < ourvector<int> > &vectorDistrictNumber, 
ourvector < ourvector<int> > &vectorDemVote, ourvector < ourvector<int> > &vectorRepVote) { //function that starts calling functions that loop through the data in files
    ourvector <string> readLineVector; //vector to hold the entire line per file in each element of the element

    string readinLine;

    ifstream infs;

    infs.open(fileNameDistrict);

    while(!infs.fail()) { //populate readlinevector where each element is an entire line from the district data file
        getline(infs, readinLine);
        if (readinLine.size() > 0) {
            readLineVector.push_back(readinLine);
        }
    }

    loopVectorDistrict(readLineVector, vectorStateName, vectorDistrictNumber, vectorDemVote, vectorRepVote);
    
    infs.close();
}

void dataLoaded (ourvector <string> &eligibleState, ourvector <string> &districtState) { //passes in eligible state vector and district state vector to check if they are empty or not if the vectors are not empty then data has been loaded in

    cout << "Data loaded? ";
    if ((eligibleState.size() > 0) && (districtState.size() > 0)) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }
}

void stateLoaded (string &stateName) { //passes in a statename variable as long as the variable itself has a size greater than 1 then the state has been loaded in
    cout << "State: ";
    
    if (stateName.size() > 0) {
        cout << stateName << endl << endl;
    }
    else {
        cout << "N/A" << endl << endl;
    }
    
}

void loadFunction (string &eligibleFile, string &districtFile,ourvector <string> &eligibleState, ourvector <int> &eligibleVoter, ourvector <string> &districtState,
ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, 
ourvector < ourvector<int> > &districtRepNum) { //function to input file names and calls two other functions that fill the district struct and eligible struct with data

    cin >> districtFile >> eligibleFile;
    inputDisctrictFile(districtFile, districtState, districtDistrictNum, districtDemNum, districtRepNum);
    inputEligibleFile(eligibleFile, eligibleState, eligibleVoter, districtState);
}

void breakFunction (int &main) { //function to end program
    main = 0;
}

void loadOutputFunction (string &eligibleFile, string &districtFile, ourvector <string> &eligibleState, ourvector <int> &eligibleVoter,  ourvector <string> &districtState, 
ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, 
ourvector < ourvector<int> > &districtRepNum) { //outputs all the data that was loaded into the vectros, most parameters are pass by vectors that are required in order to sort the data into them.

    if (districtState.size() == 0) { //if district state size is empty then the file name that was passed in was incorrect
        cout << "Invalid first file, try again." << endl << endl;

    }
    else if (eligibleState.size() == 0) { // if eligible state is empty then the second file that was passed in was incorrect still outputs district data
        cout << "Reading: " << districtFile << endl;

        for (int i = 0; i < districtState.size(); i++) {
            cout << "..." << districtState[i] << "..." << districtDistrictNum[i].size() << " districts total" << endl;
        }

        cout << endl;
        cout << "Invalid second file, try again." << endl;
    }
    else { // else both files are good and it outputs the number of districts and eligible voters in each state
        cout << "Reading: " << districtFile << endl;

        for (int i = 0; i < districtState.size(); i++) {
            cout << "..." << districtState[i] << "..." << districtDistrictNum[i].size() << " districts total" << endl;
        }

        cout << endl;
        cout << "Reading: " << eligibleFile << endl;

        for (int i = 0; i < eligibleState.size(); i++) {
            cout << "..." << eligibleState[i] << "..." << eligibleVoter[i] << " eligible voters" << endl;
        }

    }

    
}

void checkStateValidity(string &stateName, ourvector <string> &districtState) { //checks if a state actually exits within the data we have passed into our vectors
    int boolTemp = 0; //temp bool to determine if the state input exits in the district vector

    for (int i = 0; i < districtState.size(); i++) { //loops through each state in district state vector
        string tempState = stateName; //creates a temporary state string for the user input state
        string tempDistrictState = districtState.at(i); //creates a temporary string for the state from the data vector
        transform(tempState.begin(), tempState.end(), tempState.begin(), ::toupper); //converts both temporary strings to uppercase
        transform(tempDistrictState.begin(), tempDistrictState.end(), tempDistrictState.begin(), ::toupper);

        if (tempState == tempDistrictState) { // if the two strings are the same
            stateName = districtState.at(i); //then it passes back the state name from the file data that was inputted
            boolTemp = 1;
        }
    }
    if (boolTemp != 1) {  // if bool Temp is still 0 then the state was not found within the data that was passed in.
    cout << "State does not exist, search again." << endl << endl;
    stateName.clear();
    }
}

void searchFunction (string &stateName, ourvector <string> &districtState) { //search function that inputs the statename input while making sure it doesnt get extra whitespace
    getline(cin, stateName);
    stateName = stateName.substr(1,stateName.size());
    checkStateValidity(stateName, districtState);
        
}

int findStateIndex(string &stateName, ourvector <string> &stateVector) { //returns the index of the state name user input has selected based on where it is loacted in the vector they pass in to the function
    for (int i = 0; i < stateVector.size(); i++) {
        if (stateName == stateVector.at(i)) {
            return i;
        }
    }
    return 0;
}

int checkDistrictNumber (int districtStateIndex, ourvector < ourvector<int> > &districtDistrictNum) { //returns true if a state has more than 3 districts
    if (districtDistrictNum[districtStateIndex].size() >= 3) {
        return 1;
    }
    else if (districtDistrictNum[districtStateIndex].size() < 3){
        return 0;
    }
    return 0;


}

void wastedVotes(int &districtStateIndex, int &demVoteWasted, int &repVoteWasted, int &totalDemVotes, int &totalRepVotes 
,ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, 
ourvector < ourvector<int> > &districtRepNum) { //calculates the number of wasted votes in the user selected state

    for (int i = 0; i < districtDistrictNum[districtStateIndex].size(); i++) { //loops through the data of each district in the user selected state

        int totalVotesTemp = districtDemNum[districtStateIndex].at(i) + districtRepNum[districtStateIndex].at(i); //find total votes casted
        totalDemVotes += districtDemNum[districtStateIndex].at(i);
        totalRepVotes += districtRepNum[districtStateIndex].at(i); //save total amount of replubican and democrate votes casted
        int majority = (totalVotesTemp/2) + 1; //find out what the majority of votes needed is for a district

        if (districtRepNum[districtStateIndex].at(i) > districtDemNum[districtStateIndex].at(i)) { //if republican has more votes calculate the number of democrat votes wasted and the number of republican votes wasted after majority was hit
            demVoteWasted += districtDemNum[districtStateIndex].at(i);
            repVoteWasted += (districtRepNum[districtStateIndex].at(i) - majority);
        }
        else if(districtRepNum[districtStateIndex].at(i) < districtDemNum[districtStateIndex].at(i)){ //if democrat has more votes calculate the number of republican votes wasted and the number of democrat votes wasted after majority was hit
            repVoteWasted += districtRepNum[districtStateIndex].at(i);
            demVoteWasted += (districtDemNum[districtStateIndex].at(i) - majority);
        }
    }

}

int calcGerry (int demVoteWasted, int repVoteWasted, int totalVotes, double &efficiencyNum) { //returns a number depending on if the state is gerrymandered or not

    efficiencyNum = abs((((demVoteWasted*1.0)-(1.0*repVoteWasted))/(totalVotes*1.0))*100); //calculates the efficiency number

    if (efficiencyNum >= 7) { //if efficiency number is greater than 7 state is gerry mandered
        return 1;
    }
    else if (efficiencyNum < 7) {
        return 0;
    }
    return 0;
}

void statFunction (string &stateName, ourvector <string> &eligibleState, ourvector <int> &eligibleVoter,  ourvector <string> &districtState, ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, 
ourvector < ourvector<int> > &districtRepNum) { //function for stat input from user, parameters passed in are the vectors that hold the data from the files

    int districtStateIndex = findStateIndex(stateName, districtState); //finds the index for the user inputted state based on the file for use in the 2d vectors
    int eligibleStateIndex = findStateIndex(stateName, eligibleState);

    int moreThanThree = checkDistrictNumber(districtStateIndex, districtDistrictNum); //check if a district has 3 or more districts

    int demVoteWasted = 0; //total dem votes wasted
    int repVoteWasted = 0; //total rep votes wasted
    int totalVotes = 0; //total number of votes casted
    int totalRepVotes = 0; //total rep votes casted
    int totalDemVotes = 0; //total dem votes casted
    double efficiencyNum = 0.0; //saves efficiency num
    string gerryManderedAgainst = "";

    wastedVotes(districtStateIndex, demVoteWasted, repVoteWasted, totalDemVotes, totalRepVotes, districtDistrictNum, districtDemNum, districtRepNum); 
    
    totalVotes = totalRepVotes + totalDemVotes; 

    int isGerryMandered = calcGerry(demVoteWasted, repVoteWasted, totalVotes, efficiencyNum); 

    if (totalRepVotes > totalDemVotes) { //if state is gerrymandered and who it is gerry mandered against
        gerryManderedAgainst = "Democrats";
    }
    else if  (totalRepVotes < totalDemVotes) {
        gerryManderedAgainst = "Republicans";
    }


    if (moreThanThree == 0) { //data to output if state has less than 3 districts
        cout << "Gerrymandered: No" << endl;
        cout << "Wasted Democratic votes: " << demVoteWasted << endl;
        cout << "Wasted Republican votes: " << repVoteWasted << endl;
        cout << "Eligible voters: " << eligibleVoter[eligibleStateIndex] << endl;
    }
    else {
        if (isGerryMandered) { //data to output if state is gerry mandered
        cout << "Gerrymandered: Yes" << endl;
        cout << "Gerrymandered against: " << gerryManderedAgainst << endl;
        cout << "Efficiency Factor: " << efficiencyNum << "%" << endl;
        cout << "Wasted Democratic votes: " << demVoteWasted << endl;
        cout << "Wasted Republican votes: " << repVoteWasted << endl;
        cout << "Eligible voters: " << eligibleVoter[eligibleStateIndex] << endl;

        }
        else { //data to output if a state is not gerrymandered but has more than 3 distrcts
        cout << "Gerrymandered: No" << endl;
        cout << "Wasted Democratic votes: " << demVoteWasted << endl;
        cout << "Wasted Republican votes: " << repVoteWasted << endl;
        cout << "Eligible voters: " << eligibleVoter[eligibleStateIndex] << endl;
        }
    }

}

void plotOutput (int &districtStateIndex, ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, ourvector < ourvector<int> > &districtRepNum) { //function ouputs and calculates the number of Ds or Rs to output

    for (int i = 0; i < districtDistrictNum[districtStateIndex].size(); i++) { //loop through each district in state
        int demVoteTemp = districtDemNum[districtStateIndex].at(i); // finds the number of dem or republican votes per state per district
        int repVoteTemp = districtRepNum[districtStateIndex].at(i);

        demVoteTemp = floor((((demVoteTemp * 1.0)/((demVoteTemp * 1.0) + (repVoteTemp * 1.0))) * 100)); //finds the number of Ds that need to outputted and rounds to the lowest integer
        repVoteTemp = 100 - demVoteTemp;

        cout << "District: " << districtDistrictNum[districtStateIndex].at(i) << endl; //outputs the number of Ds and Rs based on how many times the loops run and was calculated
        for (int i = 0; i < demVoteTemp; i++) {
            cout << "D";
        }
        for (int i = 0; i < repVoteTemp; i++) {
            cout << "R";
        }
        cout << endl;

    }
    cout << endl;

}

void plotFunction (string &stateName, ourvector <string> &districtState, 
ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, 
ourvector < ourvector<int> > &districtRepNum) { //calls function that outputs the data for the plot


    
    int districtStateIndex = findStateIndex(stateName, districtState); //finds the state index based on the state user inputted so that we are able to loop through the 2d vectors in order to 


    plotOutput(districtStateIndex, districtDistrictNum, districtDemNum, districtRepNum);



}

//int main is used in order to figure out if the exit statement is called which terminates the while loop
//state name is used to pass through the state name into other functions
//other vectors are being passed through for other functions
void command (int &main, string &stateName, ourvector <string> &eligibleState, ourvector <int> &eligibleVoter,  ourvector <string> &districtState, 
ourvector < ourvector<int> > &districtDistrictNum, ourvector < ourvector<int> > &districtDemNum, 
ourvector < ourvector<int> > &districtRepNum) { 

    string input; //user input
    string eligibleFileName; //eligible file input holder
    string districtFileName; //district file input holder
    

    cout << "Enter command: ";

    cin >> input;
    if (input == "load") { //if user wants to load in data
        cout << endl << "-----------------------------" << endl << endl;
        if (districtState.size() > 0) { //if the district state vector has a size greater than 0 then data has already been passed in
            cout << "Already read data in, exit and start over." << endl;
            cout << "Data loaded? Yes" << endl << "State: N/A" << endl << endl;
            cout << "Enter command: ";
            cout << endl << "-----------------------------" << endl << endl;
            breakFunction(main);
        }
        else { //if district state size is less than 0 it passes through the file namne strings and calls the functions that input the vectors with data and output what is being passed in to the vectors.
        loadFunction(eligibleFileName, districtFileName, eligibleState, eligibleVoter, districtState, districtDistrictNum, districtDemNum, districtRepNum);
        loadOutputFunction(eligibleFileName, districtFileName, eligibleState, eligibleVoter, districtState, districtDistrictNum, districtDemNum, districtRepNum);
        }
        cout << endl;
    }
    else if (input == "search") { //if user wants to call the search option in the menu
        cout << endl << "-----------------------------" << endl << endl;
        if (districtState.size() > 0) { //checks if data has been passed in so that we can get the statename
            searchFunction(stateName, districtState);
        }
        else { //if district state size is 0 then no data has been loaded in and must be loaded in before search function can be called.
            cout << "No data loaded, please load data first." << endl << endl;
            cout << "Data loaded? No" << endl << "State: N/A" << endl << endl;
            cout << "Enter command: ";
            cout << endl << "-----------------------------" << endl << endl;
            breakFunction(main);
        }
        

    }
    else if (input == "stats") { //if user input selects stats then output stats information
        cout << endl << "-----------------------------" << endl << endl;
        if (eligibleState.size() < 1) { //if district data size is less than 1 then no data has been loaded in and must be loaded in before calling the stats function
            cout << "No data loaded, please load data first." << endl << endl;
        }
        else if (stateName.size() < 1) { //if the statename size is less than 1 then there we cant display the stats without knowing which state to display information about
            cout << "No state indicated, please search for state first." << endl << endl;
        }
        else { //if both of the if statements dont go through then we are good to call the stat function
            statFunction(stateName, eligibleState, eligibleVoter, districtState, districtDistrictNum, districtDemNum, districtRepNum);
        }

    }

    else if (input == "plot") { // if user wants to call plot function
        cout << endl << "-----------------------------" << endl << endl;
        if (eligibleState.size() < 1) { //checks if state name has not been loaded in yet
            cout << "No data loaded, please load data first." << endl << endl;
        }
        else { //if state name is loaded in then we can call the plot function.
            plotFunction(stateName, districtState, districtDistrictNum, districtDemNum, districtRepNum);
        }
        

    }

    else if (input == "exit") { //exit function to end program
        cout << endl << "-----------------------------" << endl << endl;
        breakFunction(main);
    } 
}

//main variable is parameter that is passed in to check if the exit input is called
//other parameters are vectors being passed in to be manipulated in other functions to get use their data or enter data into them
void outputStatements (int &main, ourvector <string> &eligibleState, ourvector <int> &eligibleVoter,  ourvector <string> &districtState, ourvector < ourvector<int> > &districtDistrictNum,  ourvector < ourvector<int> > &districtDemNum, ourvector < ourvector<int> > &districtRepNum, 
string &stateName) { //function calls three other functions that check is the data is loaded in, checks if the state name is loaded in, and then another function that checks for the user inputs for the menu.
    
    dataLoaded(eligibleState, districtState);
    stateLoaded(stateName);
    command(main, stateName, eligibleState, eligibleVoter, districtState, districtDistrictNum, districtDemNum, districtRepNum);
    

}

int main() { // main function creates the instance of the structs and calls outputstate ments functions.
    int main = 1;

    eligibleStruct eligibleFileData;
    disctrictStruct disctrictFileData;

    string stateName;

    cout << "Welcome to the Gerrymandering App!" << endl << endl;

    while(main) {
        

        outputStatements(main, eligibleFileData.state, eligibleFileData.voters, disctrictFileData.state, disctrictFileData.district, disctrictFileData.demVote, 
    disctrictFileData.repVote, stateName);

        if (!main) { //break statement
            break;
        }

    }

    return 0;
}