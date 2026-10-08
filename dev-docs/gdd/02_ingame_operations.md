# ByteBurst™ — In-Game Operations
### Game Design Document: Live Server Infiltration & Tactical Systems

---

## 1. Overview & Visual Layout
During a live contract, the player enters an infected server's memory space. The screen is split into two synchronized viewports:
- **Top 65% — Memory Map**: Geometric visualization of memory rooms, corridor buses, resource nodes, the player's triangle avatar, and virus threats.
- **Bottom 35% — Tactical CLI Console**: The command prompt where the player issues movement, scanning, deployment, and tactical orders.

```
+-----------------------------------------------------------------------------+
| [SECTOR: L2 CACHE] [ADDR: 0x0032 - 0x0089] [CYCLE BAL: 45] [THREAT: CLASS C]|
+-----------------------------------------------------------------------------+
|                                                                             |
|   +--------------------------+               +--------------------------+   |
|   | 0x0032 (L2 CACHE)        |               | 0x0054 (QUARANTINED)     |   |
|   |                          |               |                          |   |
|   |   ▷ (Player)             |===============|   [▲ SENTINEL]           |   |
|   |                          |  (Data Line)  |        \ (pew pew)       |   |
|   |   [■ MINER N1]           |               |       [◆ TROJAN]         |   |
|   +--------------------------+               +--------------------------+   |
|                                                                             |
+-----------------------------------------------------------------------------+
| > deploy-sentinel                                                           |
| [SYS] Sentinel drone deployed. Engaging hostile in 0x0054...                |
| > _                                                                         |
+-----------------------------------------------------------------------------+
```

---

## 2. Infiltration Sequence: Step-by-Step

### Phase 1: Location & Sector Selection
When entering a server, the terminal displays procedurally generated memory regions:
```
AVAILABLE SECTORS:
  [1] L2 CACHE (range 0x0032 - 0x0089) | Diagnostic: 70% Infection Risk
  [2] RAM      (range 0xFFFA - 0xFFFF) | Diagnostic: 40% Infection Risk
  [3] L3 CACHE (range 0x12AB - 0x1F1D) | Diagnostic: 85% Infection Risk
```
- **Diagnostic Precision**: Percentage estimates of where viruses may hide. Initially rough; accuracy improves with Rig upgrades.
- Player selects their entry point (e.g. `select 1`).

---

### Phase 2: Spawning & Ambient Atmosphere
- **Player Avatar**: A minimalist cyan triangle (`▷`), spawning at the center-left of the initial room.
- **Atmosphere**: Dead quiet. Minimalist ambient low hum.
- **Room Representation**: Dungeon-like chambers represented as **Memory Allocation Blocks** (dark rectangles with thin neon borders, labeled with hex addresses like `[0x0032]`), connected by parallel data bus lines.

---

### Phase 3: The Scanner (`scan` or `send-probe`)
* **Possible Namings**: `scan`, `send-probe`, `ping`, `echo`.
* **Visual**: Three concentric greenish SDF rings radiate outward from the player triangle.
* **Audio & Directional Indicators**:
  - **No detection**: Wave fades silently.
  - **Distant Threat**: Low, muffled radar beep.
  - **Close Threat**: Loud, sharp ping. A directional dot appears on the radar HUD showing bearing and approximate distance (e.g. `Distance: 40+`).
  - **Resource Node**: Distinct crystalline chime.
  - **False Alarms & Multi-Targets**: System can detect multiple blips simultaneously; noisy sectors may produce faint false echo signatures.

---

### Phase 4: Resource Mining (`deploy-miner <node-label>`)
* **Possible Namings**: `deploy-miner`, `mine <node>`, `tap <node>`.
* **Mechanics**:
  - Scanning reveals a labeled resource node (e.g., `Node N1`).
  - Player executes: `deploy-miner N1`.
  - Miner constructs at the node and begins extracting **Cycles** automatically over time.
* **Audio Feedback**:
  - As soon as the miner goes online, a **peaceful, atmospheric synthesizer music loop** fades in.

---

### Phase 5: Grid Movement & Obstacle Collision (`move-<direction> <units>`)
* **Possible Namings**: `move-right 30`, `mr 30`, `mv r 30`, `step east 30`.
* **Mechanics**:
  - The triangle avatar glides smoothly across the grid/corridor for the specified distance.
  - **Collision**: If an obstacle or wall is encountered mid-path, the avatar halts cleanly at the barrier without clipping, outputting the actual distance traveled.

---

### Phase 6: Threat Discovery & Combat Music Shift
* **Trigger**: A scan reveals a confirmed hostile nearby (e.g. `THREAT CLASS C MEDIUM DANGER`).
* **Audio Feedback**:
  - The peaceful miner synth abruptly ducks down.
  - **Dramatic/combat percussion and synth basslines** kick in immediately.

---

### Phase 7: The Quarantine Protocol (`deploy-quarantine <sector>`)
* **Possible Namings**: `deploy-quarantine <sector>`, `quarantine <sector>`, `lockdown <sector>`, `isolate <sector>`.
* **Tactical Tradeoffs**:
  1. **Damage Boost**: All player weapons, sentinels, and defensive systems inside the quarantined sector deal significantly increased damage.
  2. **Economic Lockdown**: All mining and cycle extraction inside that sector is completely disabled while quarantine is active.
  3. **Threat Death-Burst**: When infected threats die, they emit an explosive glitch shockwave. If the fight is not controlled, nearby resource nodes can take heavy damage or be permanently destroyed!

---

### Phase 8: Sentinel Combat Drones (`deploy-sentinel`)
* **Possible Namings**: `deploy-sentinel`, `drone combat`, `spawn-sentry`.
* **Mechanics**:
  - Deploys an automated combat drone at the player’s current position.
  - Drone automatically acquires and fires at threats within its range.
  - Sound effects are retro-styled, punchy synthesizer beeps and boops.
* **Tower Defense Upgrades**:
  - Player can upgrade and specialize deployed sentinels during the run (e.g. Rail-Piercer, EMP Disrupter, Area Scrubber).

---

### Phase 9: Pre-Allocation / Resource Debt (Memory Overcommit)
* **Possible Namings**: *Pre-Allocation*, *Memory Overcommit*, *Cycle Debt*, *Emergency Overdraft*.
* **Concept**:
  - Sentinel drones, miners, and tools cost Cycles.
  - If the player faces an emergency but lacks the funds, they can **pre-allocate the upgrade**, pushing their Cycle balance into negative numbers (e.g. `-120 Cycles`).
* **Penalty**:
  - While in debt, system performance is temporarily throttled (e.g., slower movement, slower clock speed, or miners divert 100% of revenue to debt repayment) until balance returns to zero.

---

### Phase 10: Sector Traversal (`change-sector <target>`)
* **Possible Namings**: `change-sector <target>`, `goto <target>`, `bus-transit <target>`.
* **Mechanics**:
  - Once a memory block is secured or mined out, player navigates to a new region (e.g., `change-sector L3`).
  - Advances the mission forward toward higher-tier caches and deeper server vaults, supporting a complete 20–30 minute run loop.
