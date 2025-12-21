#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

using P = long double;
using P = long double;

#define CheckVar(var) if(var != other.var){return var < other.var;}
//#define CheckVarF(var) if(!areEqual(var, other.var)){return var < other.var;}

bool areEqual(P a, P b, P epsilon = 1e-3) {
	return std::abs(a - b) < epsilon;
}

struct St
{
	P a;
	P b;
	P c;
	P cash;

	bool operator<(const St &other) const {
		CheckVar(a);
		CheckVar(b);
		CheckVar(c);
		CheckVar(cash);

		return false;
	}

	P result() const {
		return a + b + c + cash;
	}
};
std::ostream &operator<< (std::ostream &os, const St &st) {
	os << "St={ " << st.a << ' ' << st.b << ' ' << st.c << ' ' << st.cash << " }";
	return os;
}

struct Act {
	P aAct = 1;
	P bAct = 1;
	P cAct = 1;

	bool operator<(const Act &other) const {
		CheckVar(aAct);
		CheckVar(bAct);
		CheckVar(cAct);
		return false;
	}

	static const P Sell;
	static const P Nothing;
	static const P Buy;

	static const std::set<P> AllAct;
};
const P Act::Sell = 0.75;
const P Act::Nothing = 1;
const P Act::Buy = 1.25;
const std::set<P> Act::AllAct = { Act::Sell , Act::Nothing, Act::Buy };
std::ostream &operator<< (std::ostream &os, const Act &act) {
	const auto pr = [](P p) -> char {
		if (p == Act::Nothing)
			return '=';
		if (p < Act::Nothing)
			return '-';
		return '+';
	};

	os << "Act={ " << pr(act.aAct) << ' ' << pr(act.bAct) << ' ' << pr(act.cAct) << " }";
	return os;
}

struct ActAndRes {
	Act act;
	P finalResult = 0;
};
std::ostream &operator<< (std::ostream &os, const ActAndRes &actAndRes) {
	os << actAndRes.act << " -> " << actAndRes.finalResult;
	return os;
}

int N;
int M = 3;

vector<vector<P>> p;

vector<vector<P>> ra;
vector<vector<P>> rb;
vector<vector<P>> rc;

vector<map<St, ActAndRes>> m;

vector<multimap<std::pair<St, Act>, St>> ways;

P calcAllAct(int n, St st);

P calc(int n, St st, Act act) {
	P nStCash = st.cash - (st.a * (act.aAct - 1) + st.b * (act.bAct - 1) + st.c * (act.cAct - 1));
	if (nStCash < 0)
		return 0;

	P res = 0;

	for (size_t k = 0; k < M; k++)
	{
		St nSt = st;

		nSt.a *= act.aAct * ra[n][k];
		nSt.b *= act.bAct * rb[n][k];
		nSt.c *= act.cAct * rc[n][k];
		nSt.cash = nStCash;

		res += p[n][k] * calcAllAct(n + 1, nSt);
		if (n + 1 < N) {
			ways[n].insert({ {st, act}, nSt });
		}
	}

	return res;
}

P calcAllAct(int n, St st) {
	if (n == N) {
		return st.result();
	}

	auto it = m[n].find(st);
	if (it != m[n].end()) {
		it->second.finalResult;
	}

	ActAndRes bestRes;
	for (const P aAct : Act::AllAct) {
		for (const P bAct : Act::AllAct) {
			for (const P cAct : Act::AllAct) {
				Act act{ aAct, bAct, cAct };
				P res = calc(n, st, act);
				if (res > bestRes.finalResult)
					bestRes = ActAndRes{ act, res };
			}
		}
	}

	m[n].insert({ st, bestRes });
	return bestRes.finalResult;
}

int main()
{
	ifstream fin("input.txt");

	if (!fin.is_open()) {
		fin.close();
		cout << "Нет файла\n";
		return 0;
	}


	fin >> N;

	const auto iniV = [](vector<vector<P>> &v) {
		v.resize(N);
		for (size_t i = 0; i < N; i++)
		{
			v[i].resize(M);
		}
	};

	iniV(p);

	iniV(ra);
	iniV(rb);
	iniV(rc);

	m.resize(N);

	ways.resize(N);

	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			fin >> p[j][i];
		}
	}

	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			fin >> ra[j][i];
			//a[j][i] -= 1;
		}
	}

	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			fin >> rb[j][i];
			//b[j][i] -= 1;
		}
	}

	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			fin >> rc[j][i];
			//c[j][i] -= 1;
		}
	}
	fin.close();

	St startSt{ 100 , 800, 400, 600 };

	cout << "Sart sum: " << startSt.result() << endl;

	P expectedResult = calcAllAct(0, startSt);

	cout << "Expected sum: " << expectedResult << endl;
	cout << "Expected profit: " << expectedResult - startSt.result() << endl;

	cout << "\nOptimal strategy\n";
	vector<map<St, ActAndRes>::iterator> toShow;
	toShow.push_back(m[0].begin());
	for (size_t n = 0; n < N; n++)
	{
		cout << "At stage " << n + 1 << ':' << endl;

		vector<map<St, ActAndRes>::iterator> nextToShow;
		for (const auto it : toShow) {
			cout << "If " << it->first << " do " << it->second << endl;
			if (n + 1 < N) {
				auto range = ways[n].equal_range({ it->first, it->second.act });
				for (auto next = range.first; next != range.second; ++next) {
					nextToShow.push_back(m[n + 1].find(next->second));
				}
			}
		}
		toShow = std::move(nextToShow);

		cout << endl;
	}

	return 0;
}