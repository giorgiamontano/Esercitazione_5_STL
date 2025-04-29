#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"  // Per l'esportazione nei formati richiesti

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;  // Usa il namespace corretto per la mesh poligonale

int main()
{
    PolygonalMesh mesh;

    // Carica la mesh poligonale (cellule 0D, 1D, 2D)
    if (!ImportMesh(mesh)) 
	{
        cerr << "File not found" << endl;
        return 1;
    }

    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp",
                           mesh.Cell0DsCoordinates);

    utilities.ExportSegments("./Cell1Ds.inp",	
                             mesh.Cell0DsCoordinates,
							 mesh.Cell1DsExtrema);
    


    return 0;

}