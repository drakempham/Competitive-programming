import sys
from collections import deque


def antifragile_execution():
    raw_dimensions = input().split()
    grid_dim, num_relics, ops_limit = int(raw_dimensions[0]), int(
        raw_dimensions[1]), int(raw_dimensions[2])

    # Grid obstacles
    vert_shields = [input().strip() for _ in range(grid_dim)]
    horiz_shields = [input().strip() for _ in range(grid_dim - 1)]

    coords_1 = {}
    coords_2 = {}
    for k in range(num_relics):
        parts = input().split()
        b, c, d, e = map(int, parts)
        coords_1[k] = (b, c)
        coords_2[k] = (d, e)

    # Collision detection
    def is_shielded(r1, c1, r2, c2):
        if r1 == r2:
            return vert_shields[r1][min(c1, c2)] == '1'
        else:
            return horiz_shields[min(r1, r2)][c1] == '1'

    # Shortest path routing via BFS
    def hyper_warp_bfs(start_node, end_node):
        if start_node == end_node:
            return []

        exploration_queue = deque([start_node])
        trace_map = {start_node: None}
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

        while exploration_queue:
            r, c = exploration_queue.popleft()
            if (r, c) == end_node:
                break

            for dr, dc in directions:
                nr, nc = r + dr, c + dc
                if 0 <= nr < grid_dim and 0 <= nc < grid_dim and (nr, nc) not in trace_map:
                    if not is_shielded(r, c, nr, nc):
                        trace_map[(nr, nc)] = (r, c)
                        exploration_queue.append((nr, nc))

        if end_node not in trace_map:
            return []

        optimal_trajectory = []
        current_waypoint = end_node
        while current_waypoint != start_node:
            optimal_trajectory.append(current_waypoint)
            current_waypoint = trace_map[current_waypoint]
        optimal_trajectory.reverse()
        return optimal_trajectory

    # Translate route to machine instructions
    def compile_to_machine_code(trajectory, start_point, current_orientation):
        if not trajectory:
            return "", current_orientation

        compass = {(0, 1): 0, (1, 0): 1, (0, -1): 2, (-1, 0): 3}
        instruction_buffer = []
        temp_pos = start_point

        for next_step in trajectory:
            dr = next_step[0] - temp_pos[0]
            dc = next_step[1] - temp_pos[1]
            required_heading = compass[(dr, dc)]

            angle_diff = (required_heading - current_orientation) % 4
            if angle_diff == 1:
                instruction_buffer.append('R')
            elif angle_diff == 2:
                instruction_buffer.extend(['R', 'R'])
            elif angle_diff == 3:
                instruction_buffer.append('L')

            instruction_buffer.append('F')
            current_orientation = required_heading
            temp_pos = next_step

        return ''.join(instruction_buffer), current_orientation

    luke_bot_pos = (0, 0)
    bot_orientation = 0
    payload_held = None

    execution_tensor = []
    unresolved_relics = list(range(num_relics))

    while unresolved_relics:
        if payload_held is None:
            # Smart Greedy: Find the closest relic using actual path length
            target_relic = None
            best_trajectory = None
            min_distance = float('inf')

            for k in unresolved_relics:
                trial_path = hyper_warp_bfs(luke_bot_pos, coords_1[k])
                if len(trial_path) < min_distance:
                    min_distance = len(trial_path)
                    best_trajectory = trial_path
                    target_relic = k

            move_ops, bot_orientation = compile_to_machine_code(
                best_trajectory, luke_bot_pos, bot_orientation)
            execution_tensor.append(move_ops)
            luke_bot_pos = coords_1[target_relic]

            execution_tensor.append('S')
            payload_held = target_relic
        else:
            # Deliver to corresponding altar
            k = payload_held
            delivery_path = hyper_warp_bfs(luke_bot_pos, coords_2[k])
            move_ops, bot_orientation = compile_to_machine_code(
                delivery_path, luke_bot_pos, bot_orientation)

            execution_tensor.append(move_ops)
            luke_bot_pos = coords_2[k]

            execution_tensor.append('S')
            payload_held = None
            unresolved_relics.remove(k)

    raw_operation_sequence = ''.join(execution_tensor)

    # --- MACRO COMPRESSION (SCORE OPTIMIZATION) ---
    def tensor_core_compression(sequence):
        max_ops_saved = 0
        optimal_substring = ""
        seq_length = len(sequence)

        for pattern_len in range(3, min(30, seq_length)):
            for i in range(seq_length - pattern_len):
                sub_pattern = sequence[i:i + pattern_len]
                occurrences = sequence.count(sub_pattern)

                if occurrences > 1:
                    # Mathematical formula for ops saved:
                    # K*L - (L + K + 1)
                    ops_saved = (occurrences * pattern_len) - \
                        (pattern_len + occurrences + 1)
                    if ops_saved > max_ops_saved:
                        max_ops_saved = ops_saved
                        optimal_substring = sub_pattern

        if max_ops_saved > 0:
            first_idx = sequence.find(optimal_substring)
            prefix = sequence[:first_idx]
            macro_registration = "M" + optimal_substring + "M"
            remainder = sequence[first_idx + len(optimal_substring):]

            remainder = remainder.replace(optimal_substring, "P")
            return prefix + macro_registration + remainder

        return sequence

    final_optimized_ops = tensor_core_compression(raw_operation_sequence)
    for op_char in final_optimized_ops[:ops_limit]:
        print(op_char)


if __name__ == '__main__':
    antifragile_execution()
