/**
 * DIMACS k-SAT to j-SAT converter (3 <= j < k)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

int iabs(int a) { return a < 0 ? -a : a; }
int imax(int a, int b) { return a >= b ? a : b; }

int main(int argc, char* argv[]) {
    string instance_path(argv[1]);
    ifstream instance(argv[1]);
    istringstream kss(argv[2]);
    int k;
    kss >> k;
    string line;

    int max_var = -1;
    while (getline(instance, line)) {
        if (line.substr(0, 1) == "c" || line.substr(0, 1) == "p") continue;
        istringstream lss(line);
        int v;
        while (lss >> v && v != 0) {
            max_var = imax(max_var, iabs(v));
        }
    }

    vector<int> output;
    int num_clauses = 0;
    instance.clear();
    instance.seekg(0, ios_base::beg);
    cout << "c " << k << "-SAT formula converted from " << instance_path.substr(instance_path.find_last_of('/')+1) << endl;
    while (getline(instance, line)) {
        if (line.substr(0, 1) == "c" || line.substr(0, 1) == "p") {
            if (line.substr(0, 1) == "p") cout << "c Originally: ";
            cout << line << endl;
            continue;
        }
        istringstream lss(line);
        int v;
        vector<int> clause;
        while (lss >> v && v != 0) clause.push_back(v);
        if (clause.size() > k) {
            int var;
            for (var = 0; var < k - 1; ++var) {
                output.push_back(clause[var]);
            }
            num_clauses++;
            output.push_back(++max_var);
            output.push_back(0);

            int vars_left = clause.size() - (k - 1);
            while (vars_left > 0) {
                output.push_back(-max_var);
                int take = vars_left > (k - 1) ? k - 2 : vars_left;
                vars_left -= take;
                while (take--) output.push_back(clause[var++]);
                if (vars_left > 0) output.push_back(++max_var);
                num_clauses++;
                output.push_back(0);
            }
        } else {
            for (int i = 0; i < clause.size(); i++) output.push_back(clause[i]);
            output.push_back(0);
            num_clauses++;
        }
    }
    cout << "p cnf " << max_var << " " << num_clauses << endl;
    for (size_t i = 0; i < output.size(); ++i) {
        if (output[i] == 0) cout << "0" << endl;
        else cout << output[i] << " ";
    }
}
