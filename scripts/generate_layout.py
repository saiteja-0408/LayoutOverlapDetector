#!/usr/bin/env python3
import json, random, os, pathlib
base = pathlib.Path(__file__).resolve().parent.parent
path = base / "resources" / "layouts" / "sample_layout.json"
rects=[]
for i in range(50):
    rects.append({
        "id": i,
        "x": random.uniform(0,700),
        "y": random.uniform(0,500),
        "w": random.uniform(20,150),
        "h": random.uniform(20,150)
    })
rects[0]["x"] = 100; rects[0]["y"]=100
rects[1]["x"] = 120; rects[1]["y"]=120
path.parent.mkdir(parents=True, exist_ok=True)
path.write_text(json.dumps(rects, indent=2))
print("Wrote", path)
