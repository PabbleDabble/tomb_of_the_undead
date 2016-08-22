// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
// Signed:Patrick Vielhaber  Date: July 12, 2013

/*
   COPYRIGHT (C) 2013 Patrick Vielhaber. All rights reserved.
       3460:209 Project 2: Code primer for Tomb of the Blind Dead
       Based on original code by Anthony Deeter.
       Author:  Patrick Vielhaber
       Version: 1.01 - August 1, 2013
*/

#include <ctime>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>

/* COMMENTS FOR THE FUTURE / BETTER ERROR CHECKING (Not comprehensive)
* 1) The game menu input string could be better... how to check the m## and s##
* values to make sure... what if there were 100+ room, need to grab 3 digits, etc.
* 2) 
*/



using namespace std;

//Tester boolean
const bool TESTER = true; //Set this to true to be allowed to cheat / view memory
const int PAUSE_TIME = 750; //Set this to 750 to put a nice pause in between entries, 0 to stop pauses

//These are the constants for games set up
const int MIN_ROOMS = 10;
const int MAX_ROOMS = 30;
const int MAX_BULLETS = 6;

//These are the constants for  room array indices
const int NORTH_INDEX = 0;
const int SOUTH_INDEX = 1;
const int WEST_INDEX = 2;
const int EAST_INDEX = 3;
const int PLAYER_INDEX = 4;
const int ZOMBIE_INDEX = 5;
const int GRAIL_INDEX = 6;

//Game data, which room the player, zombie, and grail is in
int current_room, zombie_room, grail_room;
//Game data, how many bullets left, how many rooms there are
int num_bullets, num_rooms;
//Fun extra 
int bullets_wasted = 0; //Start it as zero

bool have_grail;

/*
  Rooms array
  [ north ][ south ][ west ][ east ][ player ][ zombie ][ grail ]
*/
//Added one to this, as I did all the programming 
//Nothing changes, but now no memory will every overwrite
int room_array[MAX_ROOMS + 1][7];

//Function Prototypes
void step_1_reset();
void step_2_instructions();
void step_3_print_memory();
void step_4_display_menu();
void step_5_read_maze();
int step_6_get_random_room(int min_room);
void step_7_place_zombie();
void step_8_place_grail();
void step_9_setup();
bool step_10_check_zombie(int);
bool step_11_check_grail(int);
bool step_12_check_near_grail(int);
bool step_13_check_near_zombie(int);
void step_14_win_or_lose(int);
//I made step 15 an integer so I could output how many rooms were connected
int step_15_show_connected_rooms();
bool step_16_is_connected(int);
void step_17_move_room(int);
void step_18_shoot_room(int);
void step_19_move_zombie();
bool step_20_validate_selection(string);
void step_21_do_selection(string);
void step_22_wait_for_move();
void step_23_check_room();
void step_24_running();
void misc_pause(); //Short pause, can turn on off above (PAUSE_TIME = 0)
void wasting_ammo(); //Makes fun of user 
//In case I wanted to sent the EXE to someone
void hard_coded_maze_file();
void screen_clearer(); //Trying to clean the screen to see better

//Function Prototypes from seeder file
//bool checkGrail(int);
//bool checkZombie(int);
//bool checkNearZombie(int);
//bool checkNearGrail(int);
//void checkRoom();
//void doSelection(string);
//int getRandomRoom();
//void instructions();
//bool isConnected(int);
//void menu();
//void moveRoom(int);
//void moveZombie();
//void placeGrail();
//void placeZombie();
//void printMemory();
//void readMaze();
//void reset();
//void running();
//void setup();
//void shootRoom(int);
//void showConnectedRooms();
//bool validateSelection(string);
//void waitForMove();
//void winOrLose(int);

int main() {
   //Start by cleaning
   screen_clearer();
   
   //Beginning "welcome"
   cout << "      Enter... if you dare...\n"
           << "***THE TOMB OF THE BLIND DEAD***\n";

   //Main menu selection - Error checking seemed easiest on a char
   char user_menu_input = '0';

   //I MOVED STEP 1 INTO MAIN MENU OPTION 2: BEGIN GAME
   //So it would clear the values every time a new game starts
   //And doesn't close the program just because you lose

   //Show the menu to the user - STEP 4   
   do { //Until room == -1
       do { //This will run in an infinite loop, asking the user what do until they exit
           //Show them the menu...
           cout << "-------------------------------------------------------------\n"; //SPACCER
           step_4_display_menu();
           cout << "What would you like to do...?  ";
           //Get a user input
           cin.get(user_menu_input);
           cin.ignore(2000, '\n');
           cout << "-------------------------------------------------------------\n"; //SPACCER

           //Once they put in a valid entry, pause for moment, so it feels better
           misc_pause();

           if (user_menu_input < '1' || user_menu_input > '3')
               cout << "Invalid main menu entry.  Please try again.\n";
           //Keep running until they enter 1, 2, or 3
       } while (user_menu_input < '1' || user_menu_input > '3');

       //Check what they entered, and execute that function
       switch (user_menu_input) {
           case '1':
               //Print instructions - STEP 2
               step_2_instructions();
               break;
           case '2':
               //Begin by resetting all the globals - STEP 1
               //We will do this first to initialize the program from the very start
               cout << "Let the games begin!!\n";
               step_1_reset();

               //Call the initial setup and  begin the game
               step_9_setup();
               break;
           case '3':
               //Clear the variables (STEP 1) and exit the program
               step_1_reset();
               cout << "Thanks for playing!!\n";
               misc_pause();
               return 0;
               break;
           default:
               //This should never happen
               cout << "MAIN MENU - DEFAULT CASE ERROR";
       }
       //-1 is sentinel value for current_room to exit program
   } while (current_room != -1);

   return 0;
}

/*****************************************************
* This function resets all the global variables     *
*****************************************************/
void step_1_reset() {
   //Reset the program (all the globals)
   current_room = 0;
   zombie_room = 0;
   grail_room = 0;
   num_bullets = 0;
   num_rooms = 0;
   have_grail = 0;
   bullets_wasted = 0;

   int i, j; //Counter variables
   //First run through each room
   for (i = 1; i <= MAX_ROOMS; i++) {
       //Then run through each room index
       for (j = 0; j <= 6; j++) {
           room_array[i][j] = 0;
       }
   }
}

/*********************************************
* This function prints the instructions     *
*********************************************/
void step_2_instructions() {
   cout << "Your mission, should you choose to accept, is to seek\n"
           << "The Holy Grail within the ruins of an ancient church.\n"
           << "To succeed you must...\n"
           << "\t-Enter the church\n"
           << "\t-Avoid the undead guardian\n"
           << "\t-Find the grail\n"
           << "\t-Then escape the church! (Exit through room 1)\n"
           << "You have " << MAX_BULLETS << " silver bullets to protect yourself.\n"
           << "If the zombie finds you... you will lose!\n\n"
           << "*Hint: The zombie will randomly wander the church\n"
           << "And will sometimes sneak up behind you.\n"
           << "Even if you don't hear it near you, it is sometimes adventageous\n"
           << "To shoot warning shots into nearby rooms to attract it... Good luck!\n";
}

/**************************************************************
* This function prints the memory  room array (debugger)     *
**************************************************************/
void step_3_print_memory() {

   int i, j; //Counter variables
   //To differentiate between memory print and check room verbiage
   cout << "**PRINT MEMORY BEGIN**\n";

   //Print out the global variables
   cout << "You are in room: [" << current_room << "]\n";
   cout << "You have [" << num_bullets << "] ";
   if (num_bullets == 1)
       cout << "bullet left.\n";
   else
       cout << "bullets left.\n";

   cout << "There are [" << num_rooms << "] rooms in this church.\n";
   if (have_grail) cout << "You currently have the Holy Grail.\n";
   else cout << "You do not currently have the Holy Grail.\n\n";

   //Print the header of the matrix
   cout << "         " << "[NOR][SOU][EA ][WE ][US ][ZOM][GRA]\n"
           << "         " << "[TH ][TH ][ST ][ST ][ER ][BIE][IL ]\n";

   //Print out the room arrray, all 7 indices
   //First run through each room
   for (i = 1; i <= num_rooms; i++) {
       //Print what each room is on the left
       cout << "<ROOM:" << setw(2) << i << ">";
       //Then run through each room index
       for (j = 0; j <= 6; j++) {
           cout << "[" << setw(3) << room_array[i][j] << "]";
       }
       cout << endl;
   }
}

/***************************************************
* This function displays the menu to the user     *
***************************************************/
void step_4_display_menu() {
   cout << "     Main Menu\n";
   cout << "1: View instructions\n";
   cout << "2: Begin a new game\n";
   cout << "3: Quit the program\n";
}

/*************************************************
* This function reads the maze text file        *
* Then stores it in the maze_array variable     *
*************************************************/
void step_5_read_maze() {
   ifstream input_file;
   int i, j; //Counter variables

   //First, open the file, get the number of lines, 
   //Then store all the data into the room array
   string filename = "easy_maze.txt";

   input_file.open(filename.c_str());
   if (input_file) {
       //The first line of the file is how many lines of data to get
       input_file >> num_rooms;
       //Careful, memory error
       if (num_rooms > MAX_ROOMS)
           cout << "Warning: Potentially more rooms than <room_array> allows.\n";

       //cout << num_rooms << endl;

       for (i = 1; i <= num_rooms; i++) {
           for (j = 0; j <= 3; j++) {
               //Put the maze data in the room array
               input_file >> room_array[i][j];
           }
       }
       input_file.close();
   } else {
       cout << "No Maze File Found -- Using hard coded maze :-)\n";
       hard_coded_maze_file();
   }
}

/************************************************
* This function will get a random room         *
* We will pass in the min max                  *
* And get a random room between them           *
* num_rooms is global, so only need min_room   *
************************************************/
int step_6_get_random_room(int min_room) {
   int rand_room;
   rand_room = (rand() % (num_rooms - min_room)) + min_room;
   return rand_room;
}

/************************
* Place the Zombie     *
************************/
void step_7_place_zombie() {
   //Only put zombie in top half highest rooms
   //The smallest value of room randomizer
   int min_room = (num_rooms / 2) + 1;
   zombie_room = step_6_get_random_room(min_room);
   room_array[zombie_room][ZOMBIE_INDEX] = 1;
}

/***********************
* Place the Grail     *
***********************/
void step_8_place_grail() {
   //Only put the grail in top half highest rooms
   //The smallest value of room randomizer
   int min_room = (num_rooms / 2) + 1;
   grail_room = step_6_get_random_room(min_room);
   room_array[grail_room][GRAIL_INDEX] = 1;
}

/*****************************************************************
* This function does the initialization for the entire game     *
*****************************************************************/
void step_9_setup() {
   //Initialize the random variable - this might not be good for 
   //Playing more than one game per program initialization
   srand(time(0));
   //Put the user in the starting room
   current_room = 1;
   room_array[current_room][PLAYER_INDEX] = 1;
   //Initialize the have_grail to false
   have_grail = false;
   //Reload the users gun
   num_bullets = MAX_BULLETS;
   //To start, we need to read the maze file to get the layout of the game
   step_5_read_maze();
   //Place the zombie - get random room and put the zombie there
   step_7_place_zombie();
   //Place the grail - get random room and put the grail there
   step_8_place_grail();

   //This is my testing hub
   //I'd like to keep this here for future use / bigger maps
   //JEG - PTV CRTL-F keyword
   if (1 == 1) {
       //Keep this so it only resets stuff once
       if (current_room != -1) {
           current_room = 1;
           have_grail = false;
           room_array[zombie_room][ZOMBIE_INDEX] = 1;
           zombie_room = zombie_room;
           //zombie_room = 0;
       }
   }else have_grail = false;

   //FINALLY Now call the actual running program
   step_24_running();
}

/***********************************************
* Check if the zombie is in specified room    *
***********************************************/
bool step_10_check_zombie(int room) {
   if (room_array[room][ZOMBIE_INDEX] == 1)
       return 1;
   else
       return 0;
}

/***********************************************
* Check if the grail is in specified room     *
***********************************************/
bool step_11_check_grail(int room) {
   if (room_array[room][GRAIL_INDEX] == 1)
       return 1;
   else
       return 0;
}

/***********************************************
* Check if grail is in a nearby room          *
***********************************************/
bool step_12_check_near_grail(int room) {
   //Check N,S,W,E indeces connected to the check room
   //Then see if the GRAIL_INDEX = 1
   //Use i: 0 to 3 to check the four cardinal directions
   int connected_room;
   for (int i = 0; i <= 3; i++) {
       //If there is a room connected, check the GRAIL_INDEX
       connected_room = room_array[room][i];
       if (connected_room != 0) {
           if (room_array[connected_room] [GRAIL_INDEX] == 1)
               return true;
       }
   }
   return false;
}

/***********************************************
* Check if zombie is in adjacent room         *
***********************************************/
bool step_13_check_near_zombie(int room) {
   //Check N,S,W,E indeces connected to the check room
   //Then see if the ZOMBIE_INDEX = 1
   //Use i: 0 to 3 to check the four cardinal directions
   int connected_room;
   for (int i = 0; i <= 3; i++) {
       //If there is a room connected, check the ZOMBIE_INDEX
       connected_room = room_array[room][i];
       if (connected_room != 0) {
           if (room_array[connected_room] [ZOMBIE_INDEX] == 1)
               return true;
       }
   }
}

/****************************************
* Outputs the win  lose text           *
* Based on winning  losing boolean     *
****************************************/
void step_14_win_or_lose(int win_lose) {
   //win_lose == 4, they killed the zombie, 
   //but kept screwing around firing their gun
   if (win_lose == 4) {
       cout << "You started off a hero, vanquishing evil\n"
               << "From the church like a skilled marksmen with a purpose!!\n"
               << "Then, even though no threat remained,\n"
               << "You ran around firing priceless magical silver bullets randomly,\n"
               << "Like a crazy person.\n\n"
               << "Now, you lay comatose, as a ricochet bullet caromed off a wall\n"
               << "And lodged itself deep in your brain stem.\n"
               << "Nice shootin' Tex!\n"
               << "                   GAME OVER\n\n";
   }//win_lose == 3, they shot in the same room and 
       //died of a ricochet themselves in the same room
   else if (win_lose == 3)
       cout << "You carefully and brilliantly take aim at\n"
           << "A nearby wall in the same room.\n"
           << "A shot rings out, the bullet ricochets magnificently\n"
           << "Before hitting you in the leg.\n"
           << "You fall to the ground in agony, lantern breaking upon the stone floor.\n"
           << "As darkness overcomes you, you hear the slow, shuffling, sound\n"
           << "Of your imminent death approaching from all sides.\n\n"
           << "Nice shootin' Tex!\n"
           << "                   GAME OVER\n\n";
       //win_lose == 2: YOU LOSE - ZOMBIE SHUFFLES IN ON YOU
       //This will possibly happen after a move to a safe room
       //Or after the user shot and missed
   else if (win_lose == 2) {
       cout << "You hear shuffling from an adjoining room.\n"
               << "Just as you turn to look, the zombie is upon you and devours you whole.\n\n"
               << "YOU LOSE!! GOOD DAY SIR!!\n";
   }//win_lose = 1: YOU LOSE - YOU WALKED IN ON THE ZOMBIE
       //This will only happen when the person walks in the zombie
   else if (win_lose == 1) {
       cout << "As you walk into the room, the zombie immediately\n"
               << "senses your presence and devours you whole.\n\n"
               << "YOU LOSE!! GOOD DAY SIR!!\n";
   }//win_lose = 0: YOU WIN
   else if (win_lose == 0) {
       cout << "You emerge from the ruins of the church\n"
               << "Safe from harm, and in possession of The Holy Grail!!\n"
               << "You have won the game!!\n";
       if (num_bullets == MAX_BULLETS)
           cout << "              **FLAWLESS VICTORY!!**\n"
               << "You are the silent thief... No ammunition spent!!\n";
       else
           cout << "You used " << MAX_BULLETS - num_bullets << " bullet" << (num_bullets != 5 ? "s" : "") << ".\n" 
                   << "Now try without using your gun!!\n";
   }
   //Set the current room out of scope to so the program will end 
   current_room = -1;
}

/*********************************************************************
* This function will display any valid rooms that are connected     *
*********************************************************************/
int step_15_show_connected_rooms() {
   //So we can count how many room options we have
   int room_count = 0;

   //Check N,S,W,E indeces connected to the check room
   //For any rooms != 0 (if they exist) display them to the user
   //Use i: 0 to 3 to check the four cardinal directions
   //I wanted to display the rooms in ascending order regardless
   int display_compare = MAX_ROOMS + 1; //This needs to be bigger than any possible room
   int last_display = 0; //Initialize to 0, this will ignore walls

   //Initialize the display order, make it higher than the max
   int i, j; //Counter variable

   //Run though all the connected rooms showing the lower connect room first
   //Check each of the four spots (outer loop) and find the smallest value (inner loop)
   for (int i = 0; i <= 3; i++) {
       //Ignore walls
       if (room_array[current_room][i] != 0) {
           //Then run through all the displays for each spot
           for (int j = 0; j <= 3; j++) {
               //Compare the inner loop room against the compare variable
               //But make sure it's also bigger than the last room we displayed
               //Otherwise, we'll just display the smallest room four times
               if (room_array[current_room][j] <= display_compare &&
                       room_array[current_room][j] > last_display) {
                   display_compare = room_array[current_room][j];
               }
           } //Inside for loop: j

           //PRINT THE LOWEST 
           cout << " [ " << setw(2) << display_compare << " ] ";
           //Cunt that room 
           room_count++;
           //Store the printed value as the last display for the next loop
           last_display = display_compare;
           //Restore the compare variable for the next run
           display_compare = MAX_ROOMS + 1;
       } //Ignore walls
   } //Outside for loop: i
   return room_count;
}

/**************************************************
* This function will show if parameter value     *
* room is connected to current room              *
* for testing shooting / walking                 *
**************************************************/
bool step_16_is_connected(int room) {
   //Run through the connected rooms
   //If any of current_room connected rooms == room 
   //Return true 
   for (int i = 0; i <= 3; i++) {
       if (room_array[current_room][i] == room) return true;
   }
   return false;
}

/****************************************
* This function will move the user     *
****************************************/
void step_17_move_room(int requested_room) {

   //Test is the room is connected
   if (step_16_is_connected(requested_room)) {
       //Remove the user from the current room
       room_array[current_room][PLAYER_INDEX] = 0;
       //Put the user in the new room
       room_array[requested_room][PLAYER_INDEX] = 1;
       //Update the current room variable
       current_room = requested_room;

       //Check to see if the zombie is in there first before anything
       if (step_10_check_zombie(current_room) == true) {
           //They walked in on the zombie
           //They lose, argument (1) for walking in on the zombie
           step_14_win_or_lose(1);
       } else //No zombie
       {
           //Check to see if the user has the grail
           //And escaped 
           if (have_grail == true && current_room == 1) {
               //If they had the grail AND are in room 1, they win
               step_14_win_or_lose(0);
           }//If they didn't have the grail but it's in the room with them now
           else if (have_grail == false && step_11_check_grail(current_room) == true) {
               //Give them the grail, and then clear out the grail data
               have_grail = true;
               room_array[grail_room][GRAIL_INDEX] = 0;
               grail_room = 0;
               cout << "As you look around the room, you find\n"
                       << "The Holy Grail, gleaming in all it's glory!!\n\n"
                       << "Now that you have retrieved the grail, you must make it out alive!\n";
           } else {
               //The zombie wasn't in the room, and we checked for the grail
               cout << "You " << ((zombie_room == 0) ? "casually" : "bravely") << " move forward and enter room number " << requested_room << endl;
               //Let the zombie try to wander around then
               step_19_move_zombie();
           }
       } //Zombie wasn't in the room
   } else //Room not connected
       cout << "With great confidence, you walk directly into a wall. \n"
           << "Ouch.\n\n"
           << "That room is not connected to your current room.\n"
           << "Please try again...\n";
}

/***************************************************
* This function sees if the room is connected     *
* Then shoots into that room                      *
* If the zombie is in that room,                  *
* The zombie is removed from the game             *
***************************************************/
void step_18_shoot_room(int shoot_room) {
   //First check if they have any bullets
   if (num_bullets == 0) {
       //NO BULLETS!!!
       cout << "Click... No bullets...\n";
       //If the zombie is dead, make fun of them
       if (zombie_room == 0)
           cout << "The zombie is dead... You just like pulling the trigger, don't you?\n";
   } else if (num_bullets != 0) {
       //They have fired a bullet, unless the room doesn't exist,
       //And then we'll be nice and give it back to them
       num_bullets--;

       //First check if they're outside shooting outside 
       if (shoot_room == 1 && current_room == 1) {
           //Why are they shooting while standing outside?
           cout << "You are standing outside the church and shoot your gun\n"
                   << "Into the air like a cowboy. Congratulations.  Bullet well spent.\n";
           //THE ZOMBIE WANDERS -- The zombie is still alive, it shall possibly wander
           step_19_move_zombie();
       } else if (shoot_room == 1 && current_room != 1 && step_16_is_connected(shoot_room)) {
           //They are standing at the entrance to the church, shooting outside
           cout << "Hey Maverick... there's no zombies outside...\n";
           //THE ZOMBIE WANDERS -- The zombie is still alive, it shall possibly wander
           step_19_move_zombie();
       }//Then check if they tried to shoot into the same room
       else if (shoot_room == current_room) {
           //They shot into the same room... they deserve to die
           step_14_win_or_lose(3);
       }//The entryway is no way connected to their shot,
           //So check and see if the room they're trying to shoot at is connected 
       else if (step_16_is_connected(shoot_room)) {
           //Check if the zombie is alive
           if (zombie_room != 0) {
               //Check if the zombie is in there
               if (step_10_check_zombie(shoot_room)) {
                   //You shot the zombie!
                   cout << "BULLSEYS!! Right between the eyes!\n"
                           << "The zombie has been slain!\n";
                   room_array[zombie_room][ZOMBIE_INDEX] = 0;
                   zombie_room = 0;
               } else {
                   //They missed...
                   cout << "You hear the bullet hit the wall in room " << shoot_room << ".\n"
                           << "The zombie still roams free...\n";
                   //THE ZOMBIE WANDERS -- The zombie is still alive, it shall possibly wander
                   step_19_move_zombie();
               }
           } else if (zombie_room == 0) {
               //If the zombie is dead, make fun of them for wasting ammo
               wasting_ammo();
           }
       } else {
           cout << "You cannot shoot in that direction.\n";
           //Didn't seem fair when they can't actually shoot that way
           num_bullets++;
       }
   }
}

/**********************************************
* This function picks a random direction     *
* If there is a room in that direction       *
* Move the zombie into that room             *
**********************************************/
void step_19_move_zombie() {
   int zombie_wander;
   //Check if the zombie is still alive
   if (zombie_room != 0) {
       //Pick a random direction 0=N,1=S,2=W,3=E
       zombie_wander = rand() % 4;
       //cout << "Direction " <<zombie_wander << endl;   //TESTER

       //Check if there is a connected room to the zombie
       //In the random direction
       if (room_array[zombie_room][zombie_wander] != 0) {
           //There is an available room, move the zombie
           //Clear the old room
           room_array[zombie_room][ZOMBIE_INDEX] = 0;
           //Grab the new room
           zombie_room = room_array[zombie_room][zombie_wander];
           //Put the zombie in the new room
           room_array[zombie_room][ZOMBIE_INDEX] = 1;
       } else {
           //Do nothing, the zombie just walked into a wall
           //Would be funny to let the user hear a 'distant thud'
       }
   }
}

/***************************************************
* This function checks to make sure               *
* The user input is Q, D, S#, M#                  *
* True if it's a valid input, and false if not     *
***************************************************/
bool step_20_validate_selection(string user_input) {
   //Grabs the first string as a char, so we can compare
   char user_action = user_input[0];
   char user_action_room1; //This will grab the 

   switch (user_action) {
       case 'Q':
       case 'q':
           return true;
           break;
       case 'M':
       case 'm':
           //Check and make sure the second 'char' of their string
           //can actually be compared as an integer
           user_action_room1 = user_input[1];
           if (user_action_room1 >= '1' && user_action_room1 <= '9') {
               return true;
           }
           break;
       case 'S':
       case 's':
           //Check and make sure the second 'char' of their string
           //can actually be compared as an integer
           user_action_room1 = user_input[1];
           if (user_action_room1 >= '1' && user_action_room1 <= '9') {
               return true;
           }
           break;
       case 'D':
       case 'd':
           if (TESTER) {
               return true;
           }
   }

   //All the breaks come to here
   //Invalid entry.
   cout << "Invalid game menu string.  Please try again!\n";
   cout << "-------------------------------------------------------------\n"; //SPACER
   step_23_check_room();
   return false;
}

/************************************************************
* This function will call the viable functions             *
* Based off of what the user inputs at the game screen     *
* Q - quit                                                 *
* D - prints memory                                        *
* S# - Shoot to room #                                     *
* M# - Move to room #                                      *
************************************************************/
void step_21_do_selection(string user_input) {
   //If they want to move or shoot, check to make sure 
   int shoot_move_room;
   //Grabs the first string as a char, so we can compare
   char user_action = user_input[0];

   //Decide what to do
   switch (user_action) {
       case 'Q':
       case 'q':
           //Quit the game
           cout << "Thanks for playing!!\n\n";
           current_room = -1;
           break;
       case 'D':
       case 'd':
           if (TESTER) {
               //Print memory
               step_3_print_memory();
           }
           break;
       case 'S':
       case 's':
           //Get the room they want to shoot or move to, convert to integer from input string
           //This is the only error chance I can really see...
           //If someone enters a game menu option starting with an S or M
           //And then 
           shoot_move_room = atoi(user_input.substr(1, 2).c_str());

           //Shoot into said room (shoot room will do the appropriate tests)
           step_18_shoot_room(shoot_move_room);

           break;
       case 'M':
       case 'm':
           //Get the room they want to shoot or move to
           shoot_move_room = atoi(user_input.substr(1, 2).c_str());
           //Move to that room, move_room will do appropriate checks
           step_17_move_room(shoot_move_room);
           break;
       default:
           cout << "GAME INPUT FAIL - SNUCK BY VALIDATION\n\n";
   }
}

/***********************************************************
* This function shows the command menu to the user        *
* Then waits for an input, then validates said input      *
* Once valid input is given, it executes that command     *
***********************************************************/
void step_22_wait_for_move() {
   string user_input = "XX"; //Their response

   do {
       //Show a command window and ask for what option they want to do
       cout << "-------------------------------------------------------------\n"; //SPACER
       cout << "Please enter one of the following choices:\n";
       cout << "[Q]  Quit the current game\n";
       if (TESTER) //If we're in tester mode, let them peek
           cout << "[D]  Print the current memory\n";
       //If they have any bullets left
       if (num_bullets != 0)
           cout << "[S#] Shoot into room #  (# being an integer value of the room)\n";
       cout << "[M#] Move into room #   (# being an integer value of the room)\n\n";

       cout << "User entered: ";
       cin >> user_input;
       cin.ignore(20000, '\n');

       //This will separate the previous options and their entry...
       //From what comes next.
       cout << "-------------------------------------------------------------\n"; //SPACER

       //Once they put in a valid entry, pause for moment, so it feels better
       misc_pause();


   } while (step_20_validate_selection(user_input) == false);

   //Once they have a valid entry, do that entry
   step_21_do_selection(user_input);
}

/***************************************************************
* This is the biggie                                          *
* This function will:                                         *
* Display room bullets                                        *
* Say if they have the grail                                  *
* If the zombie is in the room, they die (win_or_lose(2))     *
* Zombie near by - hear it                                    *
* Grail near by - sense it                                    *
* Shows the rooms connected to current room                   *
***************************************************************/
void step_23_check_room() {
   //How many rooms connected
   int rooms_connected = 0;

   //Zombie test first.  If he wandered in on you no reason to output anything  else
   if (zombie_room != 0) {
       //Check to see if zombie / user in same room, if so: DEAD
       if (step_10_check_zombie(current_room) == true)
           //The zombie wandered in on them after the last move / shot
           step_14_win_or_lose(2);
       else if (step_13_check_near_zombie(current_room))
           cout << "Your heart beats faster...\n"
               << "There is a distinct shuffling in a nearby room...\n"
               << "The zombie is close...\n\n";
   }

   //Only display the rest of the information if they are still alive
   if (current_room != -1) {

       //This will give them current game data 
       cout << "You are currently in room: " << current_room << endl;

       //If zombie is dead, who cares how many bullets are left
       if (zombie_room != 0) {
           if (num_bullets > 1)
               cout << "You have " << num_bullets << " bullets left.\n";
           else if (num_bullets == 1)
               cout << "You have " << num_bullets << " bullet left.\n";
           else if (num_bullets == 0)
               cout << "You have no bullets left.\n";
       } else
           cout << "Smoke still billows from the barrel of your gun\n"
               << "\tAfter that righteous headshot. The zombie is no more!\n";

       //Have grail?
       if (have_grail)
           cout << "You are currently in possession of the grail!\n\n";
       else if (step_12_check_near_grail(current_room))
           cout << "Divine power flows through you... the Holy Grail is near!\n\n";

       //Tell the user which rooms are currently connected
       rooms_connected = step_15_show_connected_rooms();
       if (rooms_connected == 1)
           cout << " <-- There is only one room connected to your current room.\n";
       else if (rooms_connected > 1)
           cout << " <-- There is more than one room connected to your current room.\n";
       else if (rooms_connected == 0) {
           //This should never happen...
           cout << "There is a problem with the maze file. No connected rooms.\n\n";
       }
       //I wanted to be able to call check_room from other functions
       //Without it necessarily calling waiting_for_move
       //So I took waiting_for_move out of here and put it in the step_24_running loop
   }
}

/******************************************************
* This function will hold the gameplay functions     *
* To check the room and then to ask for a move       *
******************************************************/
void step_24_running() {
   //Only cut out when room == -1
   do {
       cout << "-------------------------------------------------------------\n"; //SPACER
       step_23_check_room();
       //Once the user has all relevant information
       //Bring up the menu and wait for a valid entry
       step_22_wait_for_move();
   } while (current_room != -1);


   //This will let me just restart again without having to rebuild EVERY time
   //If I want to play another time without quitting completely
   current_room = 0;
   cout << "**RESTARTING PROGRAM**\n";

}

/*********************************************************
* This program will pause for the global const          *
* variable in MS, 750 and 0 are two current options     *
*********************************************************/
void misc_pause() {
   double start_time;
   double wait_time = PAUSE_TIME;
   start_time = clock();
   do {
   } while (clock() < start_time + wait_time);
   
   //Try and clear the screen then too?
   screen_clearer();
}

/********************************************************
* I was having way too much fun                        *
* Making fun of the user for shooting in               *
* Ridiculous manners, I had to make a new function     *
* To make it simpler                                   *
********************************************************/
void wasting_ammo() {
   //We only get here if the zombie is alive, so let's ridicule them
   bullets_wasted++; //Wasted a bullet

   //Be nice(r) if it's their first one
   if (bullets_wasted == 1) {
       //First one, just remind them not to be stupid with guns
       cout << "BANG!\n";
       misc_pause();
       cout << " ...\n";
       misc_pause();
       cout << "  ...\n";
       misc_pause();
       cout << "   ...Why are you shooting up an empty church?\n"
               << "The zombie is dead, you're just wasting ammunition.\n"
               << "Silver bullets aren't cheap... chill out\n";
   }//If they waste 2 bullets or 3 bullets warn them about ricochet    
   else if (bullets_wasted < 4) {
       //This isn't the first time they've shot at nothing
       cout << "Bang... You shoot your gun unimpressively into the emptiness.\n"
               << "The zombie is dead, and yet you're still shooting at stuff...\n"
               << "Better be careful... those bullets could ricochet...\n";
   }//The zombie is dead but they have fired the gun randomly 4 times... death
   else if (bullets_wasted >= 4)
       //Death by ricochet
       step_14_win_or_lose(4);
}

/********************************************
* I wanted a way to hard_code the maze     *
* if the *.txt file wasn't available       *
* Messy, trying to clean up                *
********************************************/
void hard_coded_maze_file() {
   num_rooms = 11;
   int temp_room_array[][7] = {
       {2, 0, 0, 0},
       {0, 1, 4, 5},
       {9, 0, 0, 4},
       {0, 0, 3, 2},
       {7, 0, 2, 9},
       {10, 0, 0, 7},
       {0, 5, 6, 8},
       {0, 0, 7, 0},
       {0, 3, 5, 0},
       {0, 6, 0, 11},
       {0, 0, 10, 0}
   };

   for (int i = 0; i <= num_rooms; i++) {
       for (int j = 0; j <= 3; j++) {
           room_array[i + 1][j] = temp_room_array[i][j];
       }
   }
}

/*****************************************
* Trying to make something to clean     *
* The screen between user inputs        *
*****************************************/
void screen_clearer() {
   for (int i = 0; i < 50; i++) 
       cout << endl;
}