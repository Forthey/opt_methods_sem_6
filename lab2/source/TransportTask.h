#pragma once

#include <string>
#include <vector>
#include <cstdint>


class TransportTask {
    enum FakeOne {
        Noone,
        Consumer,
        Supplier
    } fakeOne = Noone;

    void balanceTask();
    void computePotentials(const std::vector<std::vector<bool>> &isBasic, std::vector<double> &u, std::vector<double> &v) const;
    static bool dfsCycle(int cur_i, int cur_j, int start_i, int start_j,
              std::vector<std::vector<bool>> const &isBasic,
              std::vector<std::pair<int,int>> &cycle,
              std::vector<std::vector<bool>> &visited,
              bool horizontal);
    static std::vector<std::pair<int, int>> findCycle(int start_i, int start_j, std::vector<std::vector<bool> > const &isBasic);
public:
    std::vector<std::int64_t> suppliers;
    std::vector<std::int64_t> consumers;
    std::vector<std::vector<std::int64_t>> pathCosts;

    explicit TransportTask(std::string const& filename);
    std::vector<std::vector<std::int64_t>> northwestCornerMethod();

    std::vector<std::vector<std::int64_t>> potentialsMethod(std::vector<std::vector<std::int64_t>> &basicPlan) const;

    // std::int64_t calculateCosts()
};

