#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

struct S
{
	int a;
	int b;
	int c;
	int chash;

	int operator-(const S &other) const {
		return a - other.a + b - other.b + c - other.c;
	}
	bool operator<(const S &other) const {
		if (a != other.a)
			return a < other.a;

		if (b != other.b)
			return b < other.b;

		if (c != other.c)
			return c < other.c;

		return other.chash < other.chash;
	}

	const S *parent = nullptr;
};

std::ostream &operator<< (std::ostream &os, const S &s) {
	os << s.a << '\t' << s.b << '\t' << s.c << '\t' << s.chash;
	return os;
}


int main()
{
	ifstream fin("input.txt");

	if (!fin.is_open()) {
		fin.close();
		cout << "Нет файла\n";
		return 0;
	}

	int n;

	fin >> n;

	vector<float[3]> p(n);

	vector<float[3]> a(n);
	vector<float[3]> b(n);
	vector<float[3]> c(n);

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			fin >> p[j][i];
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			fin >> a[j][i];
			a[j][i] -= 1;
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			fin >> b[j][i];
			b[j][i] -= 1;
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			fin >> c[j][i];
			c[j][i] -= 1;
		}
	}
	fin.close();

	int a_start = 100;
	int b_start = 800;
	int c_start = 400;
	int chash_start = 600;

	cout << "Sart sum: " << a_start + b_start + c_start + chash_start << endl;

	int N = n + 1;

	S s_start = S{ a_start, b_start, c_start, chash_start };

	vector<map<S, float>> m(N);
	map<S, float> start;
	start.insert({ s_start, 0 });
	m[0] = std::move(start);
	for (size_t i = 0; i < n; i++)
	{
		map<S, float> next;
		for (const auto &el : m[i])
		{
			S s_prev = el.first;

			for (int au = 0; au < 3; au++)
			{
				for (int bu = 0; bu < 3; bu++)
				{
					for (int cu = 0; cu < 3; cu++)
					{
						S s = el.first;
						s.parent = &el.first;

						s.a += (s_start.a / 4) * (au - 1);
						s.b += (s_start.b / 4) * (bu - 1);
						s.c += (s_start.c / 4) * (cu - 1);
						s.chash += s_prev - s;

						if (s.chash < 0)
							continue;

						float income = el.second;
						for (size_t pi = 0; pi < 3; pi++)
						{
							income += p[i][pi] * ((float)s.a * a[i][pi] + (float)s.b * b[i][pi] + (float)s.c * c[i][pi]);
						}

						const auto iter = next.find(s);
						if (iter == next.end()) {
							next.insert({ s, income });
						}
						else if (iter->second < income) {
							iter->second = income;
						}
					}
				}
			}
		}

		cout << i + 1 << endl;
		for (const auto &el : next)
		{
			cout << el.first << "\t+= " << el.second << endl;
		}
		cout << endl;

		m[i + 1] = std::move(next);
	}

	float result = 0;
	S best;
	for (const auto &last : m[n]) {
		const S s = last.first;
		float sum = s.a + s.b + s.c + s.chash + last.second;
		if (sum > result) {
			result = sum;
			best = s;
		}
	}

	cout << "Best result: " << result << endl;
	const S *way = &best;
	int i = n;
	while (way != nullptr)
	{
		cout << i << ": " << *way << "\t+= " << m[i].at(*way) << endl;
		way = way->parent;
		--i;
	}

	return 0;
}