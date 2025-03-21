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
    Row<double> penalty;

    void parseFileData(std::string const& data);

    void balanceTask();

    void computePotentials(const std::vector<std::vector<bool> > &isBasic, std::vector<double> &u,
                           std::vector<double> &v) const;

    static bool dfsCycle(std::size_t curI, std::size_t curJ, std::size_t startI, std::size_t startJ,
                         Table<bool> const &isBasic,
                         std::vector<std::pair<std::size_t, std::size_t>> &cycle, Table<bool> &visited,
                         bool horizontal);

    static std::vector<std::pair<std::size_t, std::size_t>> findCycle(std::size_t startI, std::size_t startJ,
                                                                       Table<bool> const &isBasic);

    void checkForMultiple(Table<double> &optimalPlan, Table<bool> &isBasic) const;
public:
    explicit TransportTask(std::string const &filename);

    Row<double> getSuppliers() const { return suppliers; }
    Row<double> getConsumers() const { return consumers; }
    Table<double> getPathCosts() const { return pathCosts; }

    Table<double> northwestCornerMethod();

    Table<double> potentialsMethod(Table<double> &basicPlan) const;

    double calculateCosts(Table<double> const& plan) const;

    Table<double> restorePlan(Table<double> const& plan) const;
};
