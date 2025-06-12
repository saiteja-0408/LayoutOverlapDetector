#include "OverlapEngine.h"
#include <algorithm>
#include <set>

OverlapEngine::OverlapEngine(std::vector<Rectangle>& rects)
  : m_rects(rects) {}

struct Event {
    double x; int id; bool start;
    bool operator<(Event const& o) const {
        if (x!=o.x) return x<o.x;
        return start>o.start;
    }
};

void OverlapEngine::computeOverlaps(){
    for(auto& r: m_rects) r.overlaps=false;

    std::vector<Event> ev;
    ev.reserve(m_rects.size()*2);
    for(auto& r: m_rects){
        ev.push_back({r.x, r.id, true});
        ev.push_back({r.x+r.w, r.id, false});
    }
    std::sort(ev.begin(), ev.end());

    auto cmp=[&](int a,int b){ return m_rects[a].y < m_rects[b].y; };
    std::set<int, decltype(cmp)> active(cmp);

    for(auto const& e: ev){
        auto& R = m_rects[e.id];
        if(e.start){
            for(int aid: active){
                auto& A = m_rects[aid];
                if(A.y + A.h < R.y) continue;
                if(R.y + R.h < A.y) continue;
                A.overlaps = R.overlaps = true;
            }
            active.insert(e.id);
        } else {
            active.erase(e.id);
        }
    }
}
