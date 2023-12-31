﻿#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class DynamicCar {
private:
	int m = 300;
	float lz = 134.0;  // момент инерции машины
	double lf = 0.721, lr = 0.823; //расстояние до передней и задней осей
	int Cm = 3600;
	int Crr = 200;
	float Cd = 1.53;
	int Cbf = 5411, Cbr = 2650;
	int Cx = -20000;
	double dt = 0.001;
	const double PI = 3.141592653589793238463;

	double X; // положение центра масс
	double Y;
	double yaw;  //ориентация автомобиля в пространстве
	double vx;   //продольная скорость
	double vy;   //поперечная скорость
	double r;    //угловая скорость
public:
	DynamicCar(double x = 0, double y = 0, double angle = 0, double speed_x = 0, double speed_y = 0, double speed = 0) {
		X = x;
		Y = y;
		yaw = angle;
		vx = speed_x;
		vy = speed_y;
		r = speed;
		
	};
	double ax(double Throttle, double SteeringAngle, double brakes) {
		double Fdrv = Throttle * Cm;
		double Fbf = brakes * Cbf * tanh(vx); 
		double Fbr = brakes * Cbr * tanh(vx); 
		double Frrr = Crr * tanh(vx);
		double Frrf = Crr * tanh(vx);
		double Fdrag = Cd * vx * vx;
		double af;
		if (vx != 0) { af = atan((vy + lf * r) / vx) - SteeringAngle; } 
		else { af = 0; };
		double Ffy = Cx * af;
		double Fx = Fdrv - Fdrag - Frrr - Fbr - Frrf * cos(SteeringAngle) - Fbf * cos(SteeringAngle) - Ffy * sin(SteeringAngle) + (m * vy * r);
		return Fx / m;
	};
	double ay(double Throttle, double SteeringAngle, double brakes) {
		double af ;
		if (vx != 0) { af = atan((vy + lf * r) / vx) - SteeringAngle; }
		else { af = 0; }; 
		double ar;
		if (vx != 0) { ar = atan(((vy - lr * r) / vx)); }
		else { ar = 0; }; 
		double Ffy = Cx * af; 
		double Fry = Cx * ar; 
		double Frrf = Crr * tanh(vx);
		double Fbf = brakes * Cbf * tanh(vx);
		double Fy = Fry + Ffy * cos(SteeringAngle) - Frrf * sin(SteeringAngle) - Fbf * sin(SteeringAngle) - (m * vx * r);
		return Fy / m;
	};
	double aw(double Throttle, double SteeringAngle, double brakes) {
		double af;
		if (vx != 0) { af = atan((vy + lf * r) / vx) - SteeringAngle; }
		else { af = 0; };
		double ar;
		if (vx != 0) { ar = atan(((vy - lr * r) / vx)); }
		else { ar = 0; };
		double Ffy = Cx * af;
		double Fry = Cx * ar; 
		double M = (Ffy * lf * cos(SteeringAngle))  - (Fry * lr); 
		return M / lz;
	};
	void MoveCar(double Throttle, double SteeringAngle, double brakes) {
		double F = 0;
		X = X + dt * (vx * cos(yaw) + vy * cos(yaw + PI / 2));
		Y = Y + dt * (vx * sin(yaw) + vy * sin(yaw + PI / 2));
		yaw = yaw + dt * r;
		vx = vx + dt * ax(Throttle, SteeringAngle, brakes);
		vy = vy + dt * ay(Throttle, SteeringAngle, brakes);
		r = r + dt * aw(Throttle, SteeringAngle, brakes);
	};
	void printFields() {
		cout << "" << X << " " << Y << " " << yaw << " " << vx << " " << vy << " " << r << endl;
	};
};

int main() {
	DynamicCar MyCar(0, 0, 0, 0, 0, 0);

	ifstream file("example.txt");
	string line, MoveLine;
	getline(file, line);
	for (int i = 0; i < stoi(line); i++) {
		getline(file, MoveLine);
		stringstream ss(MoveLine);
		string s1, s2, s3;
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		getline(ss, s3, ' ');
		for (int j = 0; j < (0.05 / 0.001); j++) {
			MyCar.MoveCar(stod(s1), stod(s2), stod(s3));
		}; 
	};
	cout << endl;
	MyCar.printFields();

	file.close();

	return 0;
}
