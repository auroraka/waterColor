
#define PolygonN 15
#include <iostream>
#include <algorithm>
using namespace std;


//double的可以保存的最大数字比long long(int64)要小很多,但是double 保存的是有效数字,并且不会算爆(只是会损失精度),比如1.23*10^100 double是可以装下的 
namespace Geometry{
	const double EPS = 1e-6;
	const double MAXV = 1e100;
	const double PI = acos(-1);
	//比较函数 
	int dcmp(const double &a, double b = 0){
		if (a - b>EPS) return 1;
		if (b - a>EPS) return -1;
		return 0;
	}
	//四舍五入到k位,因为直接用系统自带的输出时会遇到-0.00013保留2位输出-0.00的情况 
	double round(double x, int k){
		for (int i = 1; i <= k; i++) x *= 10;
		x = floor(x + 0.5);
		for (int i = 1; i <= k; i++) x /= 10;
		return x;
	}
	inline double sqr(double a){ return a*a; }
	//static char cmd[100];
	//点,向量 
	struct Point{
		double x, y;
		Point(double x = 0, double y = 0) :x(x), y(y){};
		double len()const{ return sqrt(sqr(x) + sqr(y)); }
		double dis(const Point &A)const{ return sqrt(sqr(A.x - x) + sqr(A.y - y)); }
		//返回该向量逆时针旋转angle角度后的结果
		Point rotate(const Point &angle){ return Point(x*angle.x - y*angle.y, x*angle.y + y*angle.x); }//Attention    rotate 90=rotate(Point(0,1));
		//void read(){ scanf("%lf%lf", &x, &y); }
		void print(){
			printf("%.2lf %.2lf\n", x, y);
		}
		//输出坐标不带回车 
		void printE(){
			printf("(%.2lf,%.2lf)", x, y);
		}
	};
	double det(const Point &A, const Point &B){ return A.x*B.y - A.y*B.x; }
	double dot(const Point &A, const Point &B){ return A.x*B.x + A.y*B.y; }//Atention not '-'!
	Point operator + (const Point &A, const Point &B){ return Point(A.x + B.x, A.y + B.y); }
	Point operator - (const Point &A, const Point &B){ return Point(A.x - B.x, A.y - B.y); }
	Point operator * (const double &k, const Point &A){ return Point(A.x*k, A.y*k); }
	Point operator * (const Point &A, const double &k){ return Point(A.x*k, A.y*k); }
	Point operator / (const Point &A, const double &k){ if (!k) return Point(MAXV, MAXV); return Point(A.x / k, A.y / k); }
	bool operator == (const Point &A, const Point &B){ return dcmp(A.x, B.x) == 0 && dcmp(A.y, B.y) == 0; }
	bool operator != (const Point &A, const Point &B){ return dcmp(A.x, B.x) != 0 || dcmp(A.y, B.y) != 0; }

	const Point PointZero(0, 0);
	bool pointZero(const Point &A){ return dcmp(A.x) == 0 && dcmp(A.y) == 0; }
	double sin(const Point &A, const Point &B){ if (dcmp(A.len()*B.len()) == 0) return 0; else return det(A, B) / (A.len()*B.len()); }//Attention:一锟斤拷要锟叫非凤拷锟斤拷锟斤拷 
	double cos(const Point &A, const Point &B){ if (A.len()*B.len() == 0) return 0; return dot(A, B) / (A.len()*B.len()); }
	double cross(const Point &A, const Point &B, const Point &C){ return det(A, B) + det(B, C) + det(C, A); }
	Point getAngle(const Point &A, const Point &B){ return Point(cos(A, B), sin(A, B)); }
	bool cmpAngle(const Point &A, const Point &B){ return atan2(A.x, A.y)<atan2(B.x, B.y); }
	bool cmpX(const Point &A, const Point &B){ return A.x<B.x || (A.x == B.x&&A.y<B.y); }
	Point midPoint(const Point &A, const Point &B){ return Point((A.x + B.x) / 2, (A.y + B.y) / 2); }
	double angle(const Point &A){ return atan2(A.y, A.x); }//Attention!

	double angle(const Point &A, const Point &B){ return acos(cos(A, B)); }

	double toRad(const double &a){ return a*PI / 180; }

	double toAngle(const double &a){ return a * 180 / PI; }



	/*****************线******************/


	struct Line{
		Point A, dir;
		Line(Point A = Point(0, 0), Point dir = Point(0, 0)) :A(A), dir(dir){};
		int position(const Point &P);
		double disSeg(const Point &P);
		double disLine(const Point &P);
		//void read(){ A.read(); dir.read(); dir = dir - A; }
	};
	//     ----|------>|---
	//	   1   4   3   5 2    0
	int Line::position(const Point &P){
		if (dir == Point(0, 0)) return 0;
		Point dir2(P - A);
		if (dir2 == Point(0, 0)) return 4;
		if (dcmp(det(dir, dir2)) != 0) return 0;
		if (dcmp(dot(dir, dir2))<0) return 1;
		if (dcmp(dir2.len(), dir.len())<0) return 3;
		if (dcmp(dir2.len(), dir.len())>0) return 2;
		return 5;
	}
	double Line::disLine(const Point &P){
		Point B(A + dir);
		return fabs(det(P - A, P - B)) / A.dis(B);
	}
	double Line::disSeg(const Point &P){
		Point B(A + dir);
		if (dcmp(dot(P - A, B - A)) <= 0) return P.dis(A);
		if (dcmp(dot(P - B, A - B)) <= 0) return P.dis(B);
		return fabs(det(P - A, P - B)) / A.dis(B);
	}
	bool intersectLine(const Line &la, const Line &lb, Point &P){
		Point A(la.A), A1(la.A + la.dir),
			B(lb.A), B1(lb.A + lb.dir);
		double s1 = det(B - A, B1 - A), s2 = det(B - A1, B1 - A1), s = s1 - s2;
		if (dcmp(s) == 0) return false;
		P = A + la.dir*s1 / s;
		return true;
	}
	bool between(const Point &P, const Point &A, const Point &B){ return dcmp(dot(A - P, B - P)) <= 0; }
	bool intersectSeg(const Line &la, const Line &lb, Point &P){
		Point A(la.A), A1(la.A + la.dir),
			B(lb.A), B1(lb.A + lb.dir);
		int d1 = dcmp(det(A1 - A, B - A)), d2 = dcmp(det(A1 - A, B1 - A)), d3 = dcmp(det(B1 - B, A - B)), d4 = dcmp(det(B1 - B, A1 - B));
		if ((d1^d2) == -2 && (d3^d4) == -2){
			double s1 = det(B - A, B1 - A), s2 = det(B - A1, B1 - A1);
			P = A + la.dir*s1 / (s1 - s2);
			return true;
		}
		if (!d1&&between(B, A, A1))  { P = B; return true; }
		if (!d2&&between(B1, A, A1)) { P = B1; return true; }
		if (!d3&&between(A, B, B1))  { P = A; return true; }
		if (!d4&&between(A1, B, B1)) { P = A1; return true; }
		if (!d1&&!d3) return false;
		return false;
	}
	Point projection(const Line &l, const Point &P){
		Line la(P, Point(-l.dir.y, l.dir.x)); Point A;
		intersectLine(l, la, A);
		return A;
	}



	/**************多边形****************/

	//以y为第一关键字,x为第二关键字排序 	
	bool cmpY(const Point &A, const Point &B){ return A.y<B.y || (A.y == B.y&&A.x<B.x); }
	struct Polygon{
		int n; Point a[PolygonN];
		double diameter();
		double wide();
		double MinimumAreaBoundingRectangle();
		double MinimumCircumBoundingRectangle();
		double MaximumInscribedTriangle();
		double dis(const Point &B);
		void core();
		void rotate(const Point &angle, Point P = PointZero){ for (int i = 1; i <= n; i++) a[i] = P + (a[i] - P).rotate(angle); }
		Polygon narrow(const double &x);
		//求凸包 
		void convex(){
			sort(a + 1, a + n + 1, cmpY);
			static Point d[PolygonN];
			int p1 = 0;
			for (int i = 1; i <= n; i++){
				while (p1>1 && dcmp(det(d[p1] - d[p1 - 1], a[i] - d[p1])) <= 0) p1--;
				d[++p1] = a[i];
			}
			int p2 = p1;
			for (int i = n; i >= 1; i--){
				while (p2>p1&&dcmp(det(d[p2] - d[p2 - 1], a[i] - d[p2])) <= 0) p2--;
				d[++p2] = a[i];
			}
			n = p2 - 1;
			for (int i = 1; i <= n; i++) a[i] = d[i];
		}
		//求面积 
		double getArea(){
			double area = 0;
			a[n + 1] = a[1];
			for (int i = 1; i <= n; i++) area += (a[i].x - a[i + 1].x)*(a[i].y + a[i + 1].y);
			return fabs(area)*0.5;
		}
		//求周长 
		double getCircum(){
			double cir = 0;
			a[n + 1] = a[1];
			for (int i = 1; i <= n; i++) cir += a[i].dis(a[i + 1]);
			return cir;
		}
		//求重心 
		Point getGravity(){
			Point G; double S = 0, tot = 0;
			a[n + 1] = a[1];
			for (int i = 1; i <= n; i++){
				S = det(a[i], a[i + 1]);
				G = G + S*(a[i] + a[i + 1]) / 3;
				tot += S;
			}
			G = G / tot;
			return G;
		}
		//判断点是否在多边形内 
		bool pointIn(const Point &P){
			a[n + 1] = a[1];
			for (int i = 1; i <= n; i++) if (dcmp(dot(a[i] - P, a[i + 1] - P)) <= 0 && dcmp(det(a[i] - P, a[i + 1] - P)) == 0) return true;
			int tot = 0;
			for (int i = 1; i <= n; i++){
				int k1 = dcmp(a[i].x - P.x);
				int k2 = dcmp(a[i + 1].x - P.x);
				int k3 = dcmp(det(a[i + 1] - a[i], P - a[i]));
				if (k1 <= 0 && k2>0 && k3>0) tot++;
				if (k2 <= 0 && k1>0 && k3<0) tot++;
			}
			return tot & 1;
		}


	};



	/**************旋转卡壳****************/

	//凸包直径
	double Polygon::diameter(){
		if (n<3) return a[1].dis(a[n]);
		double ans = 0;
		a[n + 1] = a[1];
		int p = 2;//Attention:p=2!
		for (int i = 1; i <= n; i++){
			while (dcmp(cross(a[i], a[i + 1], a[p]), cross(a[i], a[i + 1], a[p + 1]))<0) p = p%n + 1;//Attention!!!
			ans = max(ans, a[i].dis(a[p]));
		}
		return ans;
	}
	//凸包宽
	double Polygon::wide(){
		if (n<3) return 0;//Attention!!!
		int t = 2; double ans = MAXV; a[n + 1] = a[1];
		for (int i = 1; i <= n; i++){
			while (dcmp(cross(a[i], a[i + 1], a[t]), cross(a[i], a[i + 1], a[t + 1]))<0) t = t%n + 1;//Attention!!!
			ans = min(ans, Line(a[i], a[i + 1] - a[i]).disLine(a[t]));
		}
		return ans;
	}
	//最小面积外接矩形
#define MABR MinimumAreaBoundingRectangle
	double Polygon::MinimumAreaBoundingRectangle(){
		if (n<3) return 0;//Attention!!!
		int t1 = 2, t2 = 2, t3 = 1;
		Point P1, P2, P3; Line l;
		double area = MAXV, x, y;
		a[n + 1] = a[1];
		while (dcmp(dot(a[t3 + 1] - a[1], a[2] - a[1]), dot(a[t3] - a[1], a[2] - a[1])) >= 0) t3 = t3%n + 1;
		for (int i = 1; i <= n; i++){
			while (dcmp(cross(a[i], a[i + 1], a[t2]), cross(a[i], a[i + 1], a[t2 + 1]))<0) t2 = t2%n + 1;//Attention!!!
			while (dcmp(dot(a[t1 + 1] - a[i], a[i + 1] - a[i]), dot(a[t1] - a[i], a[i + 1] - a[i]))>0) t1 = t1%n + 1;
			while (dcmp(dot(a[t3 + 1] - a[i], a[i + 1] - a[i]), dot(a[t3] - a[i], a[i + 1] - a[i]))<0) t3 = t3%n + 1;
			l = Line(a[i], a[i + 1] - a[i]);
			P1 = projection(l, a[t1]);
			P2 = projection(l, a[t2]);
			P3 = projection(l, a[t3]);
			x = P2.dis(a[t2]);
			y = P1.dis(P3);
			area = min(area, x*y);
		}
		return area;
	}
	//最小周长外接矩形
#define MCBR MinimumCircumBoundingRectangle
	double Polygon::MinimumCircumBoundingRectangle(){
		if (n<3) return 0;
		double ans = MAXV;
		a[n + 1] = a[1];
		int p1 = 2, p2 = 2, p3 = 2;
		while (dcmp(dot(a[2] - a[1], a[p3] - a[1]), dot(a[2] - a[1], a[p3 + 1] - a[1])) <= 0) p3 = p3%n + 1;
		for (int i = 1; i <= n; i++){
			while (dcmp(dot(a[i + 1] - a[i], a[p1] - a[i]), dot(a[i + 1] - a[i], a[p1 + 1] - a[i]))<0) p1 = p1%n + 1;
			while (dcmp(det(a[i + 1] - a[i], a[p2] - a[i]), det(a[i + 1] - a[i], a[p2 + 1] - a[i]))<0) p2 = p2%n + 1;
			while (dcmp(dot(a[i + 1] - a[i], a[p3] - a[i]), dot(a[i + 1] - a[i], a[p3 + 1] - a[i]))>0) p3 = p3%n + 1;
			Line l(a[i], a[i + 1] - a[i]);
			Point A = projection(l, a[p1]), B = projection(l, a[p2]), C = projection(l, a[p3]);
			double x = A.dis(C), y = B.dis(a[p2]);
			ans = min(ans, x + x + y + y);
		}
		return ans;
	}
	//凸包最小距离
	double MINdis(Polygon &a, Polygon &b){
		a.a[a.n + 1] = a.a[1]; b.a[b.n + 1] = b.a[1];
		int t = 1; double ans = MAXV;
		for (int i = 1; i <= a.n; i++){
			Line l(a.a[i], a.a[i + 1] - a.a[i]);
			while (dcmp(det(l.dir, b.a[t] - a.a[i]), det(l.dir, b.a[t + 1] - a.a[i]))<0) t = t%b.n + 1;
			ans = min(ans, min(l.disSeg(b.a[t]), l.disSeg(b.a[t + 1])));//Attention!	
		}
		return ans;
	}
	double disMin(Polygon &a, Polygon &b){ return min(MINdis(a, b), MINdis(b, a)); }

	//凸包最大距离
	double MAXdis(Polygon &a, Polygon &b){
		a.a[a.n + 1] = a.a[1]; b.a[b.n + 1] = b.a[1];
		int t = 1; double ans = 0;
		for (int i = 1; i <= a.n; i++){
			Line l(a.a[i], a.a[i + 1] - a.a[i]);
			while (dcmp(det(l.dir, b.a[t] - a.a[i]), det(l.dir, b.a[t + 1] - a.a[i]))<0) t = t%b.n + 1;
			ans = max(ans, a.a[i].dis(b.a[t])); ans = max(ans, a.a[i].dis(b.a[t + 1]));
		}
		return ans;
	}
	double disMax(Polygon &a, Polygon &b){ return max(MAXdis(a, b), MAXdis(b, a)); }

	/**************半平面交*******************/

	//直线切多边形
	void cut(Polygon &a, const Line &l){
		//EPS=1e-4;
		static Polygon b; b.n = 0; Point P; a.a[a.n + 1] = a.a[1];//Attention!
		for (int i = 1; i <= a.n; i++){
			if (dcmp(det(l.dir, a.a[i] - l.A)) >= 0) b.a[++b.n] = a.a[i];
			Line la(a.a[i], a.a[i + 1] - a.a[i]);
			if (intersectLine(la, l, P) && between(P, a.a[i], a.a[i + 1])) b.a[++b.n] = P;//Important Attention:intersect()要锟斤拷凸锟斤拷锟较碉拷锟斤拷锟斤拷锟叫革拷锟竭ｏ拷锟斤拷锟斤拷锟叫撅拷锟斤拷锟斤拷锟斤拷! 
		}
		a.n = b.n; for (int i = 1; i <= b.n; i++) a.a[i] = b.a[i];
	}
	//半平面交NlogN
	//dcmp()>0 当半平面交交成一个点的时候无法算出-->判定是否有交会出问题 
	//dcmp()>=0 会有重复的顶点-->算面积不会有问题 
	bool cmpLine(const Line &la, const Line &lb){ return dcmp(atan2(la.dir.y, la.dir.x), atan2(lb.dir.y, lb.dir.x))<0; }
	bool onLeft(const Point &P, const Line &l){
		return dcmp(det(l.dir, P - l.A)) >= 0;
	}
	void HalfplaneIntersect(Line l[], int n, Polygon &a){
		static Point P[PolygonN];
		static Line Q[PolygonN];
		sort(l + 1, l + n + 1, cmpLine);//Attention!
		Q[1] = l[1]; int p1 = 1, p2 = 1;
		for (int i = 2; i <= n; i++){
			while (p1<p2&&!onLeft(P[p2 - 1], l[i])) p2--;
			while (p1<p2&&!onLeft(P[p1], l[i])) p1++;
			Q[++p2] = l[i];
			if (dcmp(det(Q[p2].dir, Q[p2 - 1].dir)) == 0){
				if (onLeft(l[i].A, Q[p2 - 1])) Q[p2 - 1] = l[i];
				p2--;
			}
			if (p1<p2) intersectLine(Q[p2], Q[p2 - 1], P[p2 - 1]);
		}
		while (p1<p2&&!onLeft(P[p2 - 1], Q[p1])) p2--;
		if (p1<p2) intersectLine(Q[p2], Q[p1], P[p2]);
		a.n = 0;
		if (p2 - p1 <= 1) return;
		for (int i = p1; i <= p2; i++) a.a[++a.n] = P[i];
	}
	//求核NlogN 
	void core(Polygon &a){
		static Line l[PolygonN]; a.a[a.n + 1] = a.a[1];
		for (int i = 1; i <= a.n; i++) l[i] = Line(a.a[i], a.a[i + 1] - a.a[i]);
		HalfplaneIntersect(l, a.n, a);
	}
	//内缩NlogN 
	void narrow(Polygon &a, double x, Polygon &b){
		static Line l[PolygonN];
		a.a[a.n + 1] = a.a[1];
		for (int i = 1; i <= a.n; i++){
			l[i] = Line(a.a[i], a.a[i + 1] - a.a[i]);
			l[i].A = l[i].A + l[i].dir.rotate(Point(0, 1))*x / l[i].dir.len();
		}
		HalfplaneIntersect(l, a.n, b);
	}
	/**********************************/

	//最大内接三角形 
#define MIT MaximumInscribedTriangle 
	double Polygon::MaximumInscribedTriangle(){
		a[n + 1] = a[1]; double area = 0;
		int p = 2;
		for (int i = 1; i <= n + 1; i++){
			while (p <= n&&dcmp(det(a[i + 1] - a[i], a[p] - a[i]), det(a[i + 1] - a[i], a[p + 1] - a[i]))<0) p++;
			area = max(area, det(a[i + 1] - a[i], a[p] - a[i]));
			int k = p;
			for (int j = i + 2; j <= n + 1; j++){
				while (k <= n&&dcmp(det(a[j] - a[i], a[k] - a[i]), det(a[j] - a[i], a[k + 1] - a[i]))<0) k++;
				area = max(area, det(a[j] - a[i], a[k] - a[i]));
			}
		}
		return area*0.5;
	}
	//点到多边形距离(点在多边形外)
	double Polygon::dis(const Point &P){
		double ans = 0x7fffffff; a[n + 1] = a[1];
		for (int i = 1; i <= n; i++) ans = min(ans, Line(a[i], a[i + 1] - a[i]).disSeg(P));//Attention!
		return ans;
	}
	//三角形外心 
	Point circumCenter(const Point &A, const Point &B, const Point &C){
		Point D = midPoint(A, B), E = midPoint(B, C), P;
		Line la(D, (B - A).rotate(Point(0, 1)));
		Line lb(E, (C - B).rotate(Point(0, 1)));
		intersectLine(la, lb, P);
		return P;
	}
	//三角形内心 
	Point innerCenter(const Point &A, const Point &B, const Point &C){
		double a = B.dis(C), b = C.dis(A), c = A.dis(B);
		Point P = (A*a + B*b + C*c) / (a + b + c);
		return P;
	}
	/*Point innerCenter(const Point &A,const Point &B,const Point &C){
	double a;Point P;
	a=angle(B-A,C-A)/2;
	Line la(A,(B-A).rotate(a));
	a=angle(C-B,A-B)/2;
	Line lb(B,(C-B).rotate(a));
	intersectLine(la,lb,P);
	return P;
	}*/


	/*****************圆******************/
	//圆 
	struct Circle{
		Point O; double r;
		Circle(Point O = Point(0, 0), double r = 0) :O(O), r(r){};
		Point point(double a)const{ return Point(O.x + r*std::cos(a), O.y + r*std::sin(a)); }
		bool circleIn(const Point &A){ return dcmp(A.dis(O), r)<0; }//Attention::锟较革拷锟斤拷锟斤拷 
		double sectorArea(const Point &A, const Point &B)const;
		double intersectArea(const Point &A, const Point &B);
		double intersectArea(Polygon &a);
		//void read(){ scanf("%lf%lf%lf\n", &O.x, &O.y, &r); }
		void print(){ printf("%.2lf %.2lf %.2lf\n", O.x, O.y, r); }
	};
	//圆锟斤拷圆锟斤拷锟斤拷 
	void intersect(const Circle &c1, const Circle &c2, Point v[], int &tot){
		if (pointZero(c1.O - c2.O) && dcmp(c1.r, c2.r) == 0) { tot = -1; return; }
		double d = c1.O.dis(c2.O);
		if (dcmp(c1.r + c2.r, d)<0) return;
		if (dcmp(fabs(c1.r - c2.r), d)>0) return;
		double a = angle(c2.O - c1.O);
		double da = acos((sqr(c1.r) + sqr(d) - sqr(c2.r)) / (2 * c1.r*d));
		v[++tot] = c1.point(a + da); v[++tot] = c1.point(a - da);
		if (pointZero(v[tot] - v[tot - 1])) tot--;
	}
	//锟斤拷锟斤拷锟斤拷圆锟斤拷锟斤拷(l.A锟斤拷圆锟斤拷) 
	void intersectRay(const Circle &c, const Line &l, Point &A){
		Point P = projection(l, c.O);
		double d = P.dis(c.O), x = sqrt(sqr(c.r) - sqr(d));
		A = P + l.dir*x / l.dir.len();
	}
	//圆锟斤拷直锟竭斤拷锟斤拷 
	void intersectLine(const Circle &c, const Line &l, Point v[], int &tot){//Attenrion::&tot
		Point P = projection(l, c.O);
		if (pointZero(P - c.O)){

			Point dir = l.dir*c.r / l.dir.len();
			v[++tot] = P + dir;
			v[++tot] = P - dir;
			return;
		}
		double d = P.dis(c.O);
		if (dcmp(d, c.r)>0) return;
		if (dcmp(d, c.r) == 0) { v[++tot] = P; return; }
		double a = angle(P - c.O), da = acos(d / c.r);
		v[++tot] = c.point(a + da);
		v[++tot] = c.point(a - da);
	}
	//锟斤拷锟斤拷锟斤拷锟斤拷	
	double Circle::sectorArea(const Point &A, const Point &B)const{ return angle(A - O, B - O)*sqr(r) / 2; }
	//圆锟诫顶锟斤拷锟斤拷圆锟侥碉拷锟斤拷锟斤拷锟轿斤拷(锟斤拷锟斤拷锟斤拷锟斤拷) 
	double Circle::intersectArea(const Point &A1, const Point &B1){
		Point A(A1), B(B1); Circle c(O, r);
		int t = dcmp(cross(O, A, B)), t1 = circleIn(A), t2 = circleIn(B);
		if (!t) return 0;
		if (t1&&t2) return cross(O, A, B)*0.5;
		if ((!t1) && (!t2)){
			double ans = sectorArea(A, B);
			Line l(A, B - A);//Point P=projection(l,O);(锟斤拷锟斤拷锟斤拷锟筋，锟斤拷锟斤拷锟斤拷)
			if (dcmp(l.disSeg(O), r)<0){
				Point v[3]; int tot = 0;
				intersectLine(c, l, v, tot);
				ans = ans - sectorArea(v[1], v[2]) + fabs(cross(O, v[1], v[2]))*0.5;
			}
			return ans*t;
		}
		if (t2) swap(A, B);
		Point C, D;
		intersectRay(c, Line(A, B - A), C);
		intersectRay(c, Line(O, B - O), D);
		double ans = fabs(cross(O, A, C))*0.5 + sectorArea(C, D);
		return ans*t;
	}
	//圆锟斤拷锟斤拷锟斤拷锟轿斤拷锟斤拷锟斤拷锟斤拷 
	double Circle::intersectArea(Polygon &a){
		a.a[a.n + 1] = a.a[1]; double ans = 0;
		for (int i = 1; i <= a.n; i++) ans += intersectArea(a.a[i], a.a[i + 1]);
		return fabs(ans);
	}
	//圆锟斤拷圆锟斤拷锟斤拷锟斤拷锟斤拷 
	double intersectArea(Circle &c1, Circle &c2){
		if (c1.r<c2.r) swap(c1, c2);
		double d = c1.O.dis(c2.O);
		if (dcmp(c1.r + c2.r, d)<0) return 0;
		if (dcmp(c1.r - c2.r, d) >= 0) return PI*sqr(c2.r);
		//double x1=c1.O.x,y1=c1.O.y,r1=c1.r,
		//       x2=c2.O.x,y2=c2.O.y,r2=c2.r;//???
		double a1 = 2 * acos((sqr(c1.r) + sqr(d) - sqr(c2.r)) / (2 * c1.r*d)), a2 = 2 * acos((sqr(c2.r) + sqr(d) - sqr(c1.r)) / (2 * c2.r*d));
		return a1*sqr(c1.r)*0.5 + a2*sqr(c2.r)*0.5 - std::sin(a1)*sqr(c1.r)*0.5 - std::sin(a2)*sqr(c2.r)*0.5;
	}
	//锟斤拷锟斤拷锟斤拷锟斤拷圆锟斤拷锟斤拷锟斤拷(锟斤拷锟斤拷锟斤拷锟竭凤拷锟斤拷锟斤拷锟? 
	void Tangent(const Circle &c, const Point &P, Point v[], int &tot){
		Point A = c.O - P;
		double d = A.len();
		if (dcmp(c.r, d)>0) return;
		if (dcmp(c.r, d) == 0){ v[++tot] = A.rotate(PI / 2); return; }
		double a = asin(c.r / d);
		v[++tot] = A.rotate(a);
		v[++tot] = A.rotate(-a);
	}
	//???
	//锟皆诧拷锟斤拷锟?锟斤拷锟斤拷锟叫碉拷) 
	void Tangent(Circle &c1, Circle &c2, Point v1[], Point v2[], int &tot){
		if (pointZero(c1.O - c2.O) && dcmp(c1.r, c2.r) == 0) { tot = -1; return; }
		int t = 0;
		if (c1.r<c2.r){ swap(c1, c2); swap(v1, v2); t = 1; }
		double d = c1.O.dis(c2.O);
		if (dcmp(c1.r - c2.r, d)>0) return;
		double a = angle(c2.O - c1.O);
		if (dcmp(c1.r - c2.r, d) == 0){
			v1[++tot] = c1.point(a); v2[tot] = c2.point(a); return;
		}
		double da = acos((c1.r - c2.r) / d);
		v1[++tot] = c1.point(a + da); v2[tot] = c2.point(a + da);
		v1[++tot] = c1.point(a - da); v2[tot] = c2.point(a - da);
		if (dcmp(c1.r + c2.r, d) == 0){
			v1[++tot] = c1.point(a); v2[tot] = c2.point(PI + a);
		}
		if (dcmp(c1.r + c2.r, d)<0){
			double da = acos((c1.r + c2.r) / d);
			v1[++tot] = c1.point(a + da); v2[tot] = c2.point(PI + a + da);
			v1[++tot] = c1.point(a - da); v2[tot] = c2.point(PI + a - da);
		}
		if (t){ swap(c1, c2); swap(v1, v2); }
		return;
	}
	//锟斤拷小圆锟斤拷锟斤拷[锟斤拷锟斤拷锟斤拷锟絔 
	void MinimumCircle(Point a[], int n, Circle &c){
		random_shuffle(a + 1, a + n + 1);
		Point O = a[1]; double r = 0;
		for (int i = 2; i <= n; i++){
			if (dcmp(O.dis(a[i]), r) <= 0) continue;
			O = a[i]; r = 0;
			for (int j = 1; j<i; j++){
				if (dcmp(O.dis(a[j]), r) <= 0) continue;
				O = midPoint(a[i], a[j]);
				r = O.dis(a[i]);
				for (int k = 1; k<j; k++){
					if (dcmp(O.dis(a[k]), r) <= 0) continue;
					O = circumCenter(a[i], a[j], a[k]);
					r = O.dis(a[i]);
				}
			}
		}
		c = Circle(O, r);
	}
}

