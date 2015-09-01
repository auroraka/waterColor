#ifndef DEBUG_H
#define DEBUG_H

#include "include\geometry\Geometry.h"

#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;
using namespace cv;

enum DebugStatus{ Off, StdOut, FileOut };
enum DebugCommand{ NoEnter};

//use "Debug().setStatus(Off)" to close debug 
class Debug{

private:
	ostringstream sout;
	//ofstream fout;
	string fileName;
	int noEnter;
	int floatDigit;
	static DebugStatus Status;
public:


	Debug();
	~Debug();
	static void pause();
	void changeDebugFileName(string fileName);
	void setStatus(DebugStatus a);
	DebugStatus getStatus(){ return Status; }

	Debug& operator << (DebugCommand a);
	Debug& operator << (Vec3b a);
	Debug& operator << (float a);
	Debug& operator << (Geometry::Point a);
	template<typename T>
	Debug& operator << (T a){
		if (Status == Off) return *this;
		sout << a;
		return *this;
	}	
};
#endif