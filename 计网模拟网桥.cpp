#include<iostream>
#include<fstream>
#include<string>
using namespace std;
struct data_item {					//ת����ṹ��
	string address = "";
	int port = 0;
};
/*����ѧϰ�㷨*/
void study(data_item data[], string addr, int p) {
	int i = 0;
	bool add_flag = true;
	while (data[i].address != "") {						//�鿴ת�����ǲ������д���
		if (data[i].address == addr) {
			add_flag = false;
			break;
		}
		i++;
	}
	if (add_flag) {
		data[i].address = addr;
		data[i].port = p;
	}
}
void transmit(data_item data[],string line,int p) {
	bool flag = true;
	int i = 0;
	string addr[2];
	addr[0] = line.substr(0, 12);			//��ȡĿ�ĵ�ַ
	addr[1] = line.substr(12, 12);			//��ȡԴ��ַ
	i = 0;
	while (i < 100 && data[i].address != "") {			//ѭ��ÿ�������봫������֡�����ж��Ƿ�ת����ֱ������Ϊ��
		//�ж�Ŀ�ĵ�ַ�Ƿ���ת������
		if (data[i].address == addr[0] && data[i].port != p) {			
				cout << "Ŀ�ĵ�ַ��" << addr[0] << " Դ��ַ��" << addr[1] << " ��֡�Ӷ˿�" << data[i].port << "ת����" << endl;
				study(data, addr[1], p);
				flag = false;
				break;
		}
		else if(data[i].address == addr[0] && data[i].port == p){
				cout << "Ŀ�ĵ�ַ��" << addr[0] << " Դ��ַ��" << addr[1] << " ��֡����������ͬһ�ӿڣ�" << endl;
				study(data, addr[1], p);
				flag = false;
				break;
		}
		i++;
	}
	if (flag) {			//���������ת���������ѧϰ
		study(data, addr[1], p);
		cout << "Ŀ�ĵ�ַ��" << addr[0] << " Դ��ַ��" << addr[1] << " ��֡��ת�����Ҳ�����ת���������˿�" << endl;
	}
}
int main() {
	data_item data[100];
	fstream file1,file2;
	file1.open("G:\\file1.txt", ios::in | ios::out | ios::app);
	if (!file1) {
		cout << "file1�ļ���ʧ�ܣ�" << endl;
		exit(1);
	}
	file2.open("G:\\file2.txt", ios::in | ios::out | ios::app);
	if (!file2) {
		cout << "file2�ļ���ʧ�ܣ�" << endl;
		exit(1);
	}
	string line1,line2;
	bool  exchange = true;
	int i;
	while (!file1.eof()	|| !file2.eof()) {
		if (exchange && !file1.eof()) {			//��ȡ�ļ�1,�����������ӵĶ˿ں�Ϊ1
			getline(file1, line1);				
			transmit(data, line1,1);
			exchange = !exchange;
		}
		else {									//��ȡ�ļ�2�������������ӵĶ˿ں�Ϊ2
			getline(file2, line2);
			transmit(data, line2,2);
			exchange = !exchange;
		}
		i = 0;
		cout << endl << "* ��ǰת��������Ϊ��" << endl;
		while (i < 100 && data[i].address != "") {
			cout << data[i].address << "		" << data[i].port << endl;
			i++;			
		}
		cout << endl;
	}
	file1.close();
	file2.close();
	system("pause");
}