#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<time.h>
using namespace std;
void FCFS(vector<int> list,int length,int init_visit) {
	int total = 0, cur_visit = init_visit;
	cout << "FCFS算法的磁头移动轨迹为：" << endl;
	cout << init_visit << " ";
	for (int i = 0; i < length; i++) {
		cout << list[i] << " ";
	}
	cout << endl << "移动的总磁道数： ";
	for (int i = 0; i < length; i++) {
		total += cur_visit > list[i] ? (cur_visit - list[i]) : (list[i] - cur_visit);
		cur_visit = list[i];
	}
	cout << total << endl << endl;
}
void SSTF(vector<int> list, int length, int init_visit) {
	int min_dis, dis, sub, cur_visit;
	int total = 0;
	cur_visit = init_visit;
	cout << "SSTF算法的磁头移动轨迹为：" << endl;
	cout << cur_visit << " ";
	for (int i = 0; i < length; i++) {
		min_dis = 3600;
		for (int j = 0; j < list.size();j++) {						//找出距离当前道最近的位置
			dis = cur_visit > list[j] ? (cur_visit - list[j]) : (list[j] - cur_visit);
			if (min_dis > dis) {					
				min_dis = dis;
				sub = j;
			}
		}
		total += min_dis;
		cur_visit = list[sub];
		cout << cur_visit << " ";
		list.erase(list.begin() + sub);	
	}
	cout << endl << "移动的总磁道数： " << total << endl << endl;
}
void lift(vector<int> list, int length, int init_visit) {
	sort(list.begin(), list.end());
	int i = 0;
	while (i < length) {
		if (init_visit < list[i])
			break;
		i++;
	}
	cout << "电梯法的磁头移动轨迹为：" << endl << init_visit << " ";
	int cur_sub = i - 1, total = 0, cur_visit = init_visit;
	while (cur_sub >= 0) {							//先往左边方向移动
		cout << list[cur_sub] << " ";
		total += cur_visit - list[cur_sub];
		cur_visit = list[cur_sub];
		cur_sub--;
	}
	cur_sub = i;
	while (cur_sub < length) {				//后往右边方向移动
		cout << list[cur_sub] << " ";
		total += list[cur_sub] - cur_visit;
		cur_visit = list[cur_sub];
		cur_sub++;
	}
	cout << endl << "移动的总磁道数： " << total << endl;
}
int main(){
	srand((unsigned int)time(0));
	vector<int> list;
	int track_length, init_visit, temp;
	cout << "输入磁头初始位置：";
	cin >> init_visit;
	cout << endl << "输入磁道序列长度：";
	cin >> track_length;
	cout << "磁道序列：";
	for (int i = 0; i < track_length; i++) {				//随机产生序列
		temp = rand() % 200 + 1;
		cout << temp << " ";
		list.push_back(temp);
	}
	cout << endl << endl;
	FCFS(list,track_length, init_visit);
	SSTF(list, track_length, init_visit);
	lift(list, track_length, init_visit);
	system("pause");
}