#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void error(const char *msg);
int decode(vector<int> signal, string dewalsh[]);

int main(int argc, char *argv[])
{

	/*
	Files were compiled with the lines 	g++ -std=c++11 3360A2Client.cpp -o ctest	and	g++ -std=c++11 3360A2Server.cpp -o stest	on the server. 
	Also the Server file MUST be initiated first, 
	else the files don't communicate.
	*/

	int sockfd, sockfd2, sockfd3, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256], buffer1[256], buffer2[256], buffer3[256];
	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	//cout << argc << " " << argv[0] << " " << argv[1] << " " << argv[2] << endl;
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket 1");
	if (sockfd2 < 0)
		error("ERROR opening socket 2");
	if (sockfd3 < 0)
		error("ERROR opening socket 3");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	//bzero(buffer, 256);
	//fgets(buffer, 255, stdin);
	bzero(buffer1, 256);
	fgets(buffer1, 255, stdin);
	bzero(buffer2, 256);
	fgets(buffer2, 255, stdin);
	bzero(buffer3, 256);
	fgets(buffer3, 255, stdin);
	/*
	the bzero and fgets lines were moved up here
	so that that the file has already been read in
	when the processes begin to fork.
	so that way, things don't get messed up.
	*/

	pid_t pid1, pid2;
	//First child:
	pid1 = fork();

	//creates a second child and a grandchild to the first
	pid2 = fork();



	////////////////SOCKET 1 and CHILD 1///////////////
	if (pid1 == 0 && pid2 > 0) {
		if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
			error("ERROR connecting 1");
		//cout << "Sending integer 1: " << endl;
		//bzero(buffer, 256);
		//fgets(buffer, 255, stdin);

		string set1 = std::string(buffer1), p1info, num1;
		int i1 = 0, counter1 = 0;
		stringstream ssin1(set1);//this block is repeated for each socket, it should be function, but the function didn't work right away so I moved on
		while (ssin1.good() && i1 < 2) {//did not end up actually needing to split the input like this
			if (counter1 % 2 == 0) {//but it worked for outputting what was being sent where so I kept it. 
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
		cout << "Child 1, sending value: " << num1 << " to child process " << p1info << endl;
		n = write(sockfd, buffer1, strlen(buffer1));
		//cout << buffer << endl;
		if (n < 0)
			error("ERROR writing to socket 1");
		sleep(1);
	}//many sleep(1)'s are placed throughout the program to control the forked processes. 
	//I assume there is a better way of controlling the forked processes, however this is what worked here.



	////////////////SOCKET 2 and CHILD 2///////////////
	if (pid1 > 0 && pid2 == 0) {
		sleep(1);
		if (connect(sockfd2, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
			error("ERROR connecting 2");
		//printf("Please enter the message 2: ");
		//bzero(buffer, 256);
		//fgets(buffer, 255, stdin);

		string set2 = std::string(buffer2), p2info, num2;
		int i2 = 0, counter2 = 0;
		stringstream ssin2(set2);//this block is repeated for each socket, it should be function, but the function didn't work right away so I moved on
		while (ssin2.good() && i2 < 2) {
			if (counter2 % 2 == 0) {
				ssin2 >> p2info;
				//cout << "Process " << p1info << " is receiving: ";
			}
			else {

				ssin2 >> num2;
				//cout << num1 << endl;
			}
			counter2++;
			++i2;
		}
		cout << "Child 2, sending value: " << num2 << " to child process " << p2info << endl;
		n = write(sockfd2, buffer2, strlen(buffer2));
		//cout << buffer << endl;
		if (n < 0)
			error("ERROR writing to socket 2");
		sleep(1);
		sleep(1);
	}



	/////////////////SOCKET 3 and CHILD 3/////////
	if (pid1 == 0 && pid2 > 0) {
		sleep(1);
		if (connect(sockfd3, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
			error("ERROR connecting 3");
		//printf("Please enter the message 3: ");
		//bzero(buffer, 256);
		//fgets(buffer, 255, stdin);

		string set3 = std::string(buffer3), p3info, num3;
		int i3 = 0, counter3 = 0;
		stringstream ssin3(set3);//this block is repeated for each socket, it should be function, but the function didn't work right away so I moved on
		while (ssin3.good() && i3 < 2) {
			if (counter3 % 2 == 0) {
				ssin3 >> p3info;
				//cout << "Process " << p1info << " is receiving: ";
			}
			else {

				ssin3 >> num3;
				//cout << num1 << endl;
			}
			counter3++;
			++i3;
		}
		cout << "Child 3, sending value: " << num3 << " to child process " << p3info << endl;
		n = write(sockfd3, buffer3, strlen(buffer3));
		if (n < 0)
			error("ERROR writing to socket 3");
		sleep(1);
		sleep(1);
	}

	/*trying to control how the child processes go into the receiving phase 
	this definitely isn't the best way to do this but it works here.
	Another negative is that this would likely mess things up if you paired my 
	client/server with someone else's client/server. 
	*/
	if (pid1 == 0 && pid2 > 0) {
		//first child
		//sleep(1);
	}
	if (pid1 > 0 && pid2 == 0) {
		//second child 
		sleep(1);
		sleep(1);
	}
	if (pid1 == 0 && pid2 == 0) {
		//third child
		sleep(1);
		sleep(1);
		sleep(1);
		sleep(1);
	}
	/*
	//these blocks of sleeps here are to control specifically the output of each forked process showing the signal,
	walsh decode code and answer. Without them, the output would get mixed up very easily. 
	*/

	int i = 0;
	int pSignal[12];
	int pDeWalsh1[4], pDeWalsh2[4], pDeWalsh3[4];

	//CHILD 1 RECEIVE
	if (pid1 == 0 && pid2 > 0) {
		/*
		bzero(buffer, 256);
		n = read(sockfd, buffer, 255);
		if (n < 0)
		error("ERROR reading from socket 1");
		printf("%s\n", buffer);
		close(sockfd);
		sleep(1);
		*/
		int signal[16];
		int deWalsh1i[4];
		int read1;
		i = 0;
		while ((read1 = recv(sockfd, &signal[i], 16 * sizeof(int), 0) > 0)) {
			//cout << signal[i] <<" ";
			i++;
		}

		//cout << endl;

		//cout << "SIGNAL 1: " << endl;
		for (int i = 0; i < 12; i++) {
			//cout << signal[i] << " ";
			pSignal[i] = signal[i];
		}
		//cout << endl;
		//cout << "DEWALSH 1: " << endl;
		for (int i = 0; i < 4; i++) {
			deWalsh1i[i] = signal[i + 12];
			pDeWalsh1[i] = deWalsh1i[i];
			//cout << deWalsh1i[i] << " ";
		}
		//cout << endl;

		/*
		bzero(buffer, 256);
		n = read(sockfd2, buffer, 255);
		if (n < 0)
		error("ERROR reading from socket 2");
		printf("%s\n", buffer);
		close(sockfd2);
		sleep(1);
		*/

		vector<int> intSignal;
		for (int i = 0; i < 12; i++) {
			intSignal.push_back(signal[i]);
			//cout << intSignal.at(i)<<" ";
		}
		string deWalsh1[4];
		for (int i = 0; i < 4; i++) {
			deWalsh1[i] = std::to_string(deWalsh1i[i]);
		}

		int answer1 = decode(intSignal, deWalsh1);

		cout << endl << "Child 1" << endl;
		cout << "Signal: ";
		for (int i = 0; i < 12; i++) {
			cout << intSignal.at(i) << " ";
		}
		cout << endl << "Code: ";
		for (int i = 0; i < 4; i++) {
			cout << deWalsh1[i] << " ";
		}
		cout << endl << "Received value = " << answer1 << endl;

		exit(0);
	}
	if (pid1 > 0 && pid2 > 0) {
		wait(NULL);
	}
	

	//CHILD 2 RECEIVE 
	if (pid1 > 0 && pid2 == 0) {

		sleep(1);

		int signal2[16];
		int deWalsh2i[4];
		int read2;
		i = 0;
		while ((read2 = recv(sockfd2, &signal2[i], 16 * sizeof(int), 0) > 0)) {
			//cout << signal2[i] << " ";
			i++;
		}
		//cout << endl;

		//cout << "SIGNAL 2: " << endl;
		for (int i = 0; i < 12; i++) {
			//cout << signal2[i] << " ";
		}
		//cout << endl;
		//cout << "DEWALSH 2: " << endl;
		for (int i = 0; i < 4; i++) {
			deWalsh2i[i] = signal2[i + 12];
			pDeWalsh2[i] = deWalsh2i[i];
			//cout << deWalsh2i[i] << " ";
		}
		//cout << endl;

		/*
		bzero(buffer, 256);
		n = read(sockfd3, buffer, 255);
		if (n < 0)
		error("ERROR reading from socket 3");
		printf("%s\n", buffer);
		close(sockfd3);
		sleep(1);
		*/

		vector<int> intSignal;
		for (int i = 0; i < 12; i++) {
			intSignal.push_back(signal2[i]);
			//cout << intSignal.at(i)<<" ";
		}//child1 done here

		string deWalsh2[4];
		for (int i = 0; i < 4; i++) {
			deWalsh2[i] = std::to_string(deWalsh2i[i]);
		}
		int answer2 = decode(intSignal, deWalsh2);

		cout << endl << "Child 2" << endl;
		cout << "Signal: ";
		for (int i = 0; i < 12; i++) {
			cout << intSignal.at(i) << " ";
		}
		cout << endl << "Code: ";
		for (int i = 0; i < 4; i++) {
			cout << deWalsh2[i] << " ";
		}
		cout << endl << "Received value = " << answer2 << endl;

		exit(0);
	}
	if (pid1 > 0 && pid2 > 0) {
		wait(NULL);
	}
	sleep(1);

	if (pid1 == 0 && pid2 == 0) {

		sleep(1);
		sleep(1);

		int signal3[16];
		int deWalsh3i[4];
		int read3;
		i = 0;
		while ((read3 = recv(sockfd3, &signal3[i], 16 * sizeof(int), 0) > 0)) {
			//cout << signal3[i] << " ";
			i++;
		}
		//cout << endl;

		//cout << "SIGNAL 3: " << endl;
		for (int i = 0; i < 12; i++) {
			//cout << signal3[i] << " ";
		}
		//cout << endl;
		//cout << "DEWALSH 3: " << endl;
		for (int i = 0; i < 4; i++) {
			deWalsh3i[i] = signal3[i + 12];
			pDeWalsh3[i] = deWalsh3i[i];
			//cout << deWalsh3i[i] << " ";
		}
		//cout << endl;

		vector<int> intSignal;
		for (int i = 0; i < 12; i++) {
			intSignal.push_back(signal3[i]);
			//cout << intSignal.at(i)<<" ";
		}//child1 done here

		string deWalsh3[4];
		for (int i = 0; i < 4; i++) {

			deWalsh3[i] = std::to_string(deWalsh3i[i]);
		}

		int answer3 = decode(intSignal, deWalsh3);

		cout << endl << "Child 3" << endl;
		cout << "Signal: ";
		for (int i = 0; i < 12; i++) {
			cout << intSignal.at(i) << " ";
		}
		cout << endl << "Code: ";
		for (int i = 0; i < 4; i++) {
			cout << deWalsh3[i] << " ";
		}
		cout << endl << "Received value = " << answer3 << endl;

		exit(0);
	}
	if (pid1 > 0 && pid2 > 0) {
		wait(NULL);
	}

	close(sockfd);
	close(sockfd2);
	close(sockfd3);

	return 0;
}

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int decode(vector<int> signal, string dewalsh[]) {

	int answer = 0;
	int binRes1[4], binRes2[4], binRes3[4];//binary result

	for (int i = 0; i < 4; i++) {
		//cout << signal.at(i) << " x " << dewalsh[i] << endl;
		binRes1[i] = signal.at(i) * stoi(dewalsh[i]);
	}
	for (int i = 0; i < 4; i++) {
		//cout << signal.at(i+4) << " x " << dewalsh[i] << endl;
		binRes2[i] = signal.at(i + 4) * stoi(dewalsh[i]);
	}
	for (int i = 0; i < 4; i++) {
		//cout << signal.at(i+8) << " x " << dewalsh[i] << endl;
		binRes3[i] = signal.at(i + 8)*stoi(dewalsh[i]);
	}

	int answer1 = 0, answer2 = 0, answer3 = 0, add = 0;
	//cout << endl;
	for (int i = 0; i < 4; i++) {
		//cout << binRes1[i] << " ";
		add = binRes1[i] + add;
	}
	//cout << "Add: " << add << endl;
	answer1 = add;
	add = 0;

	for (int i = 0; i < 4; i++) {
		//cout << binRes2[i] << " ";
		add = binRes2[i] + add;
	}
	//cout << "Add: " << add << endl;
	answer2 = add;
	add = 0;

	for (int i = 0; i < 4; i++) {
		//cout << binRes3[i] << " ";
		add = binRes3[i] + add;
	}
	//cout << "Add: " << add << endl;
	answer3 = add;
	add = 0;
	//cout << endl;

	//at this point it *probably* should have been another function
	answer1 = answer1 / 4;
	answer2 = answer2 / 4;
	answer3 = answer3 / 4;

	if (answer1 == -1 && answer2 == -1 && answer3 == -1) {//-1 -1 -1 = 0
		answer = 0;
	}
	if (answer1 == -1 && answer2 == -1 && answer3 == 1) {//-1 -1 1 = 1
		answer = 1;
	}
	if (answer1 == -1 && answer2 == 1 && answer3 == -1) {//-1 1 -1 = 2
		answer = 2;
	}
	if (answer1 == -1 && answer2 == 1 && answer3 == 1) {//-1 1 1 = 3
		answer = 3;
	}
	if (answer1 == 1 && answer2 == -1 && answer3 == -1) {//1 -1 -1 = 4
		answer = 4;
	}
	if (answer1 == 1 && answer2 == -1 && answer3 == 1) {//1 -1 1 = 5
		answer = 5;
	}
	if (answer1 == 1 && answer2 == 1 && answer3 == -1) {//1 1 -1 = 6
		answer = 6;
	}
	if (answer1 == 1 && answer2 == 1 && answer3 == 1) {//1 1 1 = 7
		answer = 7;
	}

	//cout << "Answer: " << answer << endl;
	return answer;
}

/*
socket code was slightly altered from the provided client.c code based on:
https://stackoverflow.com/questions/662328/what-is-a-simple-c-or-c-tcp-server-and-client-example

stringstream split code was pulled from
https://stackoverflow.com/questions/16029324/c-splitting-a-string-into-an-array

sending an array through a socket:
https://www.geeksforgeeks.org/sort-array-using-socket-programming/

help creating multiple forks so I don't break something with the server on accident:
https://www.geeksforgeeks.org/creating-multiple-process-using-fork/
*/