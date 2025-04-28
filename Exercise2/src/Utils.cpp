#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{

bool ImportMesh(PolygonalMesh& mesh) //Prende una struttura PolygonalMesh vuota da riempire
{
    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;
}


bool ImportCell0Ds(PolygonalMesh& mesh)
{
    std::ifstream file("Cell0Ds.csv");

    if(file.fail())
        return false;

    std::list<std::string> listLines;  // crea una lista per salvare tutte le righe
    std::string line;
    while (std::getline(file, line)) // legge il file riga per riga
        listLines.push_back(line);  // aggiunge ogni riga al fondo della lista

    file.close();

    // Remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();  // salva il numero di punti trovati

    if (mesh.NumCell0Ds == 0)
    {
        std::cerr << "There is no cell 0D" << std::endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds); // prepara spazio per Cell0DsId (vettore degli ID)
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds); // crea matrice delle coordinate 3xN con x,y,z=0

    for (std::string& line : listLines) // legge ogni riga della lista listLines
    {
		replace(line.begin(), line.end(), ';', ' ');
        std::istringstream converter(line); // converte la riga in uno stream di dati separabili

        unsigned int id;
        unsigned int marker;
		Vector2d coord;

        converter >>  id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);
        // converter >> id: estrae il primo valore numerico dalla riga e lo memorizza in id
		// >> marker: legge il prossimo valore numerico e lo memorizza in marker 
		// >> mesh.Cell0DsCoordinates(0, id) prende il terzo valore della riga(che rappresenta la coodrdinata x) e lo memorizza nell'elemento della matriche che si trova alla riga 0 e alla colonna id
        // >> mesh.Cell0DsCoordinates(1, id) prende il quarto valore della riga (che rappresenta la coordinata y) e lo memorizza nell'elemento della matrice che si trova alla riga 1 e alla colonna id

        mesh.Cell0DsId.push_back(id); //mette l'id nel vettore degli id

        // Memorizza i Marker
        if(marker != 0) // se il marker è diverso da 0 controlla se è gia presente nell map (tipo dizionario di python) definita nel file PolygonalMesh
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}}); // se non esiste chrea una nuova voce nella map
            }
            else
            {
                it->second.push_back(id); // se esite aggiunge l'id
            }
        }
    }

    return true;
}

// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    std::ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    std::list<std::string> listLines;
    std::string line;
    while (std::getline(file, line))
        listLines.push_back(line);

    file.close();

    // Remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        std::cerr << "There is no cell 1D" << std::endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds); //matrice per momorizzare i vertici estremi

    for (std::string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        std::istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >>  mesh.Cell1DsExtrema(0, id) >>  mesh.Cell1DsExtrema(1, id);
      

        mesh.Cell1DsId.push_back(id);

        // Marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
    }

    return true;
}

// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    std::ifstream file("./Cell2Ds.csv");

    if(file.fail())
        return false;

    std::list<std::string> listLines;
    std::string line;
    while (std::getline(file, line))
        listLines.push_back(line);

    file.close();

    // Remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        std::cerr << "There is no cell 2D" << std::endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);
	
    for (std::string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        std::istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int numVertices;
        std::vector<unsigned int> vertices;
        unsigned int numEdges;
        std::vector<unsigned int> edges;

        converter >> id >> marker >> numVertices;

    
        for (unsigned int i = 0; i < numVertices; ++i)
        {
            unsigned int vertex;
			converter >> vertex;
			vertices.push_back(vertex);
        }

        converter >> numEdges;
        for (unsigned int i = 0; i < numEdges; ++i)
        {
            unsigned int edge;
			converter >> edge;
			edges.push_back(edge);
        }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);

        // Marker (opzionale anche per Cell2D)
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
    }

    return true;
}
}
