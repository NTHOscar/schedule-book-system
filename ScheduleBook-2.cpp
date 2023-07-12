#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <Windows.h>
using namespace std;

class Event {
public:
	int id;
	char eventTitle[41];
	int eventDate[3];
	int eventStartTime[2];
	int eventEndTime[2];
	char eventType;
	string eventTypeName;

	void PrintAllContent() {
		cout << setfill('0');
		cout << setw(2) << eventDate[0] << '-' << setw(2) << eventDate[1] << '-' << eventDate[2] << '/';
		cout << setw(2) << eventStartTime[0] << ':' << setw(2) << eventStartTime[1] << '-';
		cout << setw(2) << eventEndTime[0] << ':' << setw(2) << eventEndTime[1] << '/';
		cout << eventTitle << '/';
		cout << eventTypeName << endl;
	}
};

Event eventList[100];
int currentIndex = 0;
int currentTry = 1;
int crashIndex = 0;
Event crashList[100];

bool CheckCharInt(string x);			///function prototype 
bool CheckDate(string eventDate);
bool CheckTime(string eventStartTime, string eventEndTime);
bool CheckType(char num);
void DeleteEvent(int id);
void O4();

bool CheckDate(string eventDate) {
	int pos1 = eventDate.find_first_of('-'); //Find location of '-'
	int pos2 = eventDate.find_last_of('-');
	int slength = eventDate.length();

	string sdate = eventDate.substr(0, pos1); //Check if char is int
	if (!CheckCharInt(sdate) || sdate.length() != 2) {
		cout << "\n" << "*** Incorrect date format.";
		return false;
	}
	string smonth = eventDate.substr(pos1 + 1, pos2 - pos1 - 1);
	if (!CheckCharInt(smonth) || smonth.length() != 2) {
		cout << "\n" << "*** Incorrect month format.";
		return false;
	}
	string syear = eventDate.substr(pos2 + 1, slength - pos2);
	if (!CheckCharInt(syear) || syear.length() != 4) {
		cout << "\n" << "*** Incorrect year format.";
		return false;
	}

	int date = stoi(sdate); //Convert to int
	int month = stoi(smonth);
	int year = stoi(syear);
	bool leap = false;

	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) { //Check Leap Year
		leap = true;
	}
	if (!(year >= 1000 && year <= 9999)) { //Check Year
		cout << "\n" << "*** Year out of range.";
		return false;
	}

	if (!(month >= 1 && month <= 12)) { //Check Month
		cout << "\n" << "*** Month out of range.";
		return false;
	}

	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) { //Check Date
		if (!(date >= 1 && date <= 31)) {
			cout << "\n" << "*** Date out of range.";
			return false;
		}
	}
	else if (month == 2) {
		if (leap) {
			if (!(date >= 1 && date <= 29)) {
				cout << "\n" << "*** Date out of range.";
				return false;
			}
		}
		else {
			if (!(date >= 1 && date <= 28)) {
				cout << "\n" << "*** Date out of range.";
				return false;
			}
		}
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (!(date >= 1 && date <= 30)) {
			cout << "\n" << "*** Date out of range.";
			return false;
		}
	}
	else {
		cout << "*** Date out of range.";
		return false;
	}
	return true;
}

bool CheckCharInt(string x) {
	for (int i = 0; i < x.length(); i++)
	{
		if (!(x[i] >= '0' && x[i] <= '9')) {
			return false;
		}
	}
	return true;
}

bool CheckType(string num) {
	if ((!CheckCharInt(num) || num.length() > 1)) {
		cout << "\n" << "*** Incorrect Type format.";
		return false;
	}
	if (!(stoi(num) >= 1 && stoi(num) <= 4)) {
		cout << "\n" << "*** Value out of range.";
		return false;
	}
	return true;
}

bool CheckTime(string eventStartTime, string eventEndTime) {
	int pos1 = eventStartTime.find_first_of(':');
	int pos2 = eventEndTime.find_first_of(':');
	string sshours = eventStartTime.substr(0, pos1);
	if (!CheckCharInt(sshours) || sshours.length() != 2) {
		cout << "\n" << "*** Incorrect Time format.";
		return false;
	}
	string ssmin = eventStartTime.substr(pos1 + 1, eventStartTime.length() - pos1 - 1);
	if (!CheckCharInt(ssmin) || ssmin.length() != 2) {
		cout << "\n" << "*** Incorrect Time format.";
		return false;
	}
	string sehours = eventEndTime.substr(0, pos2);
	if (!CheckCharInt(sehours) || sehours.length() != 2) {
		cout << "\n" << "*** Incorrect Time format.";
		return false;
	}
	string semin = eventEndTime.substr(pos2 + 1, eventEndTime.length() - pos2 - 1);
	if (!CheckCharInt(semin) || semin.length() != 2) {
		cout << "\n" << "*** Incorrect Time format.";
		return false;
	}
	int shours = stoi(sshours);
	int smin = stoi(ssmin);
	int ehours = stoi(sehours);
	int emin = stoi(semin);

	if (!(shours >= 0 && shours <= 23)) {
		cout << "\n" << "*** Time of out range.";
		return false;
	}
	if (!(ehours >= 0 && ehours <= 23)) {
		cout << "\n" << "*** Time of out range.";
		return false;
	}
	if (!(smin >= 0 && smin <= 59)) {
		cout << "\n" << "*** Time of out range.";
		return false;
	}
	if (!(emin >= 0 && emin <= 59)) {
		cout << "\n" << "*** Time of out range.";
		return false;
	}
	if (!(shours <= ehours)) {
		cout << "\n" << "*** End time earlier than start time.";
		return false;
	}
	if (shours == ehours && !(smin < emin)) {
		cout << "\n" << "*** End time earlier than start time.";
		return false;
	}
	return true;
}

void ConvertDate(Event& newEvent, string date) {		//assign date to new added event
	int pos1 = date.find_first_of('-'); //Find location of '-'
	int pos2 = date.find_last_of('-');
	int slength = date.length();

	string sdate = date.substr(0, pos1);
	string smonth = date.substr(pos1 + 1, pos2 - pos1 - 1);
	string syear = date.substr(pos2 + 1, slength - pos2 - 1);

	newEvent.eventDate[0] = stoi(sdate); //Convert to int
	newEvent.eventDate[1] = stoi(smonth);
	newEvent.eventDate[2] = stoi(syear);
}

void ConvertTime(Event& newEvent, string sTime, string eTime) { //assign start time and end time to new added event
	int pos1 = sTime.find_first_of(':');
	int pos2 = eTime.find_first_of(':');

	string sshours = sTime.substr(0, pos1);
	string ssmin = sTime.substr(pos1 + 1, sTime.length() - pos1 - 1);
	string sehours = eTime.substr(0, pos2);
	string semin = eTime.substr(pos2 + 1, eTime.length() - pos2 - 1);

	newEvent.eventStartTime[0] = stoi(sshours);
	newEvent.eventStartTime[1] = stoi(ssmin);
	newEvent.eventEndTime[0] = stoi(sehours);
	newEvent.eventEndTime[1] = stoi(semin);

}

void ConvertType(Event& newEvent, char Type) { //assign type name to new added event by corresponding type number
	switch (Type) 
	{
	case '1':
		newEvent.eventTypeName = "Home";
		break;
	case '2':
		newEvent.eventTypeName = "Work";
		break;
	case '3':
		newEvent.eventTypeName = "Study";
		break;
	case '4':
		newEvent.eventTypeName = "Play";
		break;
	}
}

int CheckTimeAva(Event newEvent) {
	crashIndex = 0;
	for (int i = 0; i < currentIndex; i++)
	{
		if ((eventList[i].eventDate[0] == newEvent.eventDate[0] && eventList[i].eventDate[1] == newEvent.eventDate[1] && eventList[i].eventDate[2] == newEvent.eventDate[2])) {  //Check If same date
			if (newEvent.eventStartTime[0] < eventList[i].eventStartTime[0] || newEvent.eventStartTime[0] == eventList[i].eventStartTime[0] && newEvent.eventStartTime[1] < eventList[i].eventStartTime[1] || newEvent.eventStartTime[0] == eventList[i].eventStartTime[0] && newEvent.eventStartTime[1] == eventList[i].eventStartTime[1]) { // if new event start time earlier than or equals to original event start time 
				if (newEvent.eventEndTime[0] > eventList[i].eventStartTime[0] || newEvent.eventEndTime[0] == eventList[i].eventStartTime[0] && newEvent.eventEndTime[1] > eventList[i].eventStartTime[1]) { // if new event end time is later than original event start time
					crashList[crashIndex] = eventList[i];
					crashIndex++;
				}
			}			//new event start time hour is within original event start time and end time												if new event start time is equals to original event start time but minutes is greater 											// if new event start time is equals to original event end time but minutes is lesser
			else if (newEvent.eventStartTime[0] > eventList[i].eventStartTime[0] && newEvent.eventStartTime[0] < eventList[i].eventEndTime[0] || newEvent.eventStartTime[0] == eventList[i].eventStartTime[0] && newEvent.eventStartTime[1] > eventList[i].eventStartTime[1] || newEvent.eventStartTime[0] == eventList[i].eventEndTime[0] && newEvent.eventStartTime[1] < eventList[i].eventStartTime[1]) {
				if (newEvent.eventEndTime[0] > eventList[i].eventStartTime[0] || newEvent.eventEndTime[0] == eventList[i].eventStartTime[0] && newEvent.eventEndTime[1] > eventList[i].eventStartTime[1]) { // if new event end time is later than original event start time) 
					crashList[crashIndex] = eventList[i];
					crashIndex++;
				}
			}
		}
	}
	if (crashIndex > 0) { //Crashed
		return 1;
	}
	else {
		return -1; //No Crash
	}
}

void DeleteEvent(int id) {
	//cout << id << endl;
	if (id == (currentIndex - 1)) {
		currentIndex--;
	}
	else {
		for (int i = id; i < currentIndex - 1; i++)
		{
			eventList[i] = eventList[i + 1];
		}
		currentIndex--;
		for (int i = id; i < currentIndex; i++)
		{
			eventList[i].id = i;
		}
	}
}

void O1() {
	Event newEvent;
	string tempString;
	bool flag;
	string tempStart, tempEnd;

	currentTry = 3;
	do
	{
		cout << "Please Enter the Event Title (Maximum of 40 characters) : ";
		getline(cin,tempString);
		currentTry--;
		if (tempString.length() > 40) {
			cout << "\n" << "*** Please Enter a Event Title Within 40 characters.You have " <<currentTry << " try left. ***" << "\n\n";
		}
	} while (tempString.length() > 40 && currentTry > 0);
	if (tempString.length() > 40 && currentTry == 0) {
		cout << "*** You have enter 3 wrong inputs,Returning to main menu. ***" << "\n\n";
		return;
	}
	strcpy(newEvent.eventTitle, tempString.c_str());

	currentTry = 3;
	do
	{
		cout << "Please Enter the Event Date (In DD-MM-YYYY) : ";
		cin >> tempString;
		cin.ignore(1024, '\n');
		currentTry--;
		flag = CheckDate(tempString);
		if (!flag && currentTry > 0) {
			cout << "Please enter again.You have "<< currentTry <<" try left. ***" << "\n\n";
		}
	} while (!flag && currentTry > 0);
	if (!flag && currentTry == 0) {
		cout  << "You have enter 3 wrong inputs,Returning to Main Menu ***" << "\n\n";
		return;
	}
	ConvertDate(newEvent, tempString);

	currentTry = 3;
	do
	{
		cout << "Please Enter the Event Start Time (In 24 hours form hh:mm) : ";
		cin >> tempStart;
		cin.ignore(1024, '\n');
		cout << "Please Enter the Event End Time (In 24 hours form hh:mm) : ";
		cin >> tempEnd;
		cin.ignore(1024, '\n');
		currentTry--;
		flag = CheckTime(tempStart, tempEnd);
		if (!flag && currentTry > 0) {
			cout  << "Please enter again.You have " << currentTry << " try left.***" << "\n\n";
		}
	} while (!flag && currentTry > 0);
	if (!flag && currentTry == 0) {
		cout << "You have enter 3 wrong inputs,Returning to Main Menu ***" << "\n\n";
		return;
	}
	ConvertTime(newEvent, tempStart, tempEnd);

	currentTry = 3;
	do
	{
		cout << "Please Enter the Event Type With a Single Integer ( (1) Home, (2) Work, (3) Study, (4) Play ) : ";
		getline(cin, tempString);
		currentTry--;
		flag = CheckType(tempString);
		if (!flag && currentTry > 0) {
			cout << "Please enter again.You have "<<currentTry<<" try left.***" << "\n\n";
		}
	} while (!flag && currentTry > 0);
	if (!flag && currentTry == 0) {
		cout << "You have enter 3 wrong inputs,Returning to Main Menu ***" << "\n\n";
		return;
	}
	newEvent.eventType = tempString[0];
	ConvertType(newEvent, tempString[0]);

	if (CheckTimeAva(newEvent) == -1) {
		newEvent.id = currentIndex;
		eventList[currentIndex] = newEvent;
		currentIndex++;
		cout << endl << "*** New event added successfully. ***" << endl;
	}
	else {
		cout << "\n" << "There are time crash between " << crashIndex << " event(s):" << endl;
		for (int i = 0; i < crashIndex; i++)
		{
			crashList[i].PrintAllContent();
		}
		cout << endl << "*** Failed adding new event. ***" << endl;
	}

}

//sorting function for samedate
void sortMatchEvent(Event matchEvent[], int countisize) {
	int smallestIndex;
	Event temp;

	for (int index = 0; index < countisize - 1; index++) {
		//  find smallest
		smallestIndex = index;
		for (int pos = index + 1; pos < countisize; pos++) {
			if (matchEvent[pos].eventStartTime[0] < matchEvent[smallestIndex].eventStartTime[0] || (matchEvent[pos].eventStartTime[0] == matchEvent[smallestIndex].eventStartTime[0] && matchEvent[pos].eventStartTime[1] < matchEvent[smallestIndex].eventStartTime[1]))
				smallestIndex = pos;

			//swap place
			temp = matchEvent[smallestIndex];
			matchEvent[smallestIndex] = matchEvent[index];
			matchEvent[index] = temp;
		}
	}
}

void O2() {
	Event deleteEvent, matchEvent[100];
	string date,tempchoice;
	bool flag, valid = true;
	int choice, countMatchEvent = 0;
	currentTry = 2;

	do {
		cout << "Please Enter a Event Date (In DD-MM-YYYY) of the event you want to delete : ";
		cin >> date;
		cin.ignore(1024, '\n');
		flag = CheckDate(date);
		if (!flag) {
			if (currentTry == 0) {
				cout << "You have typed a wrong date for 3 times.***\n\n***** Now returning to Main Meun. *****\n\n";
				return;
			}
			else {
				cout << "Please try again. You have " << currentTry << " more chance(s).***\n\n";
				currentTry--;
			}
		}
	} while (!flag);

	ConvertDate(deleteEvent, date);		// give require date event date
	for (int i = 0; i < currentIndex; i++) {	//compare all added events
		if (eventList[i].eventDate[0] == deleteEvent.eventDate[0] && eventList[i].eventDate[1] == deleteEvent.eventDate[1] && eventList[i].eventDate[2] == deleteEvent.eventDate[2]) {		//find all match events
			matchEvent[countMatchEvent] = eventList[i];
			countMatchEvent++;
		}
	}

	if (countMatchEvent == 0) {
		cout << "There is no event on this date!\n";		//if no matched event 
		return;
	}

	sortMatchEvent(matchEvent, countMatchEvent ); //display all matched events in sorted order
	cout << endl;
	for (int index = 0; index < countMatchEvent; index++) {	
		cout << index + 1 << " : ";
		matchEvent[index].PrintAllContent();
		cout << "\n";
	}

	currentTry = 2;
	do {
		choice = 0;
		valid = true;
		cout << "Please choose an event index that showed above for deletion : ";
		getline(cin, tempchoice);
		if (!CheckCharInt(tempchoice)) {
			cout << "Input is Invalid. Please try again. You have " << currentTry << " more chance(s).\n\n";
			valid = false;
		}
		if (valid) {
			choice = stoi(tempchoice);
		}
		if (choice < 1 || choice > countMatchEvent) {	//choice checking
			if (currentTry == 0) {	
				cout << "You have typed a wrong index for 3 times.\n***** Now returning to Main Meun. *****\n\n";
				return;
			}
			else if(valid){
				valid = false;
				cout << "Input is Invalid. Please try again. You have " << currentTry << " more chance(s).\n\n";
			}
		}
		currentTry--;
	} while (!valid);

	DeleteEvent(matchEvent[choice - 1].id); // delete one event
	cout << "Deletion succesful! There is " << countMatchEvent - 1 << " event(s) left on this date.\n";
}


void O3() {
	string input;
	Event newEvent;
	int success = 0, failed = 0;
	do
	{
		getline(cin, input);
		if (input != "0") {
			int pos1, pos2, pos3, pos4;
			pos1 = input.find_first_of('/');
			pos2 = input.find_first_of('/', pos1 + 1);
			pos3 = input.find_first_of('/', pos2 + 1);
			pos4 = input.find_last_of('/');

			string date, stime, etime, Title, type;

			date = input.substr(0, pos1);
			stime = input.substr(pos1 + 1, pos2 - pos1 - 1);
			etime = input.substr(pos2 + 1, pos3 - pos2 - 1);
			Title = input.substr(pos3 + 1, pos4 - pos3 - 1);
			type = input.substr(pos4 + 1, input.length() - pos4 - 1);

			strcpy(newEvent.eventTitle, Title.c_str());
			ConvertDate(newEvent, date);
			ConvertTime(newEvent, stime, etime);
			newEvent.eventType = type[0];
			ConvertType(newEvent, newEvent.eventType);

			int flag = CheckTimeAva(newEvent);
			if (flag == 1)
			{
				failed++;
			}
			else {
				newEvent.id = currentIndex;
				eventList[currentIndex] = newEvent;
				currentIndex++;
				success++;
			}
		}
	} while (input != "0");
	cout << "Total " << success + failed << " events in batch" << endl;
	cout << success << " event(s) added" << endl;
	cout << failed << " event(s) not added due to time conflict\n";
}

//sort order from the earliest to the latest
void sortEvent(Event addedEvent[], int countisize) {
	int smallestIndex;
	Event temp;

	for (int index = 0; index < countisize - 1; index++) {
		//  find smallest
		smallestIndex = index;
		for (int pos = index + 1; pos < countisize; pos++) {
			if (addedEvent[pos].eventDate[0] == addedEvent[smallestIndex].eventDate[0] && addedEvent[pos].eventDate[1] == addedEvent[smallestIndex].eventDate[1] && addedEvent[pos].eventDate[2] == addedEvent[smallestIndex].eventDate[2]) { //events scheduled on same day
				if (addedEvent[pos].eventStartTime[0] < addedEvent[smallestIndex].eventStartTime[0] || (addedEvent[pos].eventStartTime[0] == addedEvent[smallestIndex].eventStartTime[0] && addedEvent[pos].eventStartTime[1] < addedEvent[smallestIndex].eventStartTime[1]))
					smallestIndex = pos;
			}
									//different years																	/same year but different months																																							// same year same month but different day
			else if(addedEvent[pos].eventDate[2] < addedEvent[smallestIndex].eventDate[2] || addedEvent[pos].eventDate[2] == addedEvent[smallestIndex].eventDate[2] && addedEvent[pos].eventDate[1] < addedEvent[smallestIndex].eventDate[1] || addedEvent[pos].eventDate[2] == addedEvent[smallestIndex].eventDate[2] && addedEvent[pos].eventDate[1] == addedEvent[smallestIndex].eventDate[1] && addedEvent[pos].eventDate[0] < addedEvent[smallestIndex].eventDate[0])
				smallestIndex = pos;

			//swap place
			temp = addedEvent[smallestIndex];
			addedEvent[smallestIndex] = addedEvent[index];
			addedEvent[index] = temp;
		}
	}
}


//4.1
void showAllEvents() {
	if (currentIndex == 0) {
		cout << "***** There is no event yet! *****\n\n";
		return O4();
	}
	cout << left << setfill(' ');
	cout << setw(25) << "Date and Time" << setw(45) << "Title" << setw(5) << "Type\n";
	sortEvent(eventList, currentIndex); //sort order from the earliest to the latest
	for (int i = 0; i < currentIndex; i++) { // display all events
		cout << right << setfill('0') << setw(2) << eventList[i].eventDate[0]<< "-"<< setw(2) << eventList[i].eventDate[1] << "-" << setw(4) << eventList[i].eventDate[2] << "," << setw(2) << eventList[i].eventStartTime[0] << ":" << setw(2) << eventList[i].eventStartTime[1] << "-" << setw(2) << eventList[i].eventEndTime[0] << ":" << setw(2) << eventList[i].eventEndTime[1] << "   " << setfill(' ') << left << setw(45) << eventList[i].eventTitle << setw(5) << eventList[i].eventTypeName << endl;
	}
	return O4();
};


//4.2 
void showAllEventsList() {
	Event matchEvent[100];
	time_t t = time(NULL);
	tm* tPtr = localtime(&t);
	int number, counter = 0, currentDay, currentMonth, currentYear, maxDay, endDay;
	string tempnumber;
	char Type;
	string wantedType;
	bool valid = true;
	currentTry = 2;

	number = 0;

	//ask specific type 
	do {
		cout << "Please enter a event Type from 1-4: ";
		cin >> wantedType;
		cin.ignore(1024,'\n');
		if (!(CheckType(wantedType))) {
			if (currentTry == 0) {
				cout << "You have typed a wrong Type for 3 times.***\n\n***** Now returning to Show Main Meun. *****\n\n";
				return O4();
			}
			else {
			valid = false;
			cout << "Please choose a Type within 1-4. You have " << currentTry << " more chance(s).***\n\n";
			currentTry--;
			}
		}
		else
			valid = true;

	} while (!valid);
	Type = wantedType[0];

	currentTry = 2;
	//ask  value of n
	do {
		cout << "Please enter a number of day(s) ranged from 1-7 to find scheduled events in the upcoming days: ";
		getline(cin, tempnumber);
		valid = true;
		if (!CheckCharInt(tempnumber) || tempnumber.length() != 1) {
			valid = false;
			cout << "Input is out of range. Please input a number ranged from 1-7. You have " << currentTry << " more chance(s).\n\n";
		}
		if (valid) {
			number = stoi(tempnumber);
		}
		if (!(number >= 1 && number <= 7)) {
			if (currentTry == 0) {
				cout << "You have typed a wrong number for 3 times.\n***** Now returning to Show Main Meun. *****\n\n";
				return O4();
			}
			else if (valid){
				valid = false;
				cout << "Input is out of range. Please input a number ranged from 1-7. You have " << currentTry << " more chance(s).\n\n";
			}
		}
		currentTry--;
	} while (!valid);
	
	currentDay = (tPtr->tm_mday);
	currentMonth = (tPtr->tm_mon) + 1;
	currentYear = (tPtr->tm_year) + 1900;
	endDay = currentDay + number;	//End point

	// maximun day for current month
	if (currentYear % 4 == 0 && currentYear % 100 != 0 || currentYear % 400 == 0) {
		if (currentMonth == 2)
			maxDay = 29;
		else if (currentMonth == 4 || currentMonth == 6 || currentMonth == 9 || currentMonth == 11)
			maxDay = 30;
		else
			maxDay = 31;
	}
	else {
		if (currentMonth == 2)
			maxDay = 28;
		else if (currentMonth == 4 || currentMonth == 6 || currentMonth == 9 || currentMonth == 11)
			maxDay = 30;
		else
			maxDay = 31;
	}

	int startDay = currentDay; //use for for loop 

	for (int d = startDay; d <= endDay; d++) {
		if (currentDay > maxDay) {
			currentDay = currentDay - maxDay;	//more than maxday
			currentMonth += 1;		//month increase 1 
		}
		for (int i = 0; i < currentIndex; i++) {
			if (eventList[i].eventDate[0] == currentDay && eventList[i].eventDate[1] == currentMonth && eventList[i].eventDate[2] == currentYear && eventList[i].eventType == Type) {	//check every event equals to specfic type from current day to scheduled in the coming n days 
				matchEvent[counter] = eventList[i];
				counter++;
			}
		}
		currentDay++;
	}

	if (counter == 0) {
		cout << "There are no events of the specified event type scheduled in the coming "<< number << " days!\n";		//if no matched event 
		return;
	}

	sortEvent(matchEvent, counter); // sort from the earliest to the latest
	cout << endl;
	cout << left << setfill(' ');
	cout << setw(25) << "Date and Time" << setw(45) << "Title" << setw(5) << "Type\n";
	for (int i = 0; i < counter; i++) {
		cout << right << setfill('0') << setw(2) << matchEvent[i].eventDate[0] << "-" << setw(2) << matchEvent[i].eventDate[1] << "-" << setw(4) << matchEvent[i].eventDate[2] << "," << setw(2) << matchEvent[i].eventStartTime[0] << ":" << setw(2) << matchEvent[i].eventStartTime[1] << "-" << setw(2) << matchEvent[i].eventEndTime[0] << ":" << setw(2) << matchEvent[i].eventEndTime[1] << "   " << setfill(' ') << left << setw(45) << matchEvent[i].eventTitle << setw(5) << matchEvent[i].eventTypeName << endl;
	}

	return O4();
}

void showAllEventCalender() {
	string time, stringyear, stringmonth;
	bool flag;
	int month, year, ctry;


	string weekdays[7] = { "Sunday","Monday","Tuesday","Wednesday","Thusday","Friday","Saturday" };
	month = 0;
	year = 0;
	ctry = 3;
	do
	{
		flag = true;
		cout << "Please Enter month and year (In MM-YYYY) : ";
		cin >> time;
		stringyear = time.substr(time.find_first_of('-') + 1, time.length() - time.find_first_of('-'));
		stringmonth = time.substr(0, time.find_first_of('-'));
		ctry--;
		if ((stringmonth.length() < 2 || stringyear.length() != 4 || !CheckCharInt(stringmonth)|| !CheckCharInt(stringyear))) {
			if (ctry > 0) {
				cout << "***Incorrect format! Please input again.You have " << ctry << " try left.***" << endl;
			}
			flag = false;
			
		}
		if (flag && (CheckCharInt(stringmonth) && CheckCharInt(stringyear))) {
			month = stoi(stringmonth); //finding month in int
			year = stoi(stringyear); //finding year in int
			
		}
		if ((!(month >= 1 && month <= 12) || !(year >= 1000 && year <= 9999) ) && ctry > 0 && flag) {
			if (ctry > 0) {
				cout << "***Input out of range! Please input again.You have " << ctry << " try left.***" << endl;
			}
			flag = false;
			
		}
		if (ctry == 0 && !flag) {
			cout <<'\n'<< "***Returning to main menu.***" << endl;
			return;
		}
	} while (!flag);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int twidth, cwidth, cheight;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	twidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;  //Getting the current console width to make it varies while in different console size
	int remainder = (twidth - 8) % 7;
	twidth -= remainder;
	cwidth = (twidth - 8) / 7;

	bool leap = false;
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) { //Check Leap Year
		leap = true;
	}

	int numOfdays = 0;
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) { //Check Date
		numOfdays = 31;
	}
	else if (month == 2) {
		if (leap) {
			numOfdays = 29;
		}
		else {
			numOfdays = 28;
		}
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11) {
		numOfdays = 30;
	}

	int yrLast = stoi(stringyear.substr(2, 2));
	int yrFirst = stoi(stringyear.substr(0, 2));

	int monthnum = month - 2;
	if (monthnum == 0) {
		monthnum = 12;
		yrLast--;
	}
	else if (monthnum == -1) {
		monthnum = 11;
		yrLast--;
	}

	int num = 1 + ((13 * monthnum - 1) / 5) + yrLast + (yrLast / 4) + (yrFirst / 4) - (2 * yrFirst); //Zeller's rule output
	int weekday;
	if (num >= 0) {
		weekday = num % 7;
	}
	else {
		weekday = num % 7 + 7;
	}

	cout << setfill('-') << setw(twidth) << "" << endl;
	cout << "|" << setfill(' ') << setw(twidth / 2) << time << setw(twidth / 2 - 1) << "|" << endl;  //Center the title
	cout << setfill('-') << setw(twidth) << "" << endl;

	for (int i = 0; i < 7; i++)
	{
		cout << "|" << setfill(' ') << setw((cwidth - weekdays[i].length()) / 2) << "" << left << setw(cwidth - (cwidth - weekdays[i].length()) / 2) << weekdays[i]; //display weekdays
		cout << right;
	}
	cout << "|" << endl;
	cout << setfill('-') << setw(twidth) << "" << endl;

	Event MonthEvents[32][100];  //Get all the events in that month
	int Amount[32]; //Number of events each day
	for (int i = 1; i <= numOfdays; i++)
	{
		Amount[i] = 0;
		for (int n = 0; n < currentIndex; n++)
		{
			if (eventList[n].eventDate[0] == i && eventList[n].eventDate[1] == month && eventList[n].eventDate[2] == year) {
				MonthEvents[i][Amount[i]] = eventList[n];
				Amount[i]++;
			}
		}
	}

	for (int i = 1; i <= numOfdays; i++)  //Sorting events each day
	{
		for (int n = 0; n < Amount[i]; n++)
		{
			int smallestIndex = n;
			for (int x = n + 1; x < Amount[i]; x++)
			{
				if (MonthEvents[i][smallestIndex].eventStartTime[0] > MonthEvents[i][x].eventStartTime[0]) { //for different start hours
					smallestIndex = x;
				}
				else if (MonthEvents[i][smallestIndex].eventStartTime[0] = MonthEvents[i][x].eventStartTime[0] && MonthEvents[i][smallestIndex].eventStartTime[1] > MonthEvents[i][x].eventStartTime[1]) {//for same start hours
					smallestIndex = x;
				}
			}
			Event temp = MonthEvents[i][n];
			MonthEvents[i][n] = MonthEvents[i][smallestIndex];
			MonthEvents[i][smallestIndex] = temp;
		}
	}

	int maxdate = 1;
	int startdate = 0;
	int nowdate = 0;
	bool done[7];
	for (int i = 0; i < 6; i++)  //PrintOut Events each week
	{
		nowdate = maxdate;
		startdate = maxdate;
		for (int n = 0; n < 7; n++)
		{
			done[n] = false;
		}

		if (i == 0) { //First Week
			for (int n = 0; n < 7; n++) //Print date
			{
				if (weekday == 7 || n >= weekday) {
					cout << "|" << setfill(' ') << setw(cwidth / 2) << "" << setw(cwidth / 2) << left << maxdate;
					maxdate++;
				}
				else if (n < weekday) {
					cout << "|" << setfill(' ') << setw(cwidth) << "";
					done[n] = true;
				}
			}
			cout << right;
			cout << "|" << endl;
			cout << setfill('-') << setw(twidth) << "" << endl;

			int step = 0;
			while (!(done[0] && done[1] && done[2] && done[3] && done[4] && done[5] && done[6])) {
				for (int n = 0; n < 7; n++) //Print time
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else if (step >= Amount[nowdate]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
						done[n] = true;
					}
					else {
						cout << "|" << setfill(' ') << setw(ceil((cwidth - 11) / 2)) << "" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[1] << "-" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[1] << setfill(' ') << setw(ceil((cwidth - 11) / 2) + 1) << "";
						nowdate++;
					}
				}
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				for (int n = 0; n < 7; n++) //Print title
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else {
						if (strlen(MonthEvents[nowdate][step].eventTitle) <= cwidth) {  //for title smaller than cell width
							cout << "|" << setfill(' ') << setw(cwidth) << left << MonthEvents[nowdate][step].eventTitle;
							nowdate++;
						}
						else {
							cout << "|";
							for (int x = 0; x < cwidth - 3; x++)  //for title longer than cell width
							{
								cout << MonthEvents[nowdate][step].eventTitle[x];
							}
							cout << setfill('.') << setw(3) << "";
							nowdate++;
						}
					}
				}
				cout << setfill(' '); //reset
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				step++;
			}
			cout << setfill('-') << setw(twidth) << "" << endl;
		}
		else if (i == 1 || i == 2 || i == 3) { //Mid three weeks
			for (int n = 0; n < 7; n++) //Print date
			{
				cout << "|" << setfill(' ') << setw(cwidth / 2) << "" << setw(cwidth / 2) << left << maxdate;
				maxdate++;
			}
			cout << right;
			cout << "|" << endl;
			cout << setfill('-') << setw(twidth) << "" << endl;
			int step = 0;
			while (!(done[0] && done[1] && done[2] && done[3] && done[4] && done[5] && done[6])) {
				for (int n = 0; n < 7; n++) //Print time
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else if (step >= Amount[nowdate]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
						done[n] = true;
					}
					else {
						cout << "|" << setfill(' ') << setw(ceil((cwidth - 11) / 2)) << "" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[1] << "-" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[1] << setfill(' ') << setw(ceil((cwidth - 11) / 2) + 1) << "";
					}
					nowdate++;
				}
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				for (int n = 0; n < 7; n++) //Print title
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else {
						if (strlen(MonthEvents[nowdate][step].eventTitle) <= cwidth) {  //for title smaller than cell width
							cout << "|" << setfill(' ') << setw(cwidth) << left << MonthEvents[nowdate][step].eventTitle;
						}
						else {
							cout << "|";
							for (int x = 0; x < cwidth - 3; x++)  //for title longer than cell width
							{
								cout << MonthEvents[nowdate][step].eventTitle[x];
							}
							cout << setfill('.') << setw(3) << "";
						}
					}
					nowdate++;
				}
				cout << setfill(' '); //reset
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				step++;
			}
			cout << setfill('-') << setw(twidth) << "" << endl;
		}
		else if (i == 4) { //LastWeek For some months
			for (int n = 0; n < 7; n++) //Print date
			{
				if (!(maxdate > numOfdays)) {
					cout << "|" << setfill(' ') << setw(cwidth / 2) << "" << setw(cwidth / 2) << left << maxdate;
					maxdate++;
				}
				else {
					cout << "|" << setw(cwidth) << "";
				}
			}
			cout << right;
			cout << "|" << endl;
			cout << setfill('-') << setw(twidth) << "" << endl;
			int step = 0;
			while (!(done[0] && done[1] && done[2] && done[3] && done[4] && done[5] && done[6])) {
				for (int n = 0; n < 7; n++) //Print time
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else if (nowdate > numOfdays) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
						done[n] = true;
					}
					else if (step >= Amount[nowdate]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
						done[n] = true;
					}
					else {
						cout << "|" << setfill(' ') << setw(ceil((cwidth - 11) / 2)) << "" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[1] << "-" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[1] << setfill(' ') << setw(ceil((cwidth - 11) / 2) + 1) << "";
					}
					nowdate++;
				}
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				for (int n = 0; n < 7; n++) //Print title
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else {
						if (strlen(MonthEvents[nowdate][step].eventTitle) <= cwidth) {  //for title smaller than cell width
							cout << "|" << setfill(' ') << setw(cwidth) << left << MonthEvents[nowdate][step].eventTitle;
						}
						else {
							cout << "|";
							for (int x = 0; x < cwidth - 3; x++)  //for title longer than cell width
							{
								cout << MonthEvents[nowdate][step].eventTitle[x];
							}
							cout << setfill('.') << setw(3) << "";
						}
					}
					nowdate++;
				}
				cout << setfill(' '); //reset
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				step++;
			}
			cout << setfill('-') << setw(twidth) << "" << endl;
		}
		else if (i == 5 && maxdate <= numOfdays) { //for the months having 6 weeks
			for (int n = 0; n < 7; n++) //Print date
			{
				if (!(maxdate > numOfdays)) {
					cout << "|" << setfill(' ') << setw(cwidth / 2) << "" << setw(cwidth / 2) << left << maxdate;
					maxdate++;
				}
				else {
					cout << "|" << setw(cwidth) << "";
				}
			}
			cout << right;
			cout << "|" << endl;
			cout << setfill('-') << setw(twidth) << "" << endl;
			int step = 0;
			while (!(done[0] && done[1] && done[2] && done[3] && done[4] && done[5] && done[6])) {
				for (int n = 0; n < 7; n++) //Print time
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else if (nowdate > numOfdays) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
						done[n] = true;
					}
					else if (step >= Amount[nowdate]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
						done[n] = true;
					}
					else {
						cout << "|" << setfill(' ') << setw(ceil((cwidth - 11) / 2)) << "" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventStartTime[1] << "-" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[0] << ":" << setfill('0') << setw(2) << MonthEvents[nowdate][step].eventEndTime[1] << setfill(' ') << setw(ceil((cwidth - 11) / 2) + 1) << "";
					}
					nowdate++;
				}
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				for (int n = 0; n < 7; n++) //Print title
				{
					if (done[n]) {
						cout << "|" << setfill(' ') << setw(cwidth) << "";
					}
					else {
						if (strlen(MonthEvents[nowdate][step].eventTitle) <= cwidth) {  //for title smaller than cell width
							cout << "|" << setfill(' ') << setw(cwidth) << left << MonthEvents[nowdate][step].eventTitle;
						}
						else {
							cout << "|";
							for (int x = 0; x < cwidth - 3; x++)  //for title longer than cell width
							{
								cout << MonthEvents[nowdate][step].eventTitle[x];
							}
							cout << setfill('.') << setw(3) << "";

						}
					}
					nowdate++;
				}
				cout << setfill(' '); //reset
				cout << right;
				cout << "|" << endl;
				nowdate = startdate;
				step++;
			}
			cout << setfill('-') << setw(twidth) << "" << endl;
		}
	}
	return O4();
}



void O4() {
	int option;
	string optiontemp;
	bool valid = false;
	currentTry = 2;

	do {
		option = 0;
		valid = true;
		cout << "\n***** Show Event Menu *****\n";
		cout << "[1] Show all events\n";
		cout << "[2] Show events(list view)\n";
		cout << "[3] Show events(calendar view)\n";
		cout << "[4] Return to Main Menu\n";
		cout << "***************************\n";
		cout << "Option(1 - 4) : ";
		getline(cin, optiontemp);
		if (!CheckCharInt(optiontemp) || optiontemp.length() != 1) {
			valid = false;
			cout << "Input is Invalid. Please choose option within 1-4. You have " << currentTry << " more chance(s).\n\n";
		}
		if (valid) {
			option = stoi(optiontemp);
		}
		if (!(option >= 1 && option <= 4)) {
			if (currentTry == 0) {
				cout << "You have typed a wrong option for 3 times.\n***** Now returning to Main Meun. *****\n\n";
				return;
			}
			else if (valid){
				valid = false;
				cout << "Input is Invalid. Please choose option within 1-4. You have " << currentTry << " more chance(s).\n\n";			
			}
		}
		currentTry--;
	} while (!valid);
	cout << endl;

	switch (option)
	{
	case 1:
		showAllEvents();
		break;
	case 2:
		showAllEventsList();
		break;
	case 3:
		showAllEventCalender();
	case 4:
		return;
	}
}

void O5() {
		cout << endl;
		cout << setfill(' ') << left;
		cout << setw(20) << "Name: " << setw(20) << "Student ID: " << setw(20) << "Class: " << endl;
		cout << setw(20) << "Yeung Long Sang" << setw(20) << "19130920A" << setw(20) << "201C" << endl;
		cout << setw(20) << "Ngai Tsz Hin" << setw(20) << "19000386A" << setw(20) << "201B" << endl;
		cout << setw(20) << "Lau Ngai Yin" << setw(20) << "19135987A" << setw(20) << "201A" << endl;
}

void O6(bool &run) {
	string choice; 
	bool valid = true;
	currentTry = 2;

	do {
		cout << "*** Do you want to exist the Schedule Book? ***\n";
		cout << "Please enter 'y'/'Y' to confirm or 'n'/'N' to decline: ";
		cin >> choice;
		cin.clear(); 
		cin.ignore(1024, '\n');
		if (!(choice == "y" || choice == "Y" || choice == "n" || choice == "N")) { 
			if (currentTry == 0) {
				cout << "You have typed a wrong input for 3 times.\n***** Now returning to Main Meun. *****\n\n";
				return;
			}
			else {
				cout << "\n*** ERROR! Please enter 'y'/'Y' to confirm or 'n'/'N' to decline! *** \n";
				valid = false;
				cout << "You have " << currentTry << " more chance(s).\n\n";
				currentTry--;
			}
		}
		else if (choice == "y" || choice == "Y") {
			run = false;	// return to main meun and end the loop
			return;
		}
		else if (choice == "n" || choice == "N")
			return;   // return to main meun

	} while (!valid);
}
  
int main() {
	string optiontemp;
	int option;
	bool running = true;

	cout << "Welcome to Schedule Book, What can I help you today?" << endl;
	do
	{
		cout << endl;
		cout << "*** Main Menu ***" << endl;
		for (int i = 1; i <= 6; i++)
		{
			cout << "[" << i << "] ";
			switch (i)
			{
			case 1:
				cout << "Add one event" << endl;
				break;
			case 2:
				cout << "Delete one event" << endl;
				break;
			case 3:
				cout << "Add events in batch" << endl;
				break;
			case 4:
				cout << "Show events" << endl;
				break;
			case 5:
				cout << "Credits" << endl;
				break;
			case 6:
				cout << "Exit" << endl;
				break;
			}
		}
		cout << "*****************" << endl;
		cout << "Option (1 - 6): ";
		option = 0;
		getline(cin, optiontemp);
		if (optiontemp.length() == 1 && CheckCharInt(optiontemp)) {
			option = stoi(optiontemp);
		}
		switch (option)
		{
		case 1:
			O1();
			break;
		case 2: 
			O2();
			break;
		case 3:
			O3();
			break;
		case 4:
			O4();
			break;
		case 5:
			O5();
			break;
		case 6:
			O6(running); //pass by reference and see if the user input 'y' or 'Y' then end the program
			break;
		default:
			cout << "\n*** ERROR! Please enter a number ranged 1 - 6. ***" << endl;
			break;
		}
	} while (running);
	cout << "\nThanks for using schedule book! Goodbye!\n";
	return 0;
}