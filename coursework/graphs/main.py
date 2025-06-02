from dataclasses import dataclass
import json
from typing import Callable

import matplotlib.pyplot as pyplot


@dataclass
class AlgorithmIteration:
    min_cost:   int
    time:       float


class AlgorithmData:
    def __init__(self, name: str):
        self.name:      str = name
        
        self.__result:  list[tuple[int, AlgorithmIteration]] = []
    
    def __str__(self):
        return f"Algorith {self.name} with {len(self.__result)} data"
    
    def __repr__(self):
        return self.__str__()

    def add(self, iteration: int, data: AlgorithmIteration):
        self.__result.append((iteration, data))

    def get_cost(self) -> list[tuple[int, int]]:
        return [(iteration, data.min_cost) for iteration, data in self.__result]

    def get_time(self) -> list[tuple[int, int]]:
        return [(iteration, data.time) for iteration, data in self.__result]


def parse_json(filename: str) -> list[dict]:
    with open(filename) as file:
        data = json.load(file)
    
    return data


def create_models(data: list[dict]) -> tuple[tuple[AlgorithmData], AlgorithmData]:
    result: dict[str, AlgorithmData] = {}
    
    for iteration_data in data:
        iteration: int = int(iteration_data["graph_size"])

        for graph_data in iteration_data["results"]:
            if (name := graph_data["name"]) not in result:
                result[name] = AlgorithmData(name)
            
            result[name].add(iteration, AlgorithmIteration(
                min_cost=graph_data["min_cost"],
                time=graph_data["est_calc_time_seconds"]
            ))
    
    return tuple(result.values()), result["held_karp"]


def create_graph_by_data(x_axis: list, y_axis: list):
    pyplot.plot(x_axis, y_axis)


def process_data(data: list[tuple]):
    x_axis, y_axis = zip(*data)

    create_graph_by_data(x_axis, y_axis)


def create_graph(
        models: tuple[AlgorithmData],
        function: Callable[[], list[tuple[int, int | float]]],
        legend: list[str],
        name: str
    ):

    pyplot.xticks([])
    pyplot.yticks([])

    for model in models:
        process_data(function(model))

    pyplot.legend(legend)
    pyplot.title(name)


def create_graphs(models: tuple[AlgorithmData], best_model: AlgorithmData):
    legend: list[str] = []

    for model in models:
        legend.append(model.name)    
    
    pyplot.subplot(2, 1, 1)

    def right_data(model: AlgorithmData) -> list[tuple[int, int]]:
        result: list[tuple[int, int]] = []

        for index, (iteration, value) in enumerate(model.get_cost()):
            result.append((iteration, abs(best_model.get_cost()[index][1] - value)))
        
        return result
    
    create_graph(models, right_data, legend, "График зависимости ошибки от размера графа")

    pyplot.subplot(2, 1, 2)

    cost_function: Callable[[AlgorithmData], list[tuple[int, int]]] = lambda model: model.get_time()
    create_graph(models, cost_function, legend, "График зависимости времени работы алгоритма от размера графа")

    pyplot.show()


def main():
    filename: str = input("Enter filename with result: ")

    if not filename:
        filename = "../results.json"

    data: list[dict] = parse_json(filename)
    models, best_model = create_models(data)

    create_graphs(models, best_model)


if __name__ == "__main__":
    main()
