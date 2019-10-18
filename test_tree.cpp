#include "KDTRIP.hpp"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

#include <cstdlib>
std::vector<KdTrip> dataList;

std::vector<KNode> datanode;
std::vector<int> res;

int freeNode = 1;
int key = 1;
int seg = 2;
int block = seg + 1;
int c_node = 0;
uint32_t pickup_timemin = UINT32_MAX;
uint32_t pickup_timemax = 0;
uint32_t dropoff_timemin = UINT32_MAX;
uint32_t dropoff_timemax = 0;

float pickup_longmin = FLT_MAX;
float pickup_longmax = -FLT_MAX;

float pickup_latmin = FLT_MAX;
float pickup_latmax = -FLT_MAX;

float dropoff_longmin = FLT_MAX;
float dropoff_longmax = -FLT_MAX;

float dropoff_latmin = FLT_MAX;
float dropoff_latmax = -FLT_MAX;

class range
{
public:
    uint32_t min = 0;
    uint32_t max = UINT32_MAX;
    // range(uint32_t min1,uint32_t max1){ min=min1;max=max1;}
};
std::vector<range> ra;
float uint2float(uint32_t f)
{
    register uint32_t u = f ^ (((f >> 31) - 1) | 0x80000000);
    return *((float*)&u);
}


bool isMatch(int i)
{
    KdTrip aa = dataList[i];
    //   int x = 0;
    //  while(x < ra.size()) {
    //		if(x>1){
    //			if(!(aa.getUKey(x) <= uint2float(ra[x].max) && aa.getUKey(x) >= uint2float(ra[x].min)))
    //{ 	    return false;
    //	}
    //
    //		}else{
    // if(!(aa.getUKey(0) <= ra[0].max && aa.getUKey(0) >= ra[0].min)) {
    //   return false;
    //}
	//std::cout<<<<std::endl;
    return (aa.getUKey(0) <= ra[0].max && aa.getUKey(0) >= ra[0].min) &&
        (aa.getUKey(1) <= ra[1].max && aa.getUKey(1) >= ra[1].min) ;//&&
        float2uint(aa.getUKey(2)) <= (ra[2].max) &&float2uint(aa.getUKey(2)) >= (ra[2].min) &&
        (float2uint(aa.getUKey(3)) <= ra[3].max && float2uint(aa.getUKey(3)) >= ra[3].min) &&
        (float2uint(aa.getUKey(4)) <= ra[4].max && float2uint(aa.getUKey(4)) >= ra[4].min) &&
        (float2uint(aa.getUKey(5)) <= ra[5].max && float2uint(aa.getUKey(5)) >= ra[5].min);
    //}
    //	++x;
    // }
    // return true;
}
//////////////////old version search tree//////////////////////
void searchTree(uint64_t index, int depth)
{
    // std::cout<<res.size()<<"  "<<index<<std::endl;
    KNode a = datanode[index];

    if(a.child_node == UINT32_MAX) {
	for(int i = a.median_value[0]; i < a.median_value[1]; i++)
	    if(isMatch(i))
		res.push_back(i);
	//	std::cout<<res.size()<<std::endl;
	return;
    }

    int rangeIndex = (depth) % 6;

    uint32_t median = a.median_value[0];

    uint64_t nextIndex = a.child_node;
    // std::cout<<"index "<<index<<" nextIndex "<<nextIndex<<std::endl;
    // std::cout<<"min "<<ra[rangeIndex].min <<" max "<<ra[rangeIndex].max<<" median "<<median<<std::endl;
    if(ra[rangeIndex].min <= median && ra[rangeIndex].max >= median) {
	//	std::cout<<"index "<<index<<" nextIndex left "<<nextIndex<<std::endl;
	searchTree(nextIndex, depth + 1);
	nextIndex = nextIndex + 1;
	// std::cout<<"index "<<index<<" nextIndex right "<<nextIndex<<std::endl;
	searchTree(nextIndex, depth + 1);

    } else {
	if(ra[rangeIndex].max < median) {
	    searchTree(nextIndex, depth + 1);
	}
	if(ra[rangeIndex].min > median) {
	    nextIndex = nextIndex + 1;

	    searchTree(nextIndex, depth + 1);
	}
    }
}
//////////////////new version search tree//////////////////////
void searchTree1(uint64_t index, int depth)
{

    KNode a = datanode[index];

    if(a.child_node == UINT32_MAX) {
	for(int i = a.median_value[0]; i < a.median_value[1]; i++) {
	    if(isMatch(i))
		res.push_back(i);
	}
	// std::cout<<res.size()<<" a.child_node "<<a.child_node<<std::endl;
	return;
    }

    int rangeIndex = (depth) % 6;

    int l = 0;
    int r = a.median_value.size();
    //  std::cout<<res.size()<<" ra[rangeIndex].min "<<ra[rangeIndex].min<<" a "<<a.median_value[l]<<std::endl;
    while(l < a.median_value.size()) {
	if(ra[rangeIndex].min > a.median_value[l])
	    l++;
	else {
	    break;
	}
    }
    // std::cout<<res.size()<<" ra[rangeIndex].min "<<ra[rangeIndex].min<<" a "<<a.median_value[l]<<std::endl;

    while(r > 0) {
	if(ra[rangeIndex].max < a.median_value[r - 1])
	    --r;
	else {
	    break;
	}
    }
    // std::cout<<"l "<<l<<"r "<<r<<std::endl;
    uint64_t nextIndex = a.child_node;
    for(int i = l; i <= r; i++) {
	//	std::cout<<"nextIndex+i "<<nextIndex+i<<std::endl;
	searchTree1(nextIndex + i, depth + 1);
    }

    //  uint32_t median = a.median_value[0];

    // std::cout<<"index "<<index<<" nextIndex "<<nextIndex<<std::endl;
    // std::cout<<"min "<<ra[rangeIndex].min <<" max "<<ra[rangeIndex].max<<" median "<<median<<std::endl;
    //    if(ra[rangeIndex].min <= median && ra[rangeIndex].max >= median) {
    //	//	std::cout<<"index "<<index<<" nextIndex left "<<nextIndex<<std::endl;
    //	searchTree(nextIndex, depth + 1);
    //	nextIndex = nextIndex + 1;
    //	// std::cout<<"index "<<index<<" nextIndex right "<<nextIndex<<std::endl;
    //	searchTree(nextIndex, depth + 1);
    //
    //    } else {
    //	if(ra[rangeIndex].max < median) {
    //	    searchTree(nextIndex, depth + 1);
    //	}
    //	if(ra[rangeIndex].min > median) {
    //	    nextIndex = nextIndex + 1;
    //
    //	    searchTree(nextIndex, depth + 1);
    //	}
    //    }
}

//////////////////////////////old tree/////////////////////////
void buildTree1(int depth, int index, int statindex, int endindex)
{
    //  std::cout<<dataList.size()<<std::endl;
    KNode node;
    node.create(2);
    c_node += 1;
    // std::cout<<"node.median_value.size() "<<node.median_value.size()<<std::endl;
    // node.median_value=new uint32_t[1];
    node.median_value[0] = UINT32_MAX - 1;
    //	std::cout<<"initial knode "<<node.child_node<<std::endl;

    if(endindex - statindex < block) {
	//	std::cout<<statindex<<std::endl;
	// std::cout<<"endindex "<<endindex<<std::endl;
	node.child_node = UINT32_MAX;
	node.median_value[0] = statindex;
	node.median_value[1] = endindex;
	//	std::cout << "node.median_value " << node.median_value[0] << std::endl;
	//	std::cout << endindex << std::endl;
	datanode[index] = node;
	// std::cout << index << std::endl;
	//	KNode aa=datanode[index];
	// std::cout <<".median_value[0] "<< aa.median_value[0]<< std::endl;
	return;
    }
    size_t medianIndex = (endindex - statindex) / 2 - 1;
    int tem = freeNode;
    int ch = depth % 6;
    key = ch;
    std::sort(dataList.begin() + statindex, dataList.begin() + endindex,
        [](KdTrip& one, KdTrip& two) { return one.getUKey(key) < two.getUKey(key); });
    node.child_node = tem;
    node.median_value[0] = dataList[statindex + medianIndex].getUKey(ch);
    datanode[index] = node;
    //    std::cout << "knode " << std::endl;
    //    std::cout << "knode index " << index << std::endl;
    //    std::cout << "knode child_node " << node.child_node << std::endl;
    //    std::cout << "knode median_value " << node.median_value[0] << std::endl;
    freeNode += 2;
    buildTree1(depth + 1, tem, statindex, statindex + medianIndex + 1);
    statindex = statindex + medianIndex + 1;
    buildTree1(depth + 1, tem + 1, statindex, endindex);
}
//////////////////////////////new tree/////////////////////////
void buildTree2(int depth, int index, int statindex, int endindex)
{
    //  std::cout<<dataList.size()<<std::endl;
    KNode node;
    node.create(seg);
    c_node += 1;
    // std::cout<<"index  "<< index << std::endl;
    // node.median_value=new uint32_t[1];
    node.median_value[0] = UINT32_MAX - 1;
    //	std::cout<<"initial knode "<<node.child_node<<std::endl;

    if(endindex - statindex < block) {
	//	std::cout<<statindex<<std::endl;
	// std::cout<<"endindex "<<endindex<<std::endl;
	node.child_node = UINT32_MAX;
	// for(int i=0;i<endindex - statindex)
	node.median_value[0] = statindex;
	node.median_value[1] = endindex;
	//	std::cout << "node.median_value " << node.median_value[0] << std::endl;
	//	std::cout << endindex << std::endl;
	datanode[index] = node;
	// std::cout << index << std::endl;
	//	KNode aa=datanode[index];
	// std::cout <<".median_value[0] "<< aa.median_value[0]<< std::endl;
	return;
    }
    size_t medianIndex = (endindex - statindex) / (seg + 1);
    int tem = freeNode;
    int ch = depth % 6;
    key = ch;
    std::sort(dataList.begin() + statindex, dataList.begin() + endindex,
        [](KdTrip& one, KdTrip& two) { return one.getUKey(key) < two.getUKey(key); });
    node.child_node = tem;
    int aa = statindex;
    for(int i = 0; i < seg; i++) {
	node.median_value[i] = dataList[aa + medianIndex - 1].getUKey(ch);
	aa = aa + medianIndex;
    }
    //  node.median_value[0] = dataList[statindex + medianIndex].getUKey(ch);
    // node.median_value[1] = dataList[statindex + medianIndex+medianIndex].getUKey(ch);
    datanode[index] = node;
    //    std::cout << "knode " << std::endl;
    //    std::cout << "knode index " << index << std::endl;
    //    std::cout << "knode child_node " << node.child_node << std::endl;
    //    std::cout << "knode median_value " << node.median_value[0] << std::endl;
    freeNode += (seg + 1);
    for(int i = 0; i < seg; i++) {
	buildTree2(depth + 1, tem + i, statindex, statindex + medianIndex);
	statindex = statindex + medianIndex;
    }
    ////  buildTree2(depth + 1, tem, statindex, statindex + medianIndex+1);
    //   buildTree2(depth + 1, tem+1, statindex+ medianIndex+1 , statindex+ 2*medianIndex+1);
    // statindex = statindex + medianIndex ;
    buildTree2(depth + 1, tem + seg, statindex, endindex);
}
// void buildTree2(int depth, int index, int statindex, int endindex)
//{
//    //  std::cout<<dataList.size()<<std::endl;
//    KNode node;
//    node.create(3);
//    // std::cout<<"node.median_value.size() "<<node.median_value.size()<<std::endl;
//    // node.median_value=new uint32_t[1];
//    node.median_value[0] = UINT32_MAX - 1;
//    //	std::cout<<"initial knode "<<node.child_node<<std::endl;
//
//    if(endindex - statindex < 2) {
//	//	std::cout<<statindex<<std::endl;
//	// std::cout<<"endindex "<<endindex<<std::endl;
//	node.child_node = statindex;
//	node.median_value[0] = UINT32_MAX;
//	//	std::cout << "node.median_value " << node.median_value[0] << std::endl;
//	//	std::cout << endindex << std::endl;
//	datanode[index] = node;
//	// std::cout << index << std::endl;
//	//	KNode aa=datanode[index];
//	// std::cout <<".median_value[0] "<< aa.median_value[0]<< std::endl;
//	return;
//    }
//    size_t medianIndex = (endindex - statindex) / (3) - 1;
//    int tem = freeNode;
//    int ch = depth % 6;
//    key = ch;
//    std::sort(dataList.begin() + statindex, dataList.begin() + endindex,
//        [](KdTrip& one, KdTrip& two) { return one.getUKey(key) < two.getUKey(key); });
//    node.child_node = tem;
//
//    datanode[index] = node;
//	int ss=statindex ;
//	 freeNode += (3);
//	for(int i=0;i<seg;i++){
//	 node.median_value[i] = dataList[ss + medianIndex].getUKey(ch);
//	int tempp=ss;
//	 ss=(ss + medianIndex<endindex)?ss + medianIndex:endindex;
//	 buildTree2(depth + 1, tem+i, tempp, ss + 1);
//
//	}
//
//
//    //    std::cout << "knode " << std::endl;
//    //    std::cout << "knode index " << index << std::endl;
//    //    std::cout << "knode child_node " << node.child_node << std::endl;
//    //    std::cout << "knode median_value " << node.median_value[0] << std::endl;
//
//
//}

int main()
{
    std::ifstream file;
    file.open("C:\\Users\\yuanchunyu\\Downloads\\test5mil.csv");
   //  file.open("C:\\Users\\yuanchunyu\\Downloads\\testdata4.csv");
    seg = 2;// 2 median value
    block = 1000; // kblock size

    std::string line = "";
    // Iterate through each line and split the content using delimeter
    int i = 0;
    dataList.clear();
    datanode.clear();

    std::vector<std::string> tokens;
    std::cout << "reading " << std::endl;
    while(getline(file, line)) {
	i++;
	if(i % 500000 == 0)
	    std::cout << i << std::endl;
	if(i > 1) {

	    // temp.pickup_time

	    // std::cout<<line<<std::endl;

	    tokens.clear();
	    std::string token;
	    std::istringstream tokenStream(line);
	    while(std::getline(tokenStream, token, ',')) {
		tokens.push_back(token);
	    }
	    // std::cout<<tokens[1]<<std::endl;

	    KdTrip temp(tokens);
	    //  temp.display();

	    dataList.push_back(temp);
	    pickup_timemin = (temp.pickup_time < pickup_timemin) ? temp.pickup_time : pickup_timemin;
	    pickup_timemax = (temp.pickup_time > pickup_timemax) ? temp.pickup_time : pickup_timemax;
	    dropoff_timemin = (temp.dropoff_time < dropoff_timemin) ? temp.dropoff_time : dropoff_timemin;
	    dropoff_timemax = (temp.dropoff_time > dropoff_timemax) ? temp.dropoff_time : dropoff_timemax;
	    pickup_longmin = (temp.pickup_long < pickup_longmin) ? temp.pickup_long : pickup_longmin;
	    pickup_longmax = (temp.pickup_long > pickup_longmax) ? temp.pickup_long : pickup_longmax;

	    pickup_latmin = (temp.pickup_lat < pickup_latmin) ? temp.pickup_lat : pickup_latmin;
	    pickup_latmax = (temp.pickup_lat > pickup_latmax) ? temp.pickup_lat : pickup_latmax;

	    dropoff_longmin = (temp.dropoff_long < dropoff_longmin) ? temp.dropoff_long : dropoff_longmin;
	    dropoff_longmax = (temp.dropoff_long > dropoff_longmax) ? temp.dropoff_long : dropoff_longmax;
	    dropoff_latmin = (temp.dropoff_lat < dropoff_latmin) ? temp.dropoff_lat : dropoff_latmin;
	    dropoff_latmax = (temp.dropoff_lat > dropoff_latmax) ? temp.dropoff_lat : dropoff_latmax;

	    // std::cout<<temp.getUKey(3)<<std::endl;
	    // std::cout<<dataList.size()<<std::endl;
	}
    }
    tokens.clear();
    std::cout << "pickup_timemin " << pickup_timemin << std::endl;
    std::cout << "pickup_timemax " << pickup_timemax << std::endl;
    std::cout << "dropoff_timemin " << dropoff_timemin << std::endl;
    std::cout << "dropoff_timemax " << dropoff_timemax << std::endl;
    std::cout << "pickup_latmin " << pickup_latmin << std::endl;
    std::cout << "pickup_latmax " << pickup_latmax << std::endl;
    std::cout << "pickup_longmin " << pickup_longmin << std::endl;
    std::cout << "pickup_longmax " << pickup_longmax << std::endl;
    std::cout << "dropoff_latmin " << dropoff_latmin << std::endl;
    std::cout << "dropoff_latmax " << dropoff_latmax << std::endl;
    std::cout << "dropoff_longmin " << dropoff_longmin << std::endl;
    std::cout << "dropoff_longmax " << dropoff_longmax << std::endl;

    datanode = std::vector<KNode>(dataList.size() * 4);
    // Close the File
    file.close();

    //  uint32_t* tmp = (uint32_t*)malloc(sizeof(uint32_t) * (dataList.size()));

    // KKnode* knode=new KKnode();
    // KNode* a = &(datanode->at(0));
    // KdTrip* b = &(dataList->at(0));
    std::cout << dataList.size() << std::endl;
    std::cout << datanode.size() << std::endl;
    std::cout << "building tree " << std::endl;
    c_node = 0;
    buildTree1(0, 0, 0, dataList.size());
    std::cout << "building tree finish " << c_node << " node " << std::endl;
    // std::cout << datanode.at(0).median_value[0] << std::endl;
    ra.resize(7);
    std::cout << ra[0].max << std::endl;
    ra[0].min = 1420074535;
    ra[0].max = 1420158147;
    ra[1].min = 1420070400;
    ra[1].max = 1420348999;
	  ra[2].min = 0;
	  float tf= 35.0379;
    ra[2].max = float2uint( tf);
    std::cout << "search tree " << std::endl;
    // std::cout << datanode[0].median_value[0] << std::endl;
    std::cout << "done" << res.size() << std::endl;
    i = 0;
    double ti = 0;
    while(i < 3) {
	res.clear();

	auto start = std::chrono::high_resolution_clock::now();
	searchTree(0, 0);
	auto finish = std::chrono::high_resolution_clock::now();
	// std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
	ti += std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
	std::cout << "done result " << res.size() << std::endl;
	i++;
    }
    std::cout << ti / 3 << " ms\n";
dataList[res.at(0)].display();
    res.clear();
    auto start = std::chrono::high_resolution_clock::now();
    for(int o = 0; o < dataList.size(); ++o) {
	if(isMatch(o)) {
	    res.push_back(o);
	}
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms\n";

    std::cout << "bruth force " << res.size() << std::endl;

    // std::cout << "aa kdtrip " << std::endl;

    // datanode.clear();
    // datanode.resize(dataList.size() * 3);
    // datanode = std::vector<KNode>(dataList.size() * 3);
    std::cout << datanode.size() << std::endl;
    for(int y = 2; y < 11; y++) {
	res.clear();
	i = 0;
	seg = y;
	std::cout << "building tree " << std::endl;

	c_node = 0;
	buildTree2(0, 0, 0, dataList.size());
	std::cout << "building tree finish " << c_node << " node " << std::endl;
	ti = 0;
	while(i < 3) {
	    res.clear();
	    start = std::chrono::high_resolution_clock::now();
	    searchTree1(0, 0);
	    finish = std::chrono::high_resolution_clock::now();
	    // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
	    ti += std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
	    std::cout << (i + 1) << " time done result search 1 " << seg << " " << res.size() << std::endl;
		std::cout <<  " kblock " << block  << std::endl;
	    i++;
	}
	std::cout << ti / 3 << "ms\n\n";
    }
    //	seg=5;
    //	buildTree2(0, 0, 0, dataList.size());
    //	while(i<3){
    // res.clear();
    //	  start = std::chrono::high_resolution_clock::now();
    //    searchTree1(0, 0);
    //   finish = std::chrono::high_resolution_clock::now();
    //   std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
    //
    //   std::cout << "done result search 1 " <<seg<<" "<< res.size() << std::endl;
    //   i++;
    //
    //	}
    //	//  buildTree2(0, 0, 0, dataList.size());
    //	  datanode.clear();
    //	   datanode = std::vector<KNode>(dataList.size() * 3);
    //	res.clear();
    //	  start = std::chrono::high_resolution_clock::now();
    //    searchTree1(0, 0);
    //   finish = std::chrono::high_resolution_clock::now();
    //    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
    //
    //    std::cout << "done result search 1 " << res.size() << std::endl;
    //	res.clear();
    //	  start = std::chrono::high_resolution_clock::now();
    //    searchTree1(0, 0);
    //   finish = std::chrono::high_resolution_clock::now();
    //    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
    //
    //    std::cout << "done result search 1 " << res.size() << std::endl;
    //    //	std::cout<<"aa candidateknode "<<aa->median_value<<std::endl;
    //    //	std::cout<<"a candidate "<<candidate->pickup_lat<<std::endl;
    //    //	std::cout<<"a candidate->display() "<<candidate ->dropoff_time<<std::endl;
    //
    //    std::fstream myfile;
    //    //    myfile = std::fstream("C:\\Users\\yuanchunyu\\Downloads\\ktrip.binary", std::ios::out |
    //    std::ios::binary);
    //    //    // Here would be some error handling
    //    //    // for(int i = 0; i < dataList->size(); ++i){
    //    //    // Some calculations to fill a[]
    //    //    myfile.write((char*)&(dataList.at(0)), (dataList.size()) * sizeof(KdTrip));
    //    //    // }
    //    //    myfile.close();
    //    //    std::fstream myfile1;
    //    //    myfile1 = std::fstream("C:\\Users\\yuanchunyu\\Downloads\\knode.binary", std::ios::out |
    //    std::ios::binary);
    //    //    // Here would be some error handling
    //    //    // for(int i = 0; i < dataList->size(); ++i){
    //    //    // Some calculations to fill a[]
    //    //    myfile1.write((char*)&(datanode.at(0)), (y) * sizeof(KNode));
    //    //    // }
    //    //    myfile1.close();
    //
    //    //	for(int i=0;i<3000;i++){
    //    //		if(datanode[i].median_value[0]==UINT32_MAX-1)
    //    //		{
    //    //			std::cout<<i<<std::endl;
    //    //			break;
    //    //		}
    //    //	}
    std::vector<KdTrip>().swap(dataList);
    std::vector<KNode>().swap(datanode);
    std::vector<int>().swap(res);

    // delete datanode;
    // delete dataList;
    // delete res;

    system("pause");
    return 0;
}
