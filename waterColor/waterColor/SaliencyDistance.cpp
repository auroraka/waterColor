#include "SaliencyDistance.h"
#include "ToolBox.h"
#include "Debug.h"
using namespace cv;


void SaliencyDistance::saliency(Mat &src,Mat &mySaliency){
	string inputDir = "D:\\saliencyTemp\\in\\";
	string outputDir = "D:\\saliencyTemp\\out\\";
	//string inputDir = "./";
	//string outputDir = "./";
	char cmd[300];
	sprintf(cmd, "del %s && del %s", (outputDir + "saliency_RCC.png").c_str(), (outputDir + "saliency_RC.png").c_str());
	system(cmd);
	system("mkdir D:\\saliencyTemp\\in");
	imwrite(string(inputDir) + "saliency.jpg", src);
	system("cd include/saliency && Saliency.exe >> nul");
	
	mySaliency = imread(outputDir+"saliency_RCC.png",CV_LOAD_IMAGE_GRAYSCALE);
}

void SaliencyDistance::jumpFlooding(Mat &saliency, Mat &jump, Geometry::Point** ancestor){
	
	//cout <<"saliency channels :"<< saliency.channels()<<endl;

	int **visit = ArraySpace::newArray<int>(saliency.rows, saliency.cols);
	myPoint *Q = new myPoint[saliency.rows*saliency.cols+5];
	int p2 = 0;
	for (int i = 0; i < saliency.rows; i++){
		for (int j = 0; j < saliency.cols; j++){
			if (dataAt<uchar>(saliency, i, j)>128){
				dataAt<int>(jump, i, j) = 0;
				Q[++p2] = myPoint(i, j);
				ancestor[i][j]=Geometry::Point(i, j);
				visit[i][j] = 1;
				//cout << "jumping :" << i << " " << j << endl;
			}
		}
	}
	for (int p1 = 1; p1 <= p2; p1++){
		myPoint p = Q[p1];
		for (int k = 0; k < 4; k++){
			int nx = p.x + ArraySpace::xo[k];
			int ny = p.y + ArraySpace::yo[k];
			if (ArraySpace::inMap(myPoint(nx,ny),saliency.rows,saliency.cols) && !visit[nx][ny]){
				visit[nx][ny] = 1;
				dataAt<int>(jump, nx, ny) = dataAt<int>(jump, p.x, p.y) + 1;
				Q[++p2] = myPoint(nx, ny);
				ancestor[nx][ny] = Geometry::Point(ancestor[p.x][p.y]);
			}
		}
	}
	ArraySpace::deleteArray(visit,saliency.rows,saliency.cols);
	delete Q;
}

myPoint adjPoint(Mat &m, Geometry::Point x){
	myPoint a(x.x, x.y);
	if (a.x == -1) a.x = 0;
	if (a.y == -1) a.y = 0;
	if (a.x == m.rows) a.x -= 1;
	if (a.y == m.cols) a.y -= 1;
	return a;
}
//调用计算几何进行归一化处理
//has some problem now ... 
void SaliencyDistance::normalize(Mat &dis, Geometry::Point** ancestor){
	//find edge
	//return;
	for (int i = 0; i < dis.rows; i++){
		for (int j = 0; j < dis.cols; j++){
			if (dataAt<float>(dis, i, j) != 0){
				Geometry::Point o(ancestor[i][j]);
				Geometry::Point p(i, j);
				Geometry::Line l(o, p-o);
				
				Geometry::Line ll[5];
				ll[1] = Geometry::Line(Geometry::Point(0, 0), Geometry::Point(1, 0));
				ll[2] = Geometry::Line(Geometry::Point(0, 0), Geometry::Point(0, 1));
				ll[3] = Geometry::Line(Geometry::Point(dis.rows, dis.cols), Geometry::Point(-1, 0));
				ll[4] = Geometry::Line(Geometry::Point(dis.rows, dis.cols), Geometry::Point(0, -1));
				
				Geometry::Point pp[5];
				
				Geometry::Point edge(Geometry::MAXV, Geometry::MAXV);
				int flag = 0;

				for (int k = 1; k <= 4; k++){
					if (Geometry::intersectLine(l, ll[k], pp[k])){
						int pos = l.position(pp[k]);
						if ( pos== 2||pos==5){
							myPoint maybeEdge = myPoint(pp[k].x,pp[k].y);
							if (ArraySpace::inMap(dis,maybeEdge) && o.dis(pp[k]) < o.dis(edge)){
								edge = pp[k];
								flag = 1;
							}
						}
						//Debug() << "intersect point: " << pp[i];
					}
				}
				
				if (!flag){
					for (int k = 1; k <= 4; k++){
						if (Geometry::intersectLine(l, ll[k], pp[k])){
							int pos = l.position(pp[k]);
							if (pos == 2 || pos == 5){
								myPoint maybeEdge = adjPoint(dis, pp[k]);
								if (ArraySpace::inMap(dis, maybeEdge) && o.dis(pp[k]) < o.dis(edge)){
									edge = pp[k];
									flag = 1;
								}
							}
							//Debug() << "intersect point: " << pp[i];
						}
					}

				}

				if (flag){
					myPoint isEdge = adjPoint(dis,edge);
					
					if (ArraySpace::inMap(dis,isEdge) ){
						
						dataAt<float>(dis, i, j) /= dataAt<float>(dis,isEdge.x,isEdge.y);
						//Debug().setStatus(FileOut);//
						//Debug() << "dis :" << i << " " << j << " " << dataAt<float>(dis, i, j);
						//Debug() << dis;
						//printMatF<float>(dis, 10, 10, 0, 0);
						//writeMatF<float>(dis);
					}
					else{
						Debug() << "[ERROR] not in map: " << edge.x << " " << edge.y;
						Debug().pause();
						dataAt<float>(dis, i, j) = 1;
					}
				}
				else{
					for (int k = 1; k <= 4; k++){
						if (Geometry::intersectLine(l, ll[k], pp[k])){
							int pos = l.position(pp[k]);
							Debug() << "intersect: " << pp[k] << "pos: " << pos;
						}
					}
					Debug() << "[ERROR] point not found: " << i << " " << j;
					Debug() << p<< " " << o;	
					Debug().pause();
					dataAt<float>(dis, i, j) = 1;
				}
			}
		}
	}
	writeMatF<float>(dis);
}
void SaliencyDistance::deal(Mat &src, Mat &dstSaliency,Mat &dstDis)
{
	
	
	//saliency picture
	Mat mySaliency;
	saliency(src,mySaliency);
	//writeMat<uchar>(mySaliency);


	//jump flooding
	Mat jump(mySaliency.size(), CV_32S, Scalar::all(255));
	Geometry::Point** ancestor=ArraySpace::newArray<Geometry::Point>(jump.rows+5, jump.cols+5);
	//Geometry::Point ancestor[800][800];
	jumpFlooding(mySaliency,jump,ancestor);
	//show jump
	//ImageToDouble(jump,jump);
	//DoubleNomolization(jump);
	//imshow("jump", jump);
	//waitKey();


	//normalize
	Mat dis;
	jump.convertTo(dis, CV_32F);
	//normalize(dis,ancestor);
	DoubleNomolization(dis);	
	GaussianBlur(dis, dis,Size(5,5),0,0);
	//imshow("dis", dis);

	dstSaliency = mySaliency;
	dstDis = dis;
}