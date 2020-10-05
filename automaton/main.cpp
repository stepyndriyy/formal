#include "automaton.h"

int main() {
    using namespace std;
    int vertex_cnt, edges_cnt;
    int start;
    cout << "vertex count and edges count: ";
    cin >> vertex_cnt >> edges_cnt;

    vector<Edge> edges(edges_cnt); 
    cout << "write edges in format from, to, char:\n";
    for (int i = 0; i < edges_cnt; ++i) {
        int from, to;
        char letter;
        cin >> from >> to >> letter;
        edges[i] = (Edge(from, to, letter));
    } 
    std::cout << "count of terminate vertex: "; 
    int cnt_ends;
    cin >> cnt_ends;
    vector<int> terminates(cnt_ends);
    cout << "terminate vertex\n";
    for(int i = 0; i < cnt_ends; ++i) 
        cin >> terminates[i]; 

    Automaton x = Automaton(edges, terminates).determinizate();
    auto ans = x.get_edges();
    auto ans_terminates = x .get_terminates();
    std::cout <<"edges: \n";
    for (int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i].from << " " << ans[i].to << " " << ans[i].letter << '\n';
    }
    std::cout << "terminate vertexes:\n ";
    for (int i = 0; i < ans_terminates.size(); ++i) {
        std::cout << ans_terminates[i] << " ";
    } 
    std::cout << '\n';
    
    Automaton y = x.minimalize();
}
