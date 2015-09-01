#include "Geometry.h"

//double的可以保存的最大数字比long long(int64)要小很多,但是double 保存的是有效数字,并且不会算爆(只是会损失精度),比如1.23*10^100 double是可以装下的 
	const double EPS=1e-6;
	const double MAXV=1e100;
	const double PI=acos(-1);
	//比较函数 
	int Geometry::dcmp(const double &a,double b){
		if (a-b>EPS) return 1;
		if (b-a>EPS) return -1;
		return 0;
	}
	//四舍五入到k位,因为直接用系统自带的输出时会遇到-0.00013保留2位输出-0.00的情况 
	double Geometry::round(double x, int k){
		for (int i=1;i<=k;i++) x*=10;
		x=floor(x+0.5);
		for (int i=1;i<=k;i++) x/=10;
		return x;
	}

	 

/*****************线******************/
	 
	//     ----|------>|---
	//	   1   4   3   5 2    0
	int Geometry::Line::position(const Point &P){
		if (dir == Point(0, 0)) return 0;
		Point dir2(P - A);
		if (dir2 == Point(0, 0)) return 4;
		if (dcmp(det(dir, dir2)) != 0) return 0;
		if (dcmp(dot(dir, dir2))<0) return 1;
		if (dcmp(dir2.len(), dir.len())<0) return 3;
		if (dcmp(dir2.len(), dir.len())>0) return 2;
		return 5;
	}

 	
	bool Geometry::intersectLine(const Line &la, const Line &lb, Point &P){
		Point A(la.A),A1(la.A+la.dir),
        	  B(lb.A),B1(lb.A+lb.dir);
		double s1=det(B-A,B1-A),s2=det(B-A1,B1-A1),s=s1-s2;
  		if (dcmp(s)==0) return false;
		P=A+la.dir*s1/s;
		return true;
	}
	
	
	