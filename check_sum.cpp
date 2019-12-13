#include<iostream>
#include <bitset>
#include <sstream>
#include<fstream>
#include<string>
using namespace std;
/*二进制序列相加*/
string add_bin(string s1,string s2,int &carry_sum) {			
	string sum = "0000000000000000";
	int temp;
	int carry = 0;
	for (int i = 16 - 1; i >= 0; i--) {
		temp = ((s1[i] - '0') + (s2[i] - '0') + carry) % 2;
		carry = ((s1[i] - '0') + (s2[i] - '0') + carry) / 2;
		sum[i] = temp + '0';
	}
	if (carry)							//进位的累积
	{
		carry_sum += carry;
	}
	return sum;
}
/*二进制的序列转为十六进制*/
string bin_to_hex(int b1,int b2,int b3,int b4) {
	int hex;
	string s;
	hex = b1 * 8 + b2 * 4 + b3 * 2 + b4 * 1;
	if (hex < 10) {
		s = hex + '0';
	}
	else {
		switch (hex) {
		case 10:
			s = 'A';
			break;
		case 11:
			s = 'B';
			break;
		case 12:
			s = 'C';
			break;
		case 13 :
			s = 'D';
			break;
		case 14:
			s = 'E';
			break;
		case 15:
			s = 'F';
			break;	
		}
	}
	return s;
}
int main() {
	ifstream infile;
	infile.open("G:\\infile.txt", ios::in);
	if (!infile) {
		cout << "infile文件打开失败！" << endl;
		exit(1);
	}
	char ch;
	//从文件中提取一行内容到buf变量中
	ch = infile.get();
	string buf;
	while (ch != EOF) {
		if(ch != '\n')
			buf += ch;
		ch = infile.get();
	}
	int s_length;
	s_length = buf.length();
	bool flag = false;								
	if (s_length % 2 != 0) {				//如果数据字节长度为奇数，则长度+1
		s_length++;
		flag = true;
	}
	string *s = new string[s_length/2];					//用来存储每对字符的16位整数序列
	stringstream ss;
	int i = 0, count = 1, c = 0;
	while (i < s_length && buf[i] != NULL) {
		bitset<8> bit = buf[i];					//将每个字符转为8bit的二进制
		s[c] += bit.to_string();				//两两组合拼接在一起
		count++;
		if (count == 3) {
			count = 1;
			c++;
		}
		i++;
	}
	if (flag) {											//补一个字节的0以凑成偶数
		bitset<8> b(0);
		s[c] += b.to_string();
		cout << s[c] << endl;
	}	
	/*校验和相加*/
	string check_sum_bin = "0000000000000000";
	int carry_sum = 0;					//产生的进位总和
	for (i = 0; i < s_length / 2; i++) {
		check_sum_bin = add_bin(check_sum_bin, s[i],carry_sum);
	}
	/*总的进位一起加到最后的效验和中*/
	for (i = 0; i < carry_sum; i++) {
		check_sum_bin = add_bin(check_sum_bin, "0000000000000001",carry_sum);
	}
	cout << "最终校验和的二进制形式：" << check_sum_bin << endl;
	string check_sum = "";
	i = 0;
	while (check_sum_bin[i] != NULL) {
		check_sum += bin_to_hex(check_sum_bin[i] - '0', check_sum_bin[i+1] - '0', check_sum_bin[i+2] - '0', check_sum_bin[i+3] - '0');
		i = i + 4;
	}
	cout << "十六进制形式：" << check_sum << endl;
	delete[]s;
	system("pause");
}