﻿#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class KinematicCar {
private:
	double L = 1.5;
	double dt = 0.5;
public:
	double X;
	double Y;
	double yaw;
	double v;
	KinematicCar(double x = 0, double y = 0, double angle = 0,double speed = 0) {
		X = x;
		Y = y;
		yaw = angle;
		v = speed;
	};
	void MoveCar(double a, double SteeringAngle) {
		X = X + dt * v * cos(yaw);
		Y = Y + dt * v * sin(yaw);
		yaw = yaw + dt * v * tan(SteeringAngle) / L;
		v = v + dt * a;
	}
	void printFields() {
		cout << "" << X << " " << Y << " "<< yaw << " " << v << endl;
	};
};

int main() {
	KinematicCar MyCar(0, 0, 0, 0);

	ifstream file("example.txt");
	string line, MoveLine;
	getline(file, line);
	for (int i = 0; i < stoi(line); i++) {
		getline(file, MoveLine);
		stringstream ss(MoveLine);
		string s1, s2;
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		MyCar.MoveCar(stod(s1), stod(s2));
	};

	MyCar.printFields();

	file.close(); 

	return 0;
}
