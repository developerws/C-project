#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<time.h>
using namespace std;
void FCFS(vector<int> list,int length,int init_visit) {
	int total = 0, cur_visit = init_visit;
	cout << "FCFS�㷨�Ĵ�ͷ�ƶ��켣Ϊ��" << endl;
	cout << init_visit << " ";
	for (int i = 0; i < length; i++) {
		cout << list[i] << " ";
	}
	cout << endl << "�ƶ����ܴŵ����� ";
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
	cout << "SSTF�㷨�Ĵ�ͷ�ƶ��켣Ϊ��" << endl;
	cout << cur_visit << " ";
	for (int i = 0; i < length; i++) {
		min_dis = 3600;
		for (int j = 0; j < list.size();j++) {						//�ҳ����뵱ǰ�������λ��
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
	cout << endl << "�ƶ����ܴŵ����� " << total << endl << endl;
}
void lift(vector<int> list, int length, int init_visit) {
	sort(list.begin(), list.end());
	int i = 0;
	while (i < length) {
		if (init_visit < list[i])
			break;
		i++;
	}
	cout << "���ݷ��Ĵ�ͷ�ƶ��켣Ϊ��" << endl << init_visit << " ";
	int cur_sub = i - 1, total = 0, cur_visit = init_visit;
	while (cur_sub >= 0) {							//������߷����ƶ�
		cout << list[cur_sub] << " ";
		total += cur_visit - list[cur_sub];
		cur_visit = list[cur_sub];
		cur_sub--;
	}
	cur_sub = i;
	while (cur_sub < length) {				//�����ұ߷����ƶ�
		cout << list[cur_sub] << " ";
		total += list[cur_sub] - cur_visit;
		cur_visit = list[cur_sub];
		cur_sub++;
	}
	cout << endl << "�ƶ����ܴŵ����� " << total << endl;
}
int main(){
	srand((unsigned int)time(0));
	vector<int> list;
	int track_length, init_visit, temp;
	cout << "�����ͷ��ʼλ�ã�";
	cin >> init_visit;
	cout << endl << "����ŵ����г��ȣ�";
	cin >> track_length;
	cout << "�ŵ����У�";
	for (int i = 0; i < track_length; i++) {				//�����������
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