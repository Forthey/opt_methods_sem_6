import numpy as np
import random


POPULATION_SIZE = 100
GENERATIONS = 500
MUTATION_RATE = 0.02
ELITISM = True
ELITISM_SIZE = 2


CITIES = np.array([
    [60, 200], [180, 200], [80, 180], [140, 180], [20, 160],
    [100, 160], [200, 160], [140, 140], [40, 120], [100, 120]
])
NUM_CITIES = len(CITIES)


def calculate_distance(city1: np.ndarray, city2: np.ndarray) -> float:
    return np.linalg.norm(city1 - city2)


def total_distance(route: list[int]) -> float:
    distance = 0.0
    for i in range(NUM_CITIES):
        from_city = CITIES[route[i]]
        to_city = CITIES[route[(i + 1) % NUM_CITIES]]
        distance += calculate_distance(from_city, to_city)
    return distance


def generate_individual() -> list[int]:
    individual = list(range(NUM_CITIES))
    random.shuffle(individual)
    return individual


def initialize_population() -> list[list[int]]:
    return [generate_individual() for _ in range(POPULATION_SIZE)]


def fitness(individual: list[int]) -> float:
    return 1.0 / total_distance(individual)


def tournament_selection(population: list[list[int]], k: int = 3) -> list[int]:
    selected = random.sample(population, k)
    return min(selected, key=lambda x: total_distance(x))


def ordered_crossover(parent1: list[int], parent2: list[int]) -> list[int]:
    size = len(parent1)
    start, end = sorted(random.sample(range(size), 2))
    child = [-1] * size

    child[start:end] = parent1[start:end]
    
    pointer = 0
    for i in range(size):
        if child[i] == -1:
            while parent2[pointer] in child:
                pointer += 1
            child[i] = parent2[pointer]
    return child


def swap_mutation(individual: list[int]) -> list[int]:
    if random.random() < MUTATION_RATE:
        idx1, idx2 = random.sample(range(len(individual)), 2)
        individual[idx1], individual[idx2] = individual[idx2], individual[idx1]
    return individual


def evolve_population(population: list[list[int]]) -> list[list[int]]:
    new_population = []
    
    if ELITISM:
        elites = sorted(population, key=lambda x: total_distance(x))[:ELITISM_SIZE]
        new_population.extend(elites)


    while len(new_population) < POPULATION_SIZE:
        parent1 = tournament_selection(population)
        parent2 = tournament_selection(population)
        child = ordered_crossover(parent1, parent2)
        child = swap_mutation(child)
        new_population.append(child)
    
    return new_population


def genetic_algorithm() -> tuple[list[int], float]:
    population = initialize_population()
    best_individual = None
    best_distance = float('inf')
    
    for generation in range(GENERATIONS):
        population = evolve_population(population)
        

        current_best = min(population, key=lambda x: total_distance(x))
        current_distance = total_distance(current_best)
        
        if current_distance < best_distance:
            best_individual = current_best.copy()
            best_distance = current_distance
            print(f"Generation {generation}: Best distance = {best_distance:.2f}")
    
    return best_individual, best_distance


def main():
    best_route, distance = genetic_algorithm()
    print("Лучший найденный маршрут:")
    print(" -> ".join(map(str, best_route)) + f" -> {best_route[0]}")
    print(f"Общая длина: {distance:.2f}")


if __name__ == "__main__":
    main()
