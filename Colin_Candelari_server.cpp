#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

void error(char *msg) {
	perror(msg);
	exit(1);
}
void toBinary(string number, string bArray[]);
void encode(string barray[], string walsh[], vector<string> &vector); /*vector must be passed by reference, else the values passed inside the function won't stick*/
void setDecode(string number1, string number2, string number3, string dewalsh1[], string dewalsh2[], string dewalsh3[]);


int main(int argc, char* argv[]) {

	/*
	Files were compiled with the lines	g++ -std=c++11 3360A2Client.cpp -o ctest	and	g++ -std=c++11 3360A2Server.cpp -o stest	on the server.
	Also the Server file MUST be initiated first,
	else the files don't communicate.
	*/

	string walsh0[] = { "-1","-1","-1","-1" }, walsh1[] = { "-1","1","-1","1" }, walsh2[] = { "-1","-1","1","1" }, walsh3[] = { "-1","1","1","-1" };


	int sockfd, newsockfd, newsockfd2, newsockfd3, portno;
	socklen_t clilen;
	char buffer1[256], buffer2[256], buffer3[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		perror("Error no port provided");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Error opening socket");
	}
	bzero((char*)&serv_addr, sizeof(serv_addr));
	
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	////////SOCKET 1/////////
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Error on binding 1");
		exit(0);
		/*
		I don't know if this exit needs to be here
		but it seemed like things froze up
		if the port was already used so I put it here
		just in case.
		*/
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
	if (newsockfd < 0) {
		perror("Error on accept");
	}
	bzero(buffer1, 256);
	n = read(newsockfd, buffer1, 255);
	if (n < 0) {
		perror("Error reading from socket");
	}
	

	string set1 = std::string(buffer1), p1info, num1;
	int i1 = 0, counter1=0;
	stringstream ssin1(set1);//this block is repeated for each socket, it should be function, but the function didn't work right away so I moved on
	while (ssin1.good() && i1 < 2) {
		if (counter1 % 2 == 0) {
			ssin1 >> p1info;
			//cout << "Process " << p1info << " is receiving: ";
		}
		else {

			ssin1 >> num1;
			//cout << num1 << endl;
		}
		counter1++;
		++i1;
	}
	cout << "Here is the message from child 1: Value = " << num1 << ", Destination = " << p1info << endl;
	string binary1[3];
	toBinary(num1, binary1);
	//cout << "Binary: ";
	for (int i = 0; i < 3; i++) {
		//cout << binary1[i] << " ";
	}
	
	vector <string> EM1;
	//cout << endl << "Encoded message ";
	encode(binary1, walsh1, EM1);
	for (int i = 0; i < EM1.size(); i++) {
		//cout << EM1.at(i)<<" ";
	}
	//cout << endl;

	//this is where it was 
	

	///////////SOCKET 2//////////
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd2 = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
	if (newsockfd2 < 0) {
		perror("Error on accept 2");
	}
	bzero(buffer2, 256);
	n = read(newsockfd2, buffer2, 255);
	if (n < 0) {
		perror("Error reading from socket 2");
	}
	//cout << "Here is the message 2: " << buffer2;

	string set2 = std::string(buffer2), p2info, num2;
	int i2 = 0, counter2 = 0;
	stringstream ssin2(set2);
	while (ssin2.good() && i2 < 2) {
		if (counter2 % 2 == 0) {
			ssin2 >> p2info;
			//cout << "Process " << p2info << " is receiving: ";
		}
		else {

			ssin2 >> num2;
			//cout << num2 << endl;
		}
		counter2++;
		++i2;
	}
	cout << "Here is the message from child 2: Value = " << num2 << ", Destination = " << p2info << endl;
	string binary2[3];
	toBinary(num2, binary2);
	//cout << "Binary: ";
	for (int i = 0; i < 3; i++) {
		//cout << binary2[i] << " ";
	}
	
	vector <string> EM2;
	//cout << endl << "Encoded message ";
	encode(binary2, walsh2, EM2);
	for (int i = 0; i < EM2.size(); i++) {
		//cout << EM2.at(i) << " ";
	}
	//cout << endl;

	////////////SOCKET3///////////
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd3 = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
	if (newsockfd3 < 0) {
		perror("Error on accept 3");
	}
	bzero(buffer3, 256);
	n = read(newsockfd3, buffer3, 255);
	if (n < 0) {
		perror("Error reading from socket 3");
	}
	//cout << "Here is the message 3: " << buffer3;

	string set3 = std::string(buffer3), p3info, num3;
	int i3 = 0, counter3 = 0;
	stringstream ssin3(set3);
	while (ssin3.good() && i3 < 2) {
		if (counter3 % 2 == 0) {
			ssin3 >> p3info;
			//cout << "Process " << p3info << " is receiving: ";
		}
		else {

			ssin3 >> num3;
			//cout << num3 << endl;
		}
		counter3++;
		++i3;
	}
	cout << "Here is the message from child 3: Value = " << num3 << ", Destination = " << p3info << endl;
	string binary3[3];
	toBinary(num3, binary3);
	//cout << "Binary: ";
	for (int i = 0; i < 3; i++) {
		//cout << binary3[i] << " ";
	}
	vector <string> EM3;
	//cout << endl << "Encoded message ";
	encode(binary3, walsh3, EM3);
	for (int i = 0; i < EM3.size(); i++) {
		//cout << EM3.at(i) << " ";
	}
	//cout << endl;
	
	//creating the signal to be sent
	vector<int> signal;
	vector<int> iEM1, iEM2, iEM3;
	for (int i = 0; i < EM1.size(); i++) {
		iEM1.push_back(stoi(EM1.at(i)));
		iEM2.push_back(stoi(EM2.at(i)));
		iEM3.push_back(stoi(EM3.at(i)));
	}
	//cout << "Signal: ";
	for (int i = 0; i < iEM1.size(); i++) {
		signal.push_back(iEM1.at(i) + iEM2.at(i) + iEM3.at(i));
		//cout << signal.at(i) << " ";
	}
	//cout << endl;
	int sigArr[16];
	int sigArr2[16];
	int sigArr3[16];
	//int sigCount = 12;
	for (int i = 0; i < 12; i++) {
		sigArr[i] = signal.at(i);
		sigArr2[i] = signal.at(i);
		sigArr3[i] = signal.at(i);
		//cout << sigArr[i] << " ";
	}
	
	//setting which walsh codes will decode which messages
	string deWalsh1[4], deWalsh2[4], deWalsh3[4]; //one for each process.
	setDecode(p1info, p2info, p3info, deWalsh1, deWalsh2, deWalsh3);
	int deWalsh1i[4], deWalsh2i[4], deWalsh3i[4];
	for (int i = 0; i < 4; i++) {
		sigArr[i+12] = stoi(deWalsh1[i]);
		sigArr2[i+12] = stoi(deWalsh2[i]);
		sigArr3[i+12] = stoi(deWalsh3[i]);
	}
	/*
	cout << "DeWalsh1: ";
	for (int i = 0; i < 4; i++) {
		cout << deWalsh1[i] << " ";
	}
	cout << endl;
	cout << "DeWalsh2: ";
	for (int i = 0; i < 4; i++) {
		cout << deWalsh2[i] << " ";
	}
	cout << endl;
	cout << "DeWalsh3: ";
	for (int i = 0; i < 4; i++) {
		cout << deWalsh3[i] << " ";
	}
	cout << endl;
	*/


	/*
	n = write(newsockfd, "I got your message 1", 26);
	if (n < 0) {
		perror("Error writing to socket 1");
	}
	sleep(1);
	*/
	for (int i = 0; i < 16; i++) {
		if (send(newsockfd, &sigArr[i], 16 * sizeof(int), 0) < 0) {
			cout << "Error sending" << endl;;
		}
	}
	sleep(1);
	
	/*
	n = write(newsockfd2, "I got your message 2", 26);
	if (n < 0) {
		perror("Error writing to socket 2");
	}
	sleep(1);
	*/
	for (int i = 0; i < 16; i++) {
		if (send(newsockfd2, &sigArr2[i], 16 * sizeof(int), 0) < 0) {
			cout << "Error sending" << endl;;
		}
	}
	sleep(1);

	/*
	n = write(newsockfd3, "I got your message 3", 26);
	if (n < 0) {
		perror("Error writing to socket 3");
	}
	sleep(1);
	*/
	for (int i = 0; i < 16; i++) {
		if (send(newsockfd3, &sigArr3[i], 16 * sizeof(int), 0) < 0) {
			cout << "Error sending" << endl;;
		}
	}
	sleep(1);

	//look for this is where it was 
	
	close(sockfd);

	return 0;
}

void toBinary(string number, string barray[]) {

	//binary1
	/*
	it is crucial that the binary is added in the order shown i. e. 0 then 1 then 2, that way it will be printed
	such as for 4
	1, -1, -1
	instead of incorrectly as 1 aka
	-1, -1, 1.
	*/

	//cout << "from the function. Integer = "<< number << endl;
	if (number == "0") {//-1 -1 -1

		barray[0] = "-1", barray[1] = "-1", barray[2] = "-1";
	}
	if (number == "1") {//-1 -1 1

		barray[0] = "-1", barray[1] = "-1", barray[2] = "1";
	}
	if (number == "2") {//-1 1 -1

		barray[0] = "-1", barray[1] = "1", barray[2] = "-1";
	}
	if (number == "3") {//-1 1 1

		barray[0] = "-1", barray[1] = "1", barray[2] = "1";
	}
	if (number == "4") {//1 -1 -1

		barray[0] = "1", barray[1] = "-1", barray[2] = "-1";
	}
	if (number == "5") {//1 -1 1

		barray[0] = "1", barray[1] = "-1", barray[2] = "1";
	}
	if (number == "6") {//1 1 -1

		barray[0] = "1", barray[1] = "1", barray[2] = "-1";
	}
	if (number == "7") {//1 1 1

		barray[0] = "1", barray[1] = "1", barray[2] = "1";
	}

	//cout << "barray[0]: " << barray[0] << " barray[1]: " << barray[1] << " barray[2]: " << barray[2] << endl;
}

void encode(string barray[], string walsh[], vector<string> &vector) {
	for (int i = 0; i < 3; i++) {
		//cout << "Binary: " << barray[i] << endl;
		if (barray[i] == "1") {
			for (int j = 0; j < 4; j++) {
				//cout << "Walsh: " << walsh[j] << endl;
				vector.push_back(walsh[j]);
			}
		}
		if (barray[i] == "-1") {
			for (int j = 0; j < 4; j++) {
				if (walsh[j] == "1") {
					vector.push_back("-1");
				}
				else {
					vector.push_back("1");
				}
			}
		}
	}
	//cout << endl;
	for (int i = 0; i < vector.size(); i++) {
		//cout << vector.at(i) << ", ";
	}
}

void setDecode(string number1, string number2, string number3, string dewalsh1[], string dewalsh2[], string dewalsh3[]) {

	/*
	walsh1 = -1 1 -1 1
	walsh2 = -1 -1 1 1
	walsh3 = -1 1 1 -1
	*/
	if (number1 == "3") {
		dewalsh3[0] = "-1", dewalsh3[1] = "1", dewalsh3[2] = "-1", dewalsh3[3] = "1";
	}
	if (number1 == "2") {
		dewalsh2[0] = "-1", dewalsh2[1] = "1", dewalsh2[2] = "-1", dewalsh2[3] = "1";
	}
	if (number1 == "1") {//pretty sure processes shouldn't communicate with themselves but it's here just in case. 
		dewalsh1[0] = "-1", dewalsh1[1] = "1", dewalsh1[2] = "-1", dewalsh1[3] = "1";
	}

	if (number2 == "3") {
		dewalsh3[0] = "-1", dewalsh3[1] = "-1", dewalsh3[2] = "1", dewalsh3[3] = "1";
	}
	if (number2 == "2") {
		dewalsh2[0] = "-1", dewalsh2[1] = "-1", dewalsh2[2] = "1", dewalsh2[3] = "1";
	}
	if (number2 == "1") {
		dewalsh1[0] = "-1", dewalsh1[1] = "-1", dewalsh1[2] = "1", dewalsh1[3] = "1";
	}

	if (number3 == "3") {
		dewalsh3[0] = "-1", dewalsh3[1] = "1", dewalsh3[2] = "1", dewalsh3[3] = "-1";
	}
	if (number3 == "2") {
		dewalsh2[0] = "-1", dewalsh2[1] = "1", dewalsh2[2] = "1", dewalsh2[3] = "-1";
	}
	if (number3 == "1") {
		dewalsh1[0] = "-1", dewalsh1[1] = "1", dewalsh1[2] = "1", dewalsh1[3] = "-1";
	}
}



/*
socket code was slightly altered from the provided server.c code based on:
https://stackoverflow.com/questions/662328/what-is-a-simple-c-or-c-tcp-server-and-client-example

stringstream split code was pulled from
https://stackoverflow.com/questions/16029324/c-splitting-a-string-into-an-array

sending an array through a socket: 
https://www.geeksforgeeks.org/sort-array-using-socket-programming/


*/