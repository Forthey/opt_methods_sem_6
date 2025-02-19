#include "LinearTask.h"

LinearTask::LinearTask(Matrix const &A, Matrix const &B, std::vector<double> const &b1, std::vector<double> const &b2, std::vector<double> const &c, double v,
                       std::vector<std::size_t> const &notRestrictedVarIndices)
    : n(A.getCols()), c(c), v(v), A(A), b(b1) {

    removeEqualities(B, b2);
    addVarRestrictions(notRestrictedVarIndices);
}

void LinearTask::removeEqualities(Matrix const &B, std::vector<double> const &b2) {
    for (int i = 0; i < B.getRows(); i++) {
        A.extendDown(B[i]);
        b.push_back(b2[i]);

        std::vector<double> negativeB(B[i].size());
        for (int j = 0; j < negativeB.size(); j++) {
            negativeB[j] = -B[i][j];
        }
        A.extendDown(negativeB);
        b.push_back(-b2[i]);
    }
}

void LinearTask::addVarRestrictions(std::vector<std::size_t> const &notRestrictedVarIndices) {
    for (std::size_t index : notRestrictedVarIndices) {
        splitVarIndices.emplace(index, A.getCols());

        c.push_back(0.0);
        A.extendRight();

        c.back() = -c[index];
        for (std::vector<double>& v : A.getData()) {
            v.back() = -v[index];
        }
    }
}

std::vector<double> LinearTask::getTrueValues(std::vector<double> const &x) {
    std::vector<double> trueX(n);
    for (int i = 0; i < n; i++) {
        if (splitVarIndices.contains(i)) {
            trueX[i] = x[i] - x[splitVarIndices[i]];
        } else {
            trueX[i] = x[i];
        }
    }

    return trueX;
}

LinearTask LinearTask::getDualTask() const {
    std::vector<double> dualC(b.size()), dualB(c.size());
    for (int i = 0; i < b.size(); i++) {
        dualC[i] = -b[i];
    }
    for (int i = 0; i < c.size(); i++) {
        dualB[i] = -c[i];
    }
    // return {A.transform(), Matrix{}, dualB, {}, dualC, v};
    return *this;
}
