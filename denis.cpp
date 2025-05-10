#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;


set<string> parseClause(const string& clauseStr) {
    set<string> clause;
    stringstream ss(clauseStr);
    string literal;
    while (ss >> literal) {
        clause.insert(literal);
    }
    return clause;
}


bool isNegation(const string& a, const string& b) {
    return (a == "-" + b) || (b == "-" + a);
}


vector<set<string>> resolve(const set<string>& ci, const set<string>& cj) {
    vector<set<string>> resolvents;
    for (const auto& lit : ci) {
        string negated = (lit[0] == '-') ? lit.substr(1) : "-" + lit;
        if (cj.count(negated)) {
            set<string> newClause = ci;
            newClause.insert(cj.begin(), cj.end());
            newClause.erase(lit);
            newClause.erase(negated);
            resolvents.push_back(newClause);
        }
    }
    return resolvents;
}


bool resolutionSAT(const vector<string>& inputClauses) {
    vector<set<string>> clauses;
    for (const auto& str : inputClauses)
        clauses.push_back(parseClause(str));

    set<set<string>> newClauses;

    while (true) {
        int n = clauses.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                auto resolvents = resolve(clauses[i], clauses[j]);
                for (const auto& res : resolvents) {
                    if (res.empty()) return true; 
                    newClauses.insert(res);
                }
            }
        }

        bool added = false;
        for (const auto& nc : newClauses) {
            bool exists = false;
            for (const auto& c : clauses) {
                if (c == nc) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                clauses.push_back(nc);
                added = true;
            }
        }

        if (!added) return false; 
    }
}

int main() {
    vector<vector<string>> testCases = {
        {"A B", "-A", "-B"},        
        {"A B", "-A C"},           
        {"A", "-A"},               
        {"A B C", "-B C"}          
    };

    for (size_t i = 0; i < testCases.size(); ++i) {
        auto start = chrono::high_resolution_clock::now();
        bool unsat = resolutionSAT(testCases[i]);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "Test Case " << i + 1 << ": "
             << (unsat ? "UNSATISFIABLE" : "SATISFIABLE")
             << " | Time: " << elapsed.count() << " sec" << endl;
    }

    return 0;
}
