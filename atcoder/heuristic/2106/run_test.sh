#!/usr/bin/env bash
# =============================================================================
# Benchmark runner for AtCoder 2106 - Castle Renovation
# Compares solution_1 vs solution_2 across multiple test cases
#
# Usage:
#   ./run_test.sh [NUM_TESTS] [START_SEED]
#
# Examples:
#   ./run_test.sh            # 20 tests, seeds 1..20
#   ./run_test.sh 50         # 50 tests, seeds 1..50
#   ./run_test.sh 20 100     # 20 tests, seeds 100..119
# =============================================================================

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NUM_TESTS="${1:-20}"
START_SEED="${2:-1}"

# ------------- Colors ---------------------------------------------------------
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'

echo -e "${BOLD}${CYAN}========================================${NC}"
echo -e "${BOLD}${CYAN}  AtCoder 2106 - Performance Benchmark  ${NC}"
echo -e "${BOLD}${CYAN}========================================${NC}"
echo ""

# ------------- Build ----------------------------------------------------------
echo -e "${YELLOW}[STEP 1] Compiling solutions...${NC}"

g++ -O2 -std=c++17 -o "$DIR/gen" "$DIR/gen.cpp"
echo -e "  ${GREEN}✓${NC} gen compiled"

g++ -O3 -std=c++17 -march=native -o "$DIR/sol1" "$DIR/solution_1.cpp"
echo -e "  ${GREEN}✓${NC} solution_1 compiled"

g++ -O3 -std=c++17 -march=native -o "$DIR/sol2" "$DIR/solution_2.cpp"
echo -e "  ${GREEN}✓${NC} solution_2 compiled"

echo ""

# ------------- Workspace ------------------------------------------------------
TMPDIR="$DIR/bench_tmp"
mkdir -p "$TMPDIR"

# ------------- Helper: run with timeout (cross-platform) ----------------------
run_with_timeout() {
    local BIN="$1"
    local INP="$2"
    local OUT="$3"
    local TOUT="${4:-5}"
    
    python3 - "$BIN" "$INP" "$OUT" "$TOUT" <<'PYEOF'
import subprocess, sys

bin_path = sys.argv[1]
inp_path = sys.argv[2]
out_path = sys.argv[3]
timeout_s = float(sys.argv[4])

try:
    with open(inp_path, 'r') as fin, open(out_path, 'w') as fout:
        result = subprocess.run([bin_path], stdin=fin, stdout=fout,
                                stderr=subprocess.DEVNULL, timeout=timeout_s)
    sys.exit(result.returncode)
except subprocess.TimeoutExpired:
    sys.exit(124)
except Exception as e:
    print(f"Error: {e}", file=sys.stderr)
    sys.exit(1)
PYEOF
}

# ------------- Helper: parse judge output (single line: "T score") -----------
parse_result() {
    python3 -c "
import sys, re
data = sys.stdin.read()
tm = re.search(r'T=([^,\s]+)', data)
sm = re.search(r'score=(\d+)', data)
print((tm.group(1) if tm else 'ERR') + ' ' + (sm.group(1) if sm else '0'))
"
}

# ------------- Run tests ------------------------------------------------------
echo -e "${YELLOW}[STEP 2] Running ${NUM_TESTS} test cases (seeds ${START_SEED} to $((START_SEED + NUM_TESTS - 1)))...${NC}"
echo ""

# Header
printf "%-8s  %-14s %-14s  %-14s %-14s  %s\n" \
    "Seed" "Sol1 T" "Sol2 T" "Sol1 Score" "Sol2 Score" "Winner"
printf "%-8s  %-14s %-14s  %-14s %-14s  %s\n" \
    "--------" "--------------" "--------------" "--------------" "--------------" "-------"

TOTAL_S1=0
TOTAL_S2=0
WIN_S1=0
WIN_S2=0
TIE=0
FAIL_S1=0
FAIL_S2=0

for i in $(seq 0 $((NUM_TESTS - 1))); do
    SEED=$((START_SEED + i))
    INPUT="$TMPDIR/input_${SEED}.txt"
    OUT1="$TMPDIR/out1_${SEED}.txt"
    OUT2="$TMPDIR/out2_${SEED}.txt"

    # Generate input
    "$DIR/gen" "$SEED" > "$INPUT"

    # Run solution 1
    if run_with_timeout "$DIR/sol1" "$INPUT" "$OUT1" 5; then
        RES1=$(python3 "$DIR/judge.py" "$INPUT" "$OUT1" 2>/dev/null || echo "T=ERR, score=0")
        read T1 S1 <<< "$(echo "$RES1" | parse_result)"
    else
        T1="TLE/ERR"; S1=0
        FAIL_S1=$((FAIL_S1 + 1))
    fi

    # Run solution 2
    if run_with_timeout "$DIR/sol2" "$INPUT" "$OUT2" 5; then
        RES2=$(python3 "$DIR/judge.py" "$INPUT" "$OUT2" 2>/dev/null || echo "T=ERR, score=0")
        read T2 S2 <<< "$(echo "$RES2" | parse_result)"
    else
        T2="TLE/ERR"; S2=0
        FAIL_S2=$((FAIL_S2 + 1))
    fi

    TOTAL_S1=$((TOTAL_S1 + S1))
    TOTAL_S2=$((TOTAL_S2 + S2))

    # Determine winner for this test
    if [[ "$S1" -gt "$S2" ]]; then
        WIN_S1=$((WIN_S1 + 1))
        WINNER="${GREEN}Sol1${NC}"
    elif [[ "$S2" -gt "$S1" ]]; then
        WIN_S2=$((WIN_S2 + 1))
        WINNER="${RED}Sol2${NC}"
    else
        TIE=$((TIE + 1))
        WINNER="${YELLOW}TIE${NC}"
    fi

    printf "%-8s  %-14s %-14s  %-14s %-14s  " \
        "$SEED" "$T1" "$T2" "$S1" "$S2"
    echo -e "$WINNER"
done

# ------------- Summary --------------------------------------------------------
echo ""
echo -e "${BOLD}${CYAN}========================================${NC}"
echo -e "${BOLD}${CYAN}              FINAL RESULTS             ${NC}"
echo -e "${BOLD}${CYAN}========================================${NC}"
echo ""

printf "  %-30s %d\n" "Total tests:" "$NUM_TESTS"
echo ""
printf "  %-30s %s\n" "Solution 1 total score:" "$TOTAL_S1"
printf "  %-30s %s\n" "Solution 2 total score:" "$TOTAL_S2"
echo ""
printf "  %-30s %d\n" "Sol1 wins:" "$WIN_S1"
printf "  %-30s %d\n" "Sol2 wins:" "$WIN_S2"
printf "  %-30s %d\n" "Ties:" "$TIE"

if [[ "$FAIL_S1" -gt 0 ]]; then
    echo -e "  ${RED}Solution 1 failures: ${FAIL_S1}${NC}"
fi
if [[ "$FAIL_S2" -gt 0 ]]; then
    echo -e "  ${RED}Solution 2 failures: ${FAIL_S2}${NC}"
fi

echo ""
echo -e "${BOLD}  === VERDICT ===${NC}"
if [[ "$TOTAL_S1" -gt "$TOTAL_S2" ]]; then
    DIFF=$((TOTAL_S1 - TOTAL_S2))
    echo -e "  ${GREEN}${BOLD}✓ SOLUTION 1 IS BETTER${NC}"
    echo -e "  ${GREEN}  Score advantage: +${DIFF} over ${NUM_TESTS} tests${NC}"
elif [[ "$TOTAL_S2" -gt "$TOTAL_S1" ]]; then
    DIFF=$((TOTAL_S2 - TOTAL_S1))
    echo -e "  ${RED}${BOLD}✓ SOLUTION 2 IS BETTER${NC}"
    echo -e "  ${RED}  Score advantage: +${DIFF} over ${NUM_TESTS} tests${NC}"
else
    echo -e "  ${YELLOW}${BOLD}= BOTH SOLUTIONS ARE EQUAL${NC}"
fi

echo ""
echo -e "${CYAN}Temp files saved in: ${TMPDIR}${NC}"
