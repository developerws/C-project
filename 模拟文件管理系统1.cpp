#include<iostream>
#include<sstream>
#include<iomanip>
#include<fstream>
#include<string>
using namespace std;
#define MAX_user 5					//最大用户数，即主文件目录的最大目录数
#define DiskSpace 6*50			//磁盘空间大小为1MB
#define DiskBlockSize 6			//磁盘块的大小1k	
char disk[DiskSpace];				//定义一个磁盘

struct diskblock {								//磁盘块结构体
	int num;									//盘块号
	diskblock *next;
};
diskblock *free_block_head;
struct FCB {				//文件控制块FCB
	char file_name[10];
	//int file_type;			//文件类型 0是普通文件，1是目录文件
	diskblock *begin_block;		//起始盘块号
	int size;				//盘块数
	int pro_code;			//保护码 1为可写 0为可读 2为可读写
	int openflag;		//打开标志 0为关闭，1为打开
};
struct UFD {				//用户文件目录
	FCB fcb;					//目录项
	UFD* next;
};
struct MFD {				//主文件目录
	int user_count;					//用户个数
	char name[10];				//用户名
	char password[10];
	UFD *ufd;			//该用户下的用户文件目录，以文件形式存在
	MFD *next;
};
MFD *mfd_head;				//目录文件的结构（5个目录项组成）

MFD* login() {											//登录
	char name[10];
	char pw[10];
	int count = 0;
	system("cls");									//清屏
	cout << "输入用户名：";
	cin.getline(name, 10);
	cout << "输入密码：";
	cin.getline(pw, 10);
	MFD *p;
	p = mfd_head->next;
	while(p != NULL){
		if (!strcmp(p->name,name)) {
			while (count < 2) {
				if (!strcmp(p->password, pw)) {
					cout << "登录成功！" << endl;
					cout << "*		命令格式		说明				*" << endl;
					cout << "*		dir			显示文件目录		    *" << endl;
					cout << "*		create			创建文件			*" << endl;
					cout << "*		delete			删除文件			*" << endl;
					cout << "*		open			打开文件			*" << endl;
					cout << "*		close			关闭文件			*" << endl;
					cout << "*		read			读文件				*" << endl;
					cout << "*		write			写文件				*" << endl;
					cout << "*		switch			切换用户			*" << endl;
					cout << "*		adduser			添加用户			*" << endl;
					cout << "*		exit			退出系统			*" << endl;
					cout << "************************************************" << endl;
					return p;
				}
				cout << "输入密码不正确，请重新输入：";
				memset(pw,'/0',sizeof(pw));
				cin.getline(pw, 10);
				count++;
			}
			cout << "输入密码有误3次，退出！" << endl;
			return NULL;
			
		}
		p = p->next;
	}
	if (!p) {
		cout << "用户不存在，登录失败！" << endl;
		return NULL;
	}
}				
void create_user() {
	MFD *p,*q;
	char name[10];
	char pw[10];
	cout << "请输入要创建的用户名：";
	cin.getline(name, 10);
	cout << "请输入密码：";
	cin.getline(pw, 10);
	q = mfd_head;
	while (q->next!= NULL) {
		if (!strcmp(q->name, name)) {
			cout << "该用户已存在！" << endl;
			break;
		}
		q = q->next;
	}
	if (!q->next) {
		p = new MFD;
		p->next = NULL;
		strcpy(p->name, name);
		strcpy(p->password, pw);
		p->ufd = new UFD;
		p->ufd->next = NULL;
		q->next = p;
		mfd_head->user_count += 1;
		cout << "用户创建成功！" << endl;
	}
}
void list_dir(MFD *cur_user) {
	cout << left << setw(10) << "文件名" << left << setw(10) <<"磁盘块号" << left << setw(10) << "文件大小" << left << setw(10) <<"文件模式" << left << setw(10) << "文件状态" << endl;
	UFD *p;
	p = cur_user->ufd->next;
	while (p!= NULL) {
		cout << left << setw(10) << p->fcb.file_name << left << setw(10) << p->fcb.begin_block->num << left << setw(10) << p->fcb.size;
		switch (p->fcb.pro_code) {
		case 0:
			cout << left << setw(10) << "可读";
			break;
		case 1:
			cout << left << setw(10) << "可写";
			break;
		case 2:
			cout << left << setw(10) << "可读可写";
			break;
		}
		switch (p->fcb.openflag) {
		case 0:
			cout << left << setw(10) << "关闭";
			break;
		case 1:
			cout << left << setw(10) << "打开";
			break;
		}
		cout << endl;
		p = p->next;
	}

}
void create_file(MFD *cur_user,char *s) {
	UFD *p;
	p = cur_user->ufd->next;
	while (p != NULL) {								//判断是否有文件重名
		if (!strcmp(p->fcb.file_name, s)) {
			cout << "文件已存在，创建失败！" << endl;
			break;
		}
		p = p->next;
	}
	if (!p) {
		int choose;
		cout << "0:read  1:write  2:read&write ? ";
		cin >> choose;
		cin.ignore();				//忽略换行符，避免后面getchar()接收
		UFD *q, *p;
		p = new UFD;						//创建一个新的目录项
		p->next = NULL;
		strcpy(p->fcb.file_name, s);
		diskblock *n;					//从空闲块链取出一个空闲块
		n = free_block_head->next;
		free_block_head->next = n->next;
		n->next = NULL;
		p->fcb.begin_block = n;
		p->fcb.size = 0;
		p->fcb.pro_code = choose;								//假如为可读文件，然后有什么意义
		p->fcb.openflag = 0;
		q = cur_user->ufd;
		while (q->next != NULL) {
			q = q->next;
		}
		q->next = p;
	}
	
}
void delete_file(MFD *cur_user, char *s) {
	diskblock *d;
	UFD *p,*q;
	int i;
	q = cur_user->ufd;
	p = q->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name,s)) {
			d = p->fcb.begin_block;							//指向文件内容第一个磁盘块
			while (d != NULL) {								//判断文件是否还有磁盘块
				d = d->next;
				i = 0;
				while (i < p->fcb.size && i < DiskBlockSize) {								//回收磁盘内容,注意边界问题
					disk[p->fcb.begin_block->num * DiskBlockSize + i] = NULL;
					i++;
				}
				p->fcb.size -= i;
				p->fcb.begin_block->next = free_block_head->next;				//将新释放的盘块链入空闲链头中
				free_block_head->next = p->fcb.begin_block;
				p->fcb.begin_block = d;
			}
			q->next = p->next;
			delete p;
			break;
		}
		p = p->next;
		q = q->next;
	}
	if (p == NULL) {
		cout << "文件不存在！" << endl;
	}
}
void open_file(MFD *cur_user, char *s) {
	UFD *p;
	p = cur_user->ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			p->fcb.openflag = 1;
			cout << "成功打开文件！" << endl;
			break;
		}
		p = p->next;
	}
	if (p == NULL) {
		cout << "文件不存在！" << endl;
	}
}
void close_file(MFD *cur_user, char *s) {
	UFD *p;
	p = cur_user->ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			p->fcb.openflag = 0;
			cout << "成功关闭文件！" << endl;
			break;
		}
		p = p->next;
	}
	if (p == NULL) {
		cout << "文件不存在！" << endl;
	}
}
void read_file(MFD *cur_user, char *s) {
	UFD *p;
	diskblock *d;
	
	int i = 0;
	p = cur_user->ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			break;
		}
		p = p->next;
	}
	if (p) {
		if (p->fcb.openflag) {
			if (p->fcb.pro_code == 0 || p->fcb.pro_code == 2)
			{
				d = p->fcb.begin_block;
				while (i < p->fcb.size) {
					cout << disk[d->num * DiskBlockSize + i % DiskBlockSize];
					i++;
					if (i % DiskBlockSize == 0) {						//需要转到另一块磁盘块
						d = d->next;
					}
				}
				cout << endl;
			}
			else {
				cout << "文件不可读！" << endl;
			}
		}
		else {
			cout << "文件未打开！" << endl;
		}

	}
	else {
		cout << "文件不存在！" << endl;
	}
}
void write_file(MFD *cur_user, char *s) {
	UFD *p;
	cout<<"编辑文件："<<endl;
	diskblock *d,*d_new;
	int block_sum;						//所分配的磁盘块数总和
	p = cur_user->ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			break;
		}
		p = p->next;
	}
	if (p) {
		if (p->fcb.openflag) {
			if (p->fcb.pro_code == 1 || p->fcb.pro_code == 2) {
				char ch;
				d = p->fcb.begin_block;
				int i = 0;

				while (d->next!= NULL) {						//当文件中原本有内容时，将其先显示出来
					while (i < p->fcb.size) {
						cout<< disk[d->num * DiskBlockSize + i % DiskBlockSize];
						i++;
						if (i%DiskBlockSize == 0)
							break;
					}
					d = d->next;
				}
				while (i < p->fcb.size) {
					cout << disk[d->num * DiskBlockSize + i % DiskBlockSize];
					i++;
				}
				block_sum = p->fcb.size / DiskBlockSize + 1;
				while ((ch = getchar()) != '\n') {											//采用链接文件的物理组织方式
					disk[d->num * DiskBlockSize + p->fcb.size % DiskBlockSize] = ch;
					p->fcb.size++;						//加个判断，如果size大于1024，使用额外的磁盘块
					if (p->fcb.size / DiskBlockSize == block_sum) {
						d_new = free_block_head->next;				//从空闲块链中取出新块
						free_block_head->next = d_new->next;
						d_new->next = NULL;
						d->next = d_new;
						d = d_new;
						block_sum++;

					}
				}
				disk[d->num *DiskBlockSize + p->fcb.size%DiskBlockSize] = '\0';
			}
			else {
				cout << "文件不可写！" << endl;
			}
		}
		else {
			cout << "文件未打开！" << endl;
		}
	}
	else {
		cout << "文件不存在！" << endl;
	} 
}

/*初始化磁盘块,为系统存取数据的基本单位*/
void init_disk(diskblock *free_block_head) {
	diskblock *q, *p;
	q = free_block_head;
	for (int i = 0; i < (DiskSpace / DiskBlockSize); i++) {					//文件存储空间的管理 ——采用空闲块链接法
		p = new diskblock;
		p->num = i;
		p->next = NULL;
		q->next = p;
		q = p;
	}
}
void save() {
	fstream MFD_file, UFD_file,Disk;
	MFD_file.open("G:\\MFD.txt",ios::in |ios::out);
	if (!MFD_file) {
		cout << "MFD文件打不开！" << endl;
		//exit(1);
		return;
	}
	UFD_file.open("G:\\UFD.txt", ios::in | ios::out);
	if (!UFD_file) {
		cout << "UFD文件打不开！" << endl;
		//exit(1);
		return;
	}
	Disk.open("G:\\disk.txt", ios::in | ios::out);
	if (!Disk) {
		cout << "Disk文件打不开！" << endl;
		//exit(1);
		return;
	}
	MFD *p;
	UFD *q;
	p = mfd_head->next;
	while (p != NULL) {
		q = p->ufd->next;
		MFD_file << p->name << " " << p->password << endl;
		while (q != NULL) {
			UFD_file << q->fcb.file_name << " " << q->fcb.begin_block->num << " " << q->fcb.size << " " << q->fcb.pro_code << endl;
			q = q->next;
		}
		p = p->next;
	}
	int count = 0;
	
	while (count < DiskSpace) {
		//if (disk[count] != NULL)
			Disk << disk[count];
		//else
			//Disk << " ";
		count++;
		if (count % DiskBlockSize == 0) {
			Disk << endl;
		}
	}
	MFD_file.close();
	UFD_file.close();
	Disk.close();
}
int main() {
	free_block_head = new diskblock;
	free_block_head->next = NULL;
	init_disk(free_block_head);						//初始化磁盘
	
	mfd_head = new MFD;
	mfd_head->user_count = 0;
	mfd_head->next = NULL;
	
	create_user();
	MFD *cur_user;
	cur_user = login();							//获取当前登录的用户
	if(cur_user != NULL) {
		string command[10];
		command[0] = "dir";
		command[1] = "create";
		command[2] = "delete";
		command[3] = "open";
		command[4] = "close";
		command[5] = "read";
		command[6] = "write";
		command[7] = "switch";
		command[8] = "adduser";
		command[9] = "exit";
		bool flag = false;							//判断退出系统
		while (1) {
			cout << "Root:>";
			char ch;
			string c = "";
			while ((ch = getchar()) != '\n') {
				c += ch;
			}
			string str1, str2;
			istringstream is(c);
			is >> str1 >> str2;
			int count;
			for (count = 0; count < 10; count++) {
				if (command[count] == str1) {
					break;
				}
			}
			if (count != 10) {
				switch (count) {
				case 0:
					list_dir(cur_user);
					break;
				case 1:
					create_file(cur_user, (char *)str2.c_str());
					break;
				case 2:
					delete_file(cur_user, (char *)str2.c_str());
					break;
				case 3:
					open_file(cur_user, (char *)str2.c_str());
					break;
				case 4:
					close_file(cur_user, (char *)str2.c_str());
					break;
				case 5:
					read_file(cur_user, (char *)str2.c_str());
					break;
				case 6:
					write_file(cur_user, (char *)str2.c_str());
					break;
				case 7:
					cur_user = login();
					break;
				case 8:
					if (mfd_head->user_count < 6)
						create_user();
					else
						cout << "用户数不能超过5个！" << endl;
					break;
				case 9:
					flag = true;
					save();
					cout << "退出系统！" << endl;
					break;
				}
			}
			else {
				cout << "输入命令有误！" << endl;
			}
			if (flag)
				break;
		}
	}
	system("pause");
}