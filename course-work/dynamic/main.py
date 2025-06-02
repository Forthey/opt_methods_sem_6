import math
from itertools import combinations
from typing import Tuple, List, Dict

def held_karp_tsp(distances: List[List[float]]) -> Tuple[float, List[int]]:
    n = len(distances)
    
    C: Dict[frozenset, List[float]] = {}
    
    S = frozenset({0})
    C[S] = [math.inf] * n
    C[S][0] = 0
    
    for subset_size in range(2, n + 1):
        for subset in combinations(range(1, n), subset_size - 1):
            S = frozenset({0, } | set(subset))
            
            C[S] = [math.inf] * n
            
            for j in S:
                if j == 0:
                    continue
                
                S_without_j = S - {j}
                
                min_cost = math.inf
                for i in S_without_j:
                    cost = C[S_without_j][i] + distances[i][j]
                    if cost < min_cost:
                        min_cost = cost
                
                C[S][j] = min_cost
    
    full_set = frozenset(range(n))
    
    min_tour_cost = math.inf
    last_node = -1
    for j in range(1, n):
        cost = C[full_set][j] + distances[j][0]
        if cost < min_tour_cost:
            min_tour_cost = cost
            last_node = j
    
    tour = []
    remaining_set = full_set
    tour.append(last_node)
    
    for _ in range(n - 1):
        S = remaining_set - {tour[-1]}
        
        # Ищем предыдущий узел в маршруте
        min_cost = math.inf
        prev_node = -1
        for j in S:
            cost = C[S][j] + distances[j][tour[-1]]
            if cost < min_cost:
                min_cost = cost
                prev_node = j
        
        tour.append(prev_node)
        remaining_set = S
    
    tour.append(0)
    tour = tour[::-1]
    
    return min_tour_cost, tour


if __name__ == "__main__":
    dist = [
        [0, 2, 9, 10],
        [1, 0, 6, 4],
        [15, 7, 0, 8],
        [6, 3, 12, 0]
    ]
    
    cost, path = held_karp_tsp(dist)
    print(f"Минимальная стоимость: {cost}")
    print(f"Оптимальный маршрут: {path}")