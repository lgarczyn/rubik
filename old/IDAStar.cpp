/*Solver::Result solve_loop(State &initial, int clean_steps) {
	auto stack = ida_star(initial);

	Solver::Result r;

	r.actual_state = stack.back();
	r.finished = true;
	State s = stack.front();
	for (auto state : stack) {
		print_diff(s.get_cube(), state.get_cube());
		s = state;
	}
	return r;
}*/

/*int fringe_search(State init) {
	init(start, goal)
    fringe F = s
    cache C[start] = (0, null)
    flimit = h(start)
    found = false

    while (found == false) AND (F not empty)
        fmin = ∞
        for node in F, from left to right
            (g, parent) = C[node]
            f = g + h(node)
            if f > flimit
                fmin = min(f, fmin)
                continue
            if node == goal
                found = true
                break
            for child in children(node), from right to left
                g_child = g + cost(node, child)
                if C[child] != null
                    (g_cached, parent) = C[child]
                    if g_child >= g_cached
                        continue
                if child in F
                    remove child from F
                insert child in F past node
                C[child] = (g_child, node)
            remove node from F
        flimit = fmin

    if reachedgoal == true
        reverse_path(goal)

}*/
/*path              current search path (acts like a stack)
 node              current node (last node in current path)
 g                 the cost to reach current node
 f                 estimated cost of the cheapest path (root..node..goal)
 h(node)           estimated cost of the cheapest path (node..goal)
 cost(node, succ)  step cost function
 is_goal(node)     goal test
 successors(node)  node expanding function, expand nodes ordered by g + h(node)
 ida_star(root)    return either NOT_FOUND or a pair with the best path and its cost*/

/*static const int FOUND = -1;

int search(
    std::vector<State> &path,
    std::unordered_set<State, custom_hash> &store,
    int cost,
    int bound) {
	State node = path.back();
	int f = cost + 8;
	if (f > bound)
		return f;
	if (node.is_final())
		return FOUND;
	int min = INT_MAX;
	Set candidates;
	node.get_candidates(candidates);
	for (auto succ : candidates) {
		if (store.find(succ) == store.end()) {
			path.push_back(succ);
			store.insert(succ); //TODO combine with check
			int t = search(path, store, cost + 8, bound);
			if (t == FOUND)
				return FOUND;
			if (t < min)
				min = t;
			path.pop_back();
			store.erase(succ);
		}
	}
	return min;
}

std::vector<State> ida_star(State initial) {
	int bound = 8;
	std::vector<State> path;
	path.push_back(initial);
	std::unordered_set<State, custom_hash> store;
	store.insert(initial);
	while (1) {
		int t = search(path, store, 0, bound);
		if (t == FOUND)
			return path;
		if (t == INT_MAX)
			throw std::logic_error("not found");
		bound = t;
	}
}*/