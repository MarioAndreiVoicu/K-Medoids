#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<random>

struct DataPoint
{
    int x, y;
};

bool operator==(const DataPoint& lhs, const DataPoint& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void ReadData(std::vector<DataPoint>& pointVector)
{
    std::ifstream file("dataFile.txt");

    if (!file.is_open()) {
        throw std::runtime_error("Error opening dataFile.txt");
    }

    DataPoint point;
    while (file >> point.x >> point.y)
        pointVector.push_back(point);

    file.close();
}

int RandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}

void InitialMedoids(int k, const std::vector<DataPoint>& pointVector, std::vector<DataPoint>& initialMedoids)  //this function tries to get the most optimal initial medoids
{
    std::vector<int> medoidIndexes;
    for (int i = 0; i < k; i++)
    {
        int randomMedoidIndex = RandomNumber(0, pointVector.size() - 1);

        while (std::find(medoidIndexes.begin(), medoidIndexes.end(), randomMedoidIndex) != medoidIndexes.end())  //check so it doesn't chose the same medoid twice
            randomMedoidIndex = RandomNumber(0, pointVector.size() - 1);

        medoidIndexes.push_back(randomMedoidIndex);
    }

    for (int index : medoidIndexes)
        initialMedoids.push_back(pointVector[index]);
}

void InitialMatrix(std::vector<std::vector<DataPoint>>& clusterMatrix, const std::vector<DataPoint>& medoidVector, const std::vector<DataPoint>& pointVector)  //this function inserts the best clusters with the given medoids inside the matrix
{
    clusterMatrix.resize(medoidVector.size());  //the matrix has k lines (k medoids), each line being a cluster

    for (int index1 = 0; index1 < pointVector.size(); index1++)  //go trough all the points and see which medoid is the closest to each point
    {
        int bestCluster = 0, minDistance = std::abs(pointVector[index1].x - medoidVector[0].x) + std::abs(pointVector[index1].y - medoidVector[0].y);
        for (int index2 = 1; index2 < medoidVector.size(); index2++)  //go trough each medoid to see which is the closest to the current point
        {
            int currentDistance = std::abs(pointVector[index1].x - medoidVector[index2].x) + std::abs(pointVector[index1].y - medoidVector[index2].y);
            if (currentDistance < minDistance)
            {
                minDistance = currentDistance;
                bestCluster = index2;
            }
        }
        clusterMatrix[bestCluster].push_back(pointVector[index1]);
    }
}

int ClusterCost(const std::vector<DataPoint>& clusterPoints, const DataPoint& currentMedoid)  //this function calculates the cost of a cluser,which means the total sum of distances from each point to the medoid
{
    int cost = 0;
    for (int i = 0; i < clusterPoints.size(); i++)  //go trough all the points and add to the cost the distance between the point and the medoid
        cost += std::abs(clusterPoints[i].x - currentMedoid.x) + std::abs(clusterPoints[i].y - currentMedoid.y);  //Manhattan distance formula

    return cost;
}

void BestMedoids(int k, const std::vector<std::vector<DataPoint>>& clusterMatrix, std::vector<DataPoint>& bestMedoidVector)  //this function finds the best medoid for each cluster
{  //the function goes into each cluster and tries different medoids to see which is the most optimal
    bestMedoidVector.clear();

    for (int i = 0; i < k; i++) //K-Medoids means there will be K clusters
    {
        DataPoint bestMedoid = clusterMatrix[i][0];
        int bestCost = ClusterCost(clusterMatrix[i], bestMedoid), currentCost = bestCost;
        if (clusterMatrix[i].size() > 100)    //if there are more than 100 points in the cluster,instead of going trough all of them it will go trough 100 points and chose the most optimal medoid from those
            for (int j = 1; j <= 100; j++)
            {
                currentCost = ClusterCost(clusterMatrix[i], clusterMatrix[i][j]);
                if (currentCost < bestCost)
                {
                    bestCost = currentCost;
                    bestMedoid = clusterMatrix[i][j];
                }
            }
        else  //if there are less than 100 points in the cluster,it checks all of them to find the best medoid
            for (int j = 1; j < clusterMatrix[i].size(); j++)
            {
                currentCost = ClusterCost(clusterMatrix[i], clusterMatrix[i][j]);
                if (currentCost < bestCost)
                {
                    bestCost = currentCost;
                    bestMedoid = clusterMatrix[i][j];
                }
            }

        bestMedoidVector.push_back(bestMedoid);
    }
}

int BestClustersCost(int k, std::vector<DataPoint>& pointVector, std::vector<DataPoint>& bestMedoids, std::vector<std::vector<DataPoint>>& clusterMatrix)  //this function makes the matrix with the best clusters for each K value and returns the total cost
{
    InitialMedoids(k, pointVector, bestMedoids);
    InitialMatrix(clusterMatrix, bestMedoids, pointVector);
    BestMedoids(k, clusterMatrix, bestMedoids);

    int totalCost = 0;
    for (int i = 0; i < bestMedoids.size(); i++)
        totalCost += ClusterCost(clusterMatrix[i], bestMedoids[i]);

    return totalCost;
}

int OptimalClusterCount(std::vector<std::vector<DataPoint>>& clusterMatrix, std::vector<DataPoint>& bestMedoids, std::vector<DataPoint>& pointVector)  //this function calculates the best number of clusters.It keeps making more clusters until there are diminshing returns and it is not worth making more clusters
{
    int k = 2;
    float previousCost = BestClustersCost(k, pointVector, bestMedoids, clusterMatrix);

    clusterMatrix.clear();
    bestMedoids.clear();

    k = 3;
    float currentCost = BestClustersCost(k, pointVector, bestMedoids, clusterMatrix);
    if (currentCost > previousCost - 0.10 * previousCost)
        return k - 1;

    for (int k = 4; k <= 10; k++)  //it stops at maximum 10 clusters
    {
        clusterMatrix.clear();
        bestMedoids.clear();

        previousCost = currentCost;
        currentCost = BestClustersCost(k, pointVector, bestMedoids, clusterMatrix);

        if (currentCost > previousCost - 0.10 * previousCost)  // 0.10 checks if by adding one more cluster if the total cost is 10% better than before.If it is not,it stops adding clusters,if it is,it keeps adding clusters until the elbow point is hit and by adding more clusters it does not improve by more than 10 %
            return k - 1;
    }

    return k;
}

void DisplayClusterMatrix(std::vector<std::vector<DataPoint>>& clusterMatrix, const std::vector<DataPoint>& bestMedoidVector)
{
    for (int i = 0; i < bestMedoidVector.size(); i++)
    {
        std::cout << "Cluster " << i << " has the medoid " << bestMedoidVector[i].x << " " << bestMedoidVector[i].y << " and contains the points: ";
        for (int j = 0; j < clusterMatrix[i].size(); j++)
            std::cout << clusterMatrix[i][j].x << " " << clusterMatrix[i][j].y << "   ";
        std::cout << "\n";
    }
}

int main()
{
    std::vector<DataPoint> pointVector;  //this contains the coordonates of all the points
    std::vector<std::vector<DataPoint>> clusterMatrix; //this contains the clusters on the lines
    std::vector<DataPoint> bestMedoids;

    ReadData(pointVector);

    int k = OptimalClusterCount(clusterMatrix, bestMedoids, pointVector);

    DisplayClusterMatrix(clusterMatrix, bestMedoids);

    return 0;
}