//#include <iostream>
//#include <fstream>
//#include <unordered_set>
//#include <random>
//
//void GenerateUniqueRandomPointsToFile(const std::string& filename, int numPoints, int minCoord, int maxCoord)
//{
//    std::ofstream outfile(filename);
//    if (!outfile)
//    {
//        std::cerr << "Error: Unable to open file for writing." << std::endl;
//        return;
//    }
//
//    std::unordered_set<int> usedPoints;
//    std::random_device rd;
//    std::mt19937 generator(rd());
//    std::uniform_int_distribution<int> distribution(minCoord, maxCoord);
//
//    for (int i = 0; i < numPoints; ++i) {
//        int x, y;
//        do {
//            x = distribution(generator);
//            y = distribution(generator);
//        } while (!usedPoints.insert(x * (maxCoord + 1) + y).second);
//
//        outfile << x << " " << y << std::endl;
//    }
//
//    outfile.close();
//    std::cout << "Unique random points saved to " << filename << std::endl;
//}
//
//int main()
//{
//    int numPoints = 100; // Number of points
//    int minCoord = 1;      // Minimum coordinate value
//    int maxCoord = 100;  // Maximum coordinate value
//    std::string filename = "dataFile.txt"; // Output file name
//
//    GenerateUniqueRandomPointsToFile(filename, numPoints, minCoord, maxCoord);
//
//    return 0;
//}