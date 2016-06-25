# Kmeans
abstract:
  Simple Kmeans algorithm, Implemented in C++，Support "sqeuclidean","cosine","haming distance". 

run main.cpp
if you want to use this in you project, you can add new namespace and add these function to your own project.

further:
this algorithm not give an very efficient implementation, if you need, you can use kd-tree or ball tree,
combine with parallel implementation.

try to merge some categories to reduce the Total SSE will be better if you don't know the number of specific categories.


kmeans算法的C++实现，支持欧式，余玄，海明距离衡量，可输出每个类的误差平方和以及总的误差平方和
值得注意的是: kmeans的效果往往与初始点的选择有很大关系，读者可加入个性化的选择方式，另外可考虑
kd-tree以及并行实现
