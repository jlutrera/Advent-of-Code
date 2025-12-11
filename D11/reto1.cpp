#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, vector<string>> adj;
    unordered_set<string> nodes;

    // Parse lines: "name: a b c"
    string line;
    while (getline(cin, line)) {
        // skip empty lines
        bool allSpace = true;
        for (char ch : line) { if (!isspace(static_cast<unsigned char>(ch))) { allSpace = false; break; } }
        if (allSpace) continue;

        // Find colon
        size_t colon = line.find(':');
        if (colon == string::npos) continue; // ignore malformed lines

        string src = line.substr(0, colon);
        // trim src
        auto trim = [](string &s){
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == string::npos) { s.clear(); return; }
            s = s.substr(a, b - a + 1);
        };
        trim(src);

        string rest = line.substr(colon + 1);
        trim(rest);

        vector<string> outs;
        if (!rest.empty()) {
            // split by spaces
            string token;
            stringstream ss(rest);
            while (ss >> token) {
                trim(token);
                if (!token.empty()) outs.push_back(token);
            }
        }

        adj[src].insert(adj[src].end(), outs.begin(), outs.end());
        nodes.insert(src);
        for (auto &t : outs) nodes.insert(t);
    }

    // Ensure nodes with no explicit outgoing list exist in adj
    for (auto &n : nodes) {
        if (!adj.count(n)) adj[n] = {};
    }

    const string START = "you";
    const string GOAL  = "out";

    // Cycle detection + memoized path count
    unordered_map<string, int> color; // 0=unvisited,1=visiting,2=done
    unordered_map<string, unsigned long long> memo;

    function<unsigned long long(const string&)> dfs = [&](const string &u) -> unsigned long long {
        auto itc = color.find(u);
        int c = (itc == color.end() ? 0 : itc->second);
        if (c == 1) {
            // cycle detected on current path
            throw runtime_error("Cycle detected in device graph; path count may be infinite.");
        }
        if (c == 2) {
            return memo[u];
        }

        color[u] = 1;

        if (u == GOAL) {
            memo[u] = 1ULL;
            color[u] = 2;
            return 1ULL;
        }

        unsigned long long sum = 0ULL;
        for (const auto &v : adj[u]) {
            sum += dfs(v);
        }

        memo[u] = sum;
        color[u] = 2;
        return sum;
    };

    try {
        if (!nodes.count(START)) {
            cout << 0 << "\n";
            return 0;
        }
        unsigned long long ans = dfs(START);
        cout << ans << "\n";
    } catch (const exception &e) {
        // If cycles exist, you can decide how to handle it. Here we print an informative message.
        cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
