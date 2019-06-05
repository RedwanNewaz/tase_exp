#include <iostream>
#include <unordered_map>
#include <vector>
#include <iterator>

#include <utility>
#include <thread>
#include <chrono>
#include "include/PolicyReader.h"
#include "include/ValidatePolicy.h"
#include "include/PomdpPolicyChecker.h"
#include "FetchActionClient.h"

#ifdef GUI
#include <QApplication>
#endif

using namespace std;

int main(int argc, char *argv[]) {

#ifdef GUI
    QApplication app(argc, argv);
#endif
    FetchActionClient actionClient(argc,argv);
    string folderPath = actionClient.getFolderPath();
    PolicyReader reader(folderPath);
    reader.readFiles();
////    std::cout<<reader<<endl;
    auto b_0 = beliefstruct{0,0,1.0};
    auto Goal = beliefstruct{2,2,1.0};
    auto Gt = reader.GroundTruth;

    ValidatePolicy check(Goal,Gt);
    check.GetValidPolicy(reader.Policy);
    vector<CPtrs>data;
    for (int j = 0; j < reader.Policy.size(); ++j) {
        data.push_back(reader.Policy[j][0]);

    }

    actionClient(data,Gt);

    return 0;
}