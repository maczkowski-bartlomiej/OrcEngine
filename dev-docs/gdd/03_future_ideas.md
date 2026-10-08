# ByteBurst™ — Future Ideas & Systems Backlog
### Consolidated Archive of Mechanics, Expansions, and Systems (Organized)

This document collects all creative ideas developed during concept design, organized by gameplay system for future prototyping and tuning.

---

## 1. Enemy Ecosystem & Mutation Mechanics

### A. Metamorphic Elite Viruses
- **Concept**: Elite boss viruses running "Metamorphic Code Engines" that physically alter their geometric shape and vulnerabilities mid-battle.
- **Morphology Phases**:
  - *Phase 1 (Hexagon Shield)*: Frontal bullet deflection; vulnerable only to rear attacks or electric arc sentinels.
  - *Phase 2 (Spiked Ring)*: Emits expanding radial shockwaves; vulnerable to long-range piercing lasers.
  - *Phase 3 (Hyper-Cube / Tesseract)*: Blinks through narrow corridors leaving trails of burning corrupted data.

### B. The Virus Signature Database (`sigdb`)
- **Concept**: Defeated and scanned viruses drop telemetry data that feeds a persistent bestiary/database.
- **Mechanic**:
  - Scanning viruses fills a research bar: `Trojan.AVS: [||||||....] 60%`.
  - Hitting 100% completion triggers an alert allowing the player to pick 1 permanent research artifact (e.g. *Null-Pointer Piercing*, *Heuristic Firewall*, *Garbage Collector*).

### C. Server-Specific Virus Ecosystems
- **Database Servers (SQL Vaults)**: Feature *SQL Injection Leeches* (drain stored cycles directly from memory tables) and *Deadlock Phantoms* (freeze adjacent corridors).
- **Web / Proxy Servers**: Feature fast-spawning *DDoS Botnet Clusters* (swarms of fragile micro-triangles).
- **Crypto-Mining Nodes**: Feature *Proof-of-Work Parasites* that rapidly overheat CPU caches, introducing thermal throttle hazards.

### D. Corrupted Data (Dynamic Hazard & Mystery Box)
- **Concept**: When viruses miss shots or die, they drop glitch fragments that solidify into destructible **Corrupted Data** blocks.
- **The Risk/Reward Gamble**:
  Shooting or clearing a corrupted block rolls a weighted mystery table:
  - 40%: Feral Glitch/Virus spawns *(Risk)*
  - 30%: Raw Cycles/Bytes harvested *(Economy)*
  - 15%: Rare Firmware Script/Artifact drops *(Jackpot)*
  - 15%: Random System Event (*RAM Flush*, *Overvoltage Damage Spike*, or *Kernel Panic*)

---

## 2. Extraction & Run Pacing (Push-Your-Luck)

### A. Escalating Threat & Multipliers
- Server risk escalates the longer a player remains in memory:
  - *Minutes 0–5*: Low threat, stable clock, standard payout (x1.0).
  - *Minutes 6–15*: Metamorphic mutations awaken, temperature climbs, loot multiplier rises (x2.0).
  - *Minutes 15–25*: Critical overload, high-tier loot drops, extreme hazard (x3.5+).

### B. Extraction Methods
- **The 20-Second Downlink Hold**: Initiating `extract` starts an exit handshake. The player must hold their position and defend the exit port against a surge of alarmed hostiles.
- **Emergency Hard Reboot (`reboot --hard`)**: An instant emergency rip-cord that severs the connection immediately to save the player's Rig from destruction, but sacrifices 50% of unbanked mission loot.

---

## 3. Advanced Terminal & Automation Systems

### A. Command & Script Taxonomy
Defining the exact operational boundaries between:
1. **OS Built-ins**: Native hardcoded kernel commands (`scan`, `move-right`, `change-sector`, `status`).
2. **Script Executables**: Software inventory items slotted in `~/scripts/` (e.g. `scripts/eco/miner.sh`, `scripts/combat/sentinel.bin`).
3. **Sub-Processes / Daemons**: Background tasks running on hardware threads that players monitor, boost, or terminate via `top` and `kill`.

### B. Hardware Thread Sockets (Boost Slots)
Rig CPU threads possess specialized architectural traits that boost slotted scripts:
- **Overclock Socket**: +35% tick rate / fire speed, but generates extra heat.
- **DMA Fast-Bus**: +60% packet throughput for miners and extractors.
- **Cryo-Cooled Core**: Zero heat generation for continuous monitoring tools.
- **Quantum Co-Processor**: Automatically bypasses 1 layer of enemy encryption.

### C. Batch Startup Execution (`exec-dir`)
- Grouping scripts into a folder (e.g. `scripts/startup/*`) allows the player to launch their entire operational profile with a single command upon entering a server:
  `exec scripts/startup/*`

### D. Decommissioned Server Racks (Idle Metagame)
- In mid/late-game, players can purchase conquered server racks to run passive background mining loops:
  `cron --background "mine data on server_101"`
- Generates passive income between active infiltration contracts.

---

## 4. Alternative Time & Control Mechanics

### A. Pausable Tactical Clock (`Spacebar` to Pause)
- Pressing `Spacebar` freezes in-game time.
- The player can calmly study enemy positions, inspect memory ranges, and type orders without time pressure.
- Unpausing executes the actions.

### B. Discrete Tick Mode (Action-Driven Time)
- Time only advances by 1 "Clock Tick" when the player inputs a command.
- Enemies telegraph their moves (e.g. *"Trojan will reach Node 2 in 2 ticks"*), turning encounters into a tactical puzzle.
