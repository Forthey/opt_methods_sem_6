#pragma once

#include <string>
#include <vector>


class TransportTask {
    enum FakeOne {
        Noone,
        Consumer,
        Supplier
    } fakeOne = Noone;

    void balanceTask();
public:
    std::vector<std::size_t> suppliers;
    std::vector<std::size_t> consumers;
    std::vector<std::vector<std::size_t>> pathCosts;

    explicit TransportTask(std::string const& filename);
    void northwestCornerMethod();
};

