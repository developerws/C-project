#include<iostream>
#include<fstream>
#include<string>
using namespace std;
struct data_item {					//转发表结构体
	string address = "";
	int port = 0;
};
/*自我学习算法*/
void study(data_item data[], string addr, int p) {
	int i = 0;
	bool add_flag = true;
	while (data[i].address != "") {						//查看转发表是不是已有此项
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
	addr[0] = line.substr(0, 12);			//提取目的地址
	addr[1] = line.substr(12, 12);			//提取源地址
	i = 0;
	while (i < 100 && data[i].address != "") {			//循环每个表项与传进来的帧进行判断是否转发，直到表项为空
		//判断目的地址是否在转发表中
		if (data[i].address == addr[0] && data[i].port != p) {			
				cout << "目的地址：" << addr[0] << " 源地址：" << addr[1] << " 的帧从端口" << data[i].port << "转发！" << endl;
				study(data, addr[1], p);
				flag = false;
				break;
		}
		else if(data[i].address == addr[0] && data[i].port == p){
				cout << "目的地址：" << addr[0] << " 源地址：" << addr[1] << " 的帧丢弃，处于同一接口！" << endl;
				study(data, addr[1], p);
				flag = false;
				break;
		}
		i++;
	}
	if (flag) {			//如果不发生转发，则进行学习
		study(data, addr[1], p);
		cout << "目的地址：" << addr[0] << " 源地址：" << addr[1] << " 的帧在转发表找不到，转发到其他端口" << endl;
	}
}
int main() {
	data_item data[100];
	fstream file1,file2;
	file1.open("G:\\file1.txt", ios::in | ios::out | ios::app);
	if (!file1) {
		cout << "file1文件打开失败！" << endl;
		exit(1);
	}
	file2.open("G:\\file2.txt", ios::in | ios::out | ios::app);
	if (!file2) {
		cout << "file2文件打开失败！" << endl;
		exit(1);
	}
	string line1,line2;
	bool  exchange = true;
	int i;
	while (!file1.eof()	|| !file2.eof()) {
		if (exchange && !file1.eof()) {			//读取文件1,且与网桥连接的端口号为1
			getline(file1, line1);				
			transmit(data, line1,1);
			exchange = !exchange;
		}
		else {									//读取文件2，且与网桥连接的端口号为2
			getline(file2, line2);
			transmit(data, line2,2);
			exchange = !exchange;
		}
		i = 0;
		cout << endl << "* 当前转发表内容为：" << endl;
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