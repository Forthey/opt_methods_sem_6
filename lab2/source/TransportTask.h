#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "utility/custom_types.h"


class TransportTask {
    enum FakeOne {
        Noone,
        Consumer,
        Supplier
    } fakeOne = Noone;

    Row<double> suppliers;
    Row<double> consumers;
    Table<double> pathCosts;

    void parseFileData(std::string const& data);

    void balanceTask();

    void computePotentials(const std::vector<std::vector<bool> > &isBasic, std::vector<double> &u,
                           std::vector<double> &v) const;

    static bool dfsCycle(std::size_t cur_i, std::size_t cur_j, std::size_t start_i, std::size_t start_j,
                         Table<bool> const &isBasic,
                         std::vector<std::pair<std::size_t, std::size_t>> &cycle, Table<bool> &visited,
                         bool horizontal);

    static std::vector<std::pair<std::size_t, std::size_t>> findCycle(std::size_t start_i, std::size_t start_j,
                                                                       Table<bool> const &isBasic);
public:
    explicit TransportTask(std::string const &filename);

    Row<double> getSuppliers() const { return suppliers; }
    Row<double> getConsumers() const { return consumers; }
    Table<double> getPathCosts() const { return pathCosts; }

    Table<double> northwestCornerMethod();

    Table<double> potentialsMethod(Table<double> &basicPlan) const;

    double calculateCosts(Table<double> const& plan);
};
