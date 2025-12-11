#include <bits/stdc++.h>
using namespace std;

struct Graph {
    unordered_map<string, vector<string>> adj;
    unordered_set<string> nodes;
};

static inline void trim(string &s){
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == string::npos) { s.clear(); return; }
    s = s.substr(a, b - a + 1);
}

Graph parse_input(istream &in){
    Graph g;
    string line;
    while (getline(in, line)){
        bool allSpace = true;
        for (char ch : line){ if (!isspace((unsigned char)ch)) { allSpace=false; break; } }
        if (allSpace) continue;

        size_t colon = line.find(':');
        if (colon == string::npos) continue;

        string src = line.substr(0, colon);
        trim(src);
        string rest = line.substr(colon+1);
        trim(rest);

        vector<string> outs;
        if (!rest.empty()){
            string token;
            stringstream ss(rest);
            while (ss >> token){ trim(token); if (!token.empty()) outs.push_back(token); }
        }
        g.adj[src].insert(g.adj[src].end(), outs.begin(), outs.end());
        g.nodes.insert(src);
        for (auto &t : outs) g.nodes.insert(t);
    }
    for (auto &n : g.nodes) if (!g.adj.count(n)) g.adj[n] = {};
    return g;
}

// Part One: count paths from "you" to "out"
unsigned long long count_paths_you_out(const Graph &g){
    const string START = "you";
    const string GOAL  = "out";
    if (!g.nodes.count(START)) return 0;

    unordered_map<string,int> color; // 0 unvisited, 1 visiting, 2 done
    unordered_map<string, unsigned long long> memo;

    function<unsigned long long(const string&)> dfs = [&](const string &u)->unsigned long long{
        int c = color[u];
        if (c == 1) throw runtime_error("Cycle detected; infinite paths.");
        if (c == 2) return memo[u];
        color[u] = 1;

        if (u == GOAL){ memo[u]=1ULL; color[u]=2; return 1ULL; }

        unsigned long long sum = 0;
        for (const auto &v : g.adj.at(u)) sum += dfs(v);

        memo[u]=sum; color[u]=2; return sum;
    };
    return dfs(START);
}

// Part Two: count paths from "svr" to "out" that visit both dac and fft
unsigned long long count_paths_svr_out_with_dac_fft(const Graph &g){
    const string START = "svr";
    const string GOAL  = "out";
    const string DAC   = "dac";
    const string FFT   = "fft";
    if (!g.nodes.count(START)) return 0;

    // memo[node][mask] => number of valid paths from node with visited-mask
    unordered_map<string, array<unsigned long long,4>> memo;
    unordered_map<string, array<int,4>> color; // per mask: 0/1/2

    function<unsigned long long(const string&, int)> dfs = [&](const string &u, int mask)->unsigned long long{
        int c = color[u][mask];
        if (c == 1) throw runtime_error("Cycle detected; infinite paths under constraint.");
        if (c == 2) return memo[u][mask];
        color[u][mask] = 1;

        int newMask = mask;
        if (u == DAC) newMask |= 1;
        if (u == FFT) newMask |= 2;

        if (u == GOAL){
            unsigned long long res = (newMask == 3) ? 1ULL : 0ULL;
            memo[u][mask] = res; color[u][mask] = 2; return res;
        }

        unsigned long long sum = 0;
        for (const auto &v : g.adj.at(u)) sum += dfs(v, newMask);

        memo[u][mask] = sum; color[u][mask] = 2; return sum;
    };

    return dfs(START, 0);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Graph g = parse_input(cin);

    try {
        unsigned long long total_paths_you = count_paths_you_out(g);
        unsigned long long constrained_paths_svr = count_paths_svr_out_with_dac_fft(g);

        // Print answers (first the part one style count for "you", then part two constrained count for "svr")
        // If you only need part two for a given puzzle, print just constrained_paths_svr.
        cout << constrained_paths_svr << "\n";
    } catch (const exception &e){
        cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
