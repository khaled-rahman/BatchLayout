#include "ogdf/energybased/FMMMLayout.h"
#include "ogdf/fileformats/GraphIO.h"
#include <string>
#include <omp.h>
using namespace std;
using namespace ogdf;

int main(int argc, char *argv[])
{
    Graph G;
    string graph = "3elt_dual.gml";
    string input = argv[1];
    string output = argv[2];
    GraphAttributes GA(G);
    if (!GraphIO::read(G, input)) {
        std::cerr << "Could not load 3elt_dual.gml" << std::endl;
        return 1;
    }
    for (node v : G.nodes)
        GA.width(v) = GA.height(v) = 10.0;
    //omp_set_num_threads(47);
    double start = omp_get_wtime();
    FMMMLayout fmmm;
    fmmm.useHighLevelOptions(true);
    fmmm.unitEdgeLength(15.0);
    fmmm.newInitialPlacement(true);
    fmmm.fixedIterations(500);
    fmmm.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    fmmm.call(GA);
    double end = omp_get_wtime();
    cout << "Time: " << end - start << " seconds" << endl;
    GraphIO::write(GA, output, GraphIO::writeGML);
    cout << endl;
    return 0;
}

