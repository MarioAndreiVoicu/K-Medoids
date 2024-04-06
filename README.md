# K-Medoids Clustering

Overview
This project implements K-Medoids clustering, a partitioning method used to classify data into clusters.
Here,the data is represented by  In K-Medoids, 'K' represents the number of clusters the algorithm aims to identify.K-Medoids uses data points (medoids) to represent each cluster. The algorithm works iteratively to minimize the sum of distances between each data point and its corresponding medoid, forming clusters based on similarity.

How K-Medoids Works

Initialization: Randomly select 'K' data points to serve as initial medoids.

Cluster Assignment: Assign each data point to the nearest medoid, forming 'K' clusters.

Medoid Update: For each cluster, select the data point that minimizes the total distance to other points in the cluster. Update the medoid with this new data point.

Optimal K Determination: After the initial clustering, the algorithm will try different values of 'K' to find the optimal number of clusters. This is done by calculating the total cost (sum of distances) for each 'K' and identifying the point where the improvement by adding clusters diminishes by a pre-defined percent, known as the elbow point. At the elbow point, adding more clusters does not significantly improve the clustering solution, indicating the optimal number of clusters for the dataset has been found.

From this project I learned about grouping large datasets using partitioning clustering.I better understood the importance of heuristic solutions, realizing their advantages over exhaustive searches, particularly in scenarios involving large or complex problems, or when resources are limited.
