#include <inttypes.h>
#include <random>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

typedef int8_t color_t;

namespace intersection {
	inline void writeln (const std::string &s, bool append = true) {
		std::ofstream lines;
		if (append)
			lines.open ("lines.txt", std::ios::out | std::ios::app);
		else
			lines.open ("lines.txt");
		if (lines.is_open()) {
			lines << s << std::endl;
			lines.close();
		} else 
			throw std::runtime_error("Unable to open file lines.txt");
	}

	class pt {
		double x_c, y_c;
	public:
		pt (const double &a, const double &b): x_c(a), y_c(b) {}
		double x () const {return x_c;}
		double y () const {return y_c;}
		friend std::ostream& operator << (std::ostream& o, const pt &x);
	};

	class sign {
		int8_t sgn;
		const double EPS_POS = 0.0001;
		const double EPS_NEG = -0.0001;
	public:
		sign (const double &val) {
			if (val <= EPS_POS && val >= EPS_NEG) {
				sgn = 0;
			} else if (val > EPS_POS) {
				sgn = 1;
			} else {
				sgn = -1;
			}
		}
		explicit sign (const char &sign) {
			switch (sign) {
			case '+': 
				sgn = 1;
				break;
			case '-':
				sgn = -1;
				break;
			default:
				sgn = 0;
			}
		}
		int8_t sn() const {
			return sgn;
		}
		sign operator*(const sign &rhs) const {
			switch (sgn * rhs.sgn) {
			case 0:
				return sign('0');
			case 1:
				return sign('+');
			default:
				return sign('-');
			}
		}
	};
	
	inline sign area_sign (const pt &a, const pt &b, const pt &c) {
		return sign((b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x()));
	}
	
	inline bool intersect_1 (double a, double b, double c, double d) {
		if (a > b)  std::swap (a, b);
		if (c > d)  std::swap (c, d);
		return std::max(a,c) <= std::min(b,d);
	}
	
	bool intersect (pt a, pt b, pt c, pt d) {
		return intersect_1 (a.x(), b.x(), c.x(), d.x())
			&& intersect_1 (a.y(), b.y(), c.y(), d.y())
			&& ((area_sign(a, b, c) * area_sign(a, b, d)).sn() <= 0)
			&& ((area_sign(c, d, a) * area_sign(c, d, b)).sn() <= 0);
	}

	inline double rand_val() {
		return double(std::rand() - RAND_MAX / 2) / RAND_MAX;
	}

	class segment {
		pt st, fn;
		color_t color;
	public:
		segment (const double &ba, 
				const double &bb, 
				const double &ea, 
				const double &eb, 
				color_t c): st(ba, bb), fn(ea, eb), color(c) {
			writeln(std::to_string(ba) + " " +
					std::to_string(bb) + " " +
					std::to_string(ea) + " " +
					std::to_string(eb) + " " +
					std::to_string(c));
		}
		segment (const pt &b,
				const pt &e,
				color_t c = 1): st(b), fn(e), color(c) {}
		pt begin () const {return st;}
		pt end () const {return fn;}
		friend std::ostream& operator << (std::ostream& o, const segment &x);
	};

	class random_sequence {
		std::vector <segment> segments;
	public:
		random_sequence (color_t color, int n = 5) {
			pt prev (rand_val(), rand_val());
			for (int i = 0; i < n; ++i) {
				pt cur (rand_val(), rand_val());
				segments.push_back(segment(prev.x(), prev.y(), cur.x(), cur.y(), color));
				prev = cur;
			}
		}
		friend std::ostream& operator << (std::ostream& o, const random_sequence &x);
	};
/*
	class random_pt {
		std::vector <pt> points;
	public:
		random_pt (int n = 5) {
			for (int i = 0; i < n; ++i) {
				points.push_back(pt(rand_val(), rand_val()));
			}
		}

		pt find_leftmost_point() const {
			pt leftmost_point(0.0, 0.0);
			double leftmost_x, leftmost_y;
			bool uninit = true;
			for (auto &point : points) {
				double x = point.x();
				if (uninit || x < leftmost_x) {
					leftmost_x = x;
					leftmost_y = point.y();
					leftmost_point = point;
					uninit = false;
				} else if (x < leftmost_x + 0.0001 && x > leftmost_x - 0.0001) {
					if (point.y() > leftmost_y) {
						leftmost_y = point.y();
						leftmost_point = point;
					}
				}
			}
			return leftmost_point;
		}

		pt find_rightmost_point() const {
			pt rightmost_point(0.0, 0.0);
			double rightmost_x, rightmost_y;
			bool uninit = true;
			for (auto &point : points) {
				double x = point.x();
				if (uninit || x > rightmost_x) {
					rightmost_x = x;
					rightmost_y = point.y();
					rightmost_point = point;
					uninit = false;
				} else if (x < rightmost_x + 0.0001 && x > rightmost_x - 0.0001) {
					if (point.y() > rightmost_y) {
						rightmost_y = point.y();
						rightmost_point = point;
					}
				}
			}
			return rightmost_point;
		}

		int8_t is_point_above_the_line (const pt &point, const segment &line_points) const {
			double px = point.x(), py = point.y();
			pt P1 = line_points.begin();
			pt P2 = line_points.end();
			double P1x = P1.x(), P1y = P1.y();
			double P2x = P2.x(), P2y = P2.y();
			double det = (P1x - px) * (P2y - py) - (P1y - py) * (P2x - px);
			sign s(det);
			return s.sn();
		}

		std::vector <std::vector <pt>> sort_array_into_A_B_C(const segment &line_points) {
			pt P1 = line_points.begin();
			pt P2 = line_points.end();
			double x_lm = P1.x(), y_lm = P1.y();
			double x_rm = P2.x(), y_rm = P2.y();
			std::vector <pt> A_array, B_array, C_array;
			for (auto &point : points) {
				int8_t sign = is_point_above_the_line(point, line_points);
				if (sign == 0)
					C_array.push_back(point);
				else if (sign == -1)
					A_array.push_back(point);
				else
					B_array.push_back(point);
			}
			std::vector <std::vector <pt>> a;
			a.push_back(A_array);
			a.push_back(B_array);
			a.push_back(C_array);
        	return a;
		}

		void sort_and_merge_A_B_C_arrays(const std::vector <pt> &A_array, const std::vector <pt> &B_array, const std::vector <pt> &C_array) {
			std::vector <pt> A_C_array;
			for (auto &point : A_array) {
				A_C_array.push_back(point);
			}
			for (auto &point : C_array) {
				A_C_array.push_back(point);
			}
			std::sort(A_C_array.begin(), A_C_array.end(), [](pt a, pt b) { return a.x() < b.x(); });
			std::sort(A_C_array.begin(), A_C_array.end(), [](pt a, pt b) { return a.x() > b.x(); });
			std::vector <pt> merged_arrays;
			for (auto &point : A_C_array) {
				merged_arrays.push_back(point);
			}
			for (auto &point : B_array) {
				merged_arrays.push_back(point);
			}
			points = merged_arrays;
		}

		void make_poligon_like() {
			segment line_points (find_leftmost_point(), find_rightmost_point());
			auto x = sort_array_into_A_B_C(line_points);
			sort_and_merge_A_B_C_arrays(x[0], x[1], x[2]);
		}

		std::vector <pt> &return_val () {
			return points;
		}
	};

	class random_sequence_no_self_intersection {
		std::vector <segment> segments;
	public:
		random_sequence_no_self_intersection (color_t color, int n = 5) {
			random_pt poli (n);
			poli.make_poligon_like();
			bool started = true;
			pt prev(0.0, 0.0);
			for (auto &point : poli.return_val()) {
				if (started) {
					started = false;
					prev = point;
				} else {
					segments.push_back(segment(prev.x(), prev.y(), point.x(), point.y(), color));
					prev = point;
				}
			} 
		}
	};
*/


	std::ostream& operator << (std::ostream& o, const segment &x) {
		o << "Segment is:\na: " << x.st << "\nb: " << x.fn << std::endl;
		return o;
	}

	std::ostream& operator << (std::ostream& o, const pt &x) {
		o << "(" << x.x_c << ", " << x.y_c << ")";
		return o;
	}

	std::ostream& operator << (std::ostream& o, const random_sequence &x) {
		for (auto &segm : x.segments) {
			std::cout << segm;
		}
		return o;
	}
}

int main () {
	srand(time(NULL));
	try {
		intersection::writeln("##x1 y1 x2 y2 colorNumber", false);
		intersection::random_sequence x(2, 10);
		intersection::random_sequence y(5, 4); /*
		if (intersect(a.begin(), a.end(), b.begin(), b.end())) {
			std::cout << "Segments intersect!" << std::endl; 
		} else {
			std::cout << "Segments don't instersect!" << std::endl; 
		} */
		system("gnuplot < plot.cmd");
		system("open result.png");
	} catch (std::runtime_error &x) {
		std::cout << "Runtime Error occured: " << x.what() << std::endl;
	}
	return 0;
}