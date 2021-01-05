#include <iostream>
#include <cmath>
#include <vector>

class point {
private:
	int x;
	int y;
public:

	point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int getx() {
		return x;
	}

	int gety() {
		return y;
	}

	void setx(int x) {
		this->x = x;
	}

	void sety(int y) {
		this->y = y;
	}

	bool equals(point p) {
		if (this->x == p.getx() && this->y == p.gety()) return true;
		else return false;
	}
};

class line {
private:
	point p1 = point(0, 0);
	point p2 = point(0, 0); // AB - BA
public:

	line(point p1, point p2) {
		this->p1 = p1;
		this->p2 = p2;
	}

	bool equals(line line) {
		if ((this->p1.equals(line.p1) 
			&& this->p2.equals(line.p2)) 
			|| 
			(this->p1.equals(line.p2) 
			&& this->p2.equals(line.p1))) return true;
		else return false;
	}

	bool intersects(line line) {
		if (this->p1.equals(line.getp1()) 
			|| this->p1.equals(line.getp2()) 
			|| this->p2.equals(line.getp1()) 
			|| this->p2.equals(line.getp2())) return true;
		else return false;
	}

	double getlength() {
		return sqrt(pow((this->p2.getx() - this->p1.getx()), 2) + pow((this->p2.gety() - this->p1.gety()), 2));
	}

	double getslope() {
		return abs((double)(this->p2.gety() - this->p1.gety()) / (this->p2.getx() - this->p1.getx())); // we have to use abs cuz we dont know how the points are set
	}

	bool isparallel(line line) {
		if (this->getslope() == line.getslope()) return true;
		else return false;
	}

	bool isperpendicular(line line) {
		if (this->getslope() * line.getslope() == -1) return true;
		else if ((this->getslope() == INFINITY && line.getslope() == 0) 
			|| (this->getslope() == 0 && line.getslope() == INFINITY)) return true;
		else return false;
	}

	bool equalinlength(line line) {
		if (this->getlength() == line.getlength()) return true;
		else return false;
	}

	point getp1() {
		return this->p1;
	}

	point getp2() {
		return this->p2;
	}

	void setp1(point p1) {
		this->p1 = p1;
	}

	void setp2(point p2) {
		this->p2 = p2;
	}
};

using namespace std;

int main() {
	/* 
	We code our point reading using 2 variables to not overuse our heap size with a hashmap. 
	Let's use x and y for the x and y of each point
	that is going to be added to our list<point> and n
	for the amount of points we will have.
	*/
	int x, y, n;
	vector<point> points;
	vector<line> lines;
	std::cin >> n;
	for (int i = 1; i <= n; i++) {
		std::cin >> x >> y;
		points.push_back(point(x, y));
	}
	/*
	Now, let's compute every single line we can using our list of points.
	We must also take care of duplicates. We don't want to add both lines made
	with points (P1, P2) and (P2, P1)!
	To do this, we take an i from i = 0 and we loop using another
	integer j from i + 1, therefore no duplicates!
	*/
	for (int i = 0; i < points.size() - 1; i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (!points[i].equals(points[j])) {
				line l = line(points[i], points[j]);
				lines.push_back(l);
			}
		}
	}
	/*
	A rectangle is defined as the shape which has 2 pairs of
	parallel lines, which are both also equal, and that has
	a right angle. (any angle between any 2 consecutive lines
	should be right angled). Also, we must notice that for 2
	consecutive lines,  _____________ in a rectangle, they have a common point!
						|			|
						|___________|
	I honestly have no idea what the fuck I'm doing, so let's just do
	it in complexity O(n^4) because we're too dumb to think after fucking
	around with this problem for the past hour.
	*/
	int count = 0;
	if (lines.size() < 4) {
		cout << count;
		return 0;
	}
	for (int i = 0; i < lines.size() - 3; i++) {
		line l1 = lines[i];
		for (int j = i + 1; j < lines.size() - 2; j++) {
			line l2 = lines[j];
			// these 2 lines have to either be parallel and equal or perpendicular and intersected in 1 point 
			// to be the sides of a rectangle.
			int type = 0;
			if (l1.isparallel(l2) && l1.equalinlength(l2)) type = 1;
			else if (l1.isperpendicular(l2) && l1.intersects(l2)) type = 2;
			else j++;
			for (int k = j + 1; k < lines.size() - 1; k++) {
				line l3 = lines[k];
				for (int l = k + 1; l < lines.size(); l++) {
					line l4 = lines[l];
					// case 1: l1 and l2 are parallel and equal
					// case 2: l1 and l2 are perpendicular and intersect in 1 point
					switch (type) {
					case 1:
						if (l3.isparallel(l4) && l3.equalinlength(l4) && l3.intersects(l1) && l4.intersects(l1)) count++;
						break;
					case 2:
						if (l1.isparallel(l3) 
							&& l1.equalinlength(l3) 
							&& l2.isparallel(l4) 
							&& l2.equalinlength(l4) 
							&& l1.intersects(l2)
							&& l1.intersects(l4)
							&& l3.intersects(l2)
							&& l3.intersects(l4))
							count++;
						else if (l1.isparallel(l4)
							&& l1.equalinlength(l4)
							&& l2.isparallel(l3)
							&& l2.equalinlength(l3)
							&& l1.intersects(l2)
							&& l1.intersects(l3)
							&& l3.intersects(l1)
							&& l3.intersects(l4))
							count++;
					} // absolutely disgusting
				}
			}
		}
	}
	cout << count;
}