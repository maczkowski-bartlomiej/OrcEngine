# ByteBurst™ — Home Rig (Base Operations)
### Game Design Document: Between-Run Management & Preparation

---

## 1. Overview & Vision
Between missions, the player operates from their **Home Rig** (the base terminal). This is the safe, calm preparation phase where the player reviews their inventory of scripts, manages their virtual file system, configures CPU hardware threads, and selects their next contract before deploying into an infected server.

---

## 2. The SysAdmin Terminal Panel
The interface is a clean, monospace terminal panel divided into three primary navigation views/tabs:

```
+-----------------------------------------------------------------------------+
| OPERATOR BASE: [HOST: LOCALHOST] [CREDITS: $1,450] [RIG CPU: 4 THREADS]     |
+-----------------------------------------------------------------------------+
| TABS: [1: contracts]  [2: filesystem]  [3: scripts]                         |
+-----------------------------------------------------------------------------+
```

---

## 3. Core Features Breakdown

### Feature 1: The Contract Board (`[contracts]`)
* **Purpose**: Allows the player to browse, evaluate, and select available server missions.
* **Possible Namings**: *Contracts*, *Incident Tickets*, *Bounties*, *Infiltration Jobs*.
* **Mechanics**:
  - Displays 3–4 procedurally generated contracts.
  - Each entry lists:
    - **Target Server**: Name/ID (e.g., `OmniCorp_DB_01`, `Subnet_Omega`).
    - **Threat Diagnostic**: Detected virus type and threat percentage (e.g. `Trojan.AVS [70%]`).
    - **Payout**: Base reward upon successful extraction (e.g., `$1,200 Credits`).
    - **Risk Rating**: Class C (Medium), Class B (High), Class A (Critical).
* **CLI Interaction**:
  - `contracts` (lists available contracts).
  - `select contract #1` or `connect 1` (accepts the contract and transitions to the in-game mission).

---

### Feature 2: File System & Organization (`[filesystem]`)
* **Purpose**: A tactile virtual file system (`~/scripts/`) where the player inspects, renames, and organizes acquired software tools and scripts.
* **Possible Namings**: *File System*, *Storage Drive*, *Toolbox*, *Binaries Folder*.
* **Mechanics**:
  - Player organizes scripts into folders (e.g. `scripts/eco/`, `scripts/combat/`, `scripts/defense/`).
  - Player can rename scripts to their liking (e.g. `mv raw_miner.sh eco/fast_miner.sh`).
  - Underlying descriptions, tiers, and base stats remain permanently intact.
* **CLI Interaction**:
  - `ls [directory]` (lists files and folders).
  - `mkdir <folder>` (creates a directory).
  - `mv <src> <dest>` (moves or renames a file).
  - `cat <file>` (displays script description, tier, and cooldown stats).

---

### Feature 3: Hardware Thread Sockets (`[scripts]`)
* **Purpose**: Assigning executable software tools and automated daemons to physical CPU threads on the Rig.
* **Possible Namings**: *Thread Slots*, *Execution Sockets*, *CPU Allocation*, *Active Daemons*.
* **Mechanics**:
  - The Rig starts with a limited number of CPU execution threads (e.g. 2–4 threads).
  - Each active script slotted into a thread is loaded into the operator's runtime memory.
  - Slotted scripts become immediately available as quick commands or background automated routines during the live run (e.g. `deploy-miner`, `deploy-sentinel`).
* **CLI Interaction**:
  - `threads` (displays currently allocated and idle threads).
  - `slot <thread_id> <script_path>` (assigns a script to a thread).
  - `unslot <thread_id>` (frees a thread).

---

## 4. UI Layout Mockup (Home Rig)

```
+-----------------------------------------------------------------------------+
| BYTEBURST OPERATING ENVIRONMENT v1.0                                        |
+-----------------------------------------------------------------------------+
| > contracts                                                                 |
|                                                                             |
| ACTIVE CONTRACTS:                                                           |
| [#1] OmniCorp_SQL_Vault    | Threat: Trojan.AVS [70%]      | Bounty: $1,200 |
| [#2] Metro_Logistics_Node  | Threat: Worm.Replicator [55%] | Bounty: $1,800 |
| [#3] Apex_Cache_Cluster    | Threat: Glitch.Corrupt [85%]  | Bounty: $2,400 |
|                                                                             |
| > ls ~/scripts                                                              |
| [DIR] eco/      [DIR] combat/      [DIR] utility/                           |
|                                                                             |
| > cat ~/scripts/eco/miner.sh                                                |
| NAME:        miner.sh (Tier 1)                                              |
| TYPE:        Automated Harvester                                            |
| DESCRIPTION: Deploys a stationary cycle extractor onto a labeled node.     |
|                                                                             |
| > select contract #1                                                        |
| [SYS] Connecting to OmniCorp_SQL_Vault... Preparing memory tunnel...        |
+-----------------------------------------------------------------------------+
```
