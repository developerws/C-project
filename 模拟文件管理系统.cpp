#include<iostream>
#include<sstream>
#include<iomanip>
#include<fstream>
#include<string>
using namespace std;
#define MAX_user 5					//����û����������ļ�Ŀ¼�����Ŀ¼��
#define DiskSpace 6*50			//���̿ռ��СΪ1MB
#define DiskBlockSize 6			//���̿�Ĵ�С1k	
char disk[DiskSpace];				//����һ������

struct diskblock {								//���̿�ṹ��
	int num;									//�̿��
	diskblock *next;
};
diskblock *free_block_head;
struct FCB {				//�ļ����ƿ�FCB
	char file_name[10];
	//int file_type;			//�ļ����� 0����ͨ�ļ���1��Ŀ¼�ļ�
	diskblock *begin_block;		//��ʼ�̿��
	int size;				//�̿���
	int pro_code;			//������ 1Ϊ��д 0Ϊ�ɶ� 2Ϊ�ɶ�д
	int openflag;		//�򿪱�־ 0Ϊ�رգ�1Ϊ��
};
struct UFD {				//�û��ļ�Ŀ¼
	FCB fcb;					//Ŀ¼��
	UFD* next;
};
struct MFD {				//���ļ�Ŀ¼
	int user_count;					//�û�����
	char name[10];				//�û���
	char password[10];
	UFD *ufd;			//���û��µ��û��ļ�Ŀ¼�����ļ���ʽ����
	MFD *next;
};
MFD *mfd_head;				//Ŀ¼�ļ��Ľṹ��5��Ŀ¼����ɣ�

MFD* login() {											//��¼
	char name[10];
	char pw[10];
	int count = 0;
	system("cls");									//����
	cout << "�����û�����";
	cin.getline(name, 10);
	cout << "�������룺";
	cin.getline(pw, 10);
	MFD *p;
	p = mfd_head->next;
	while(p != NULL){
		if (!strcmp(p->name,name)) {
			while (count < 2) {
				if (!strcmp(p->password, pw)) {
					cout << "��¼�ɹ���" << endl;
					cout << "*		�����ʽ		˵��				*" << endl;
					cout << "*		dir			��ʾ�ļ�Ŀ¼		    *" << endl;
					cout << "*		create			�����ļ�			*" << endl;
					cout << "*		delete			ɾ���ļ�			*" << endl;
					cout << "*		open			���ļ�			*" << endl;
					cout << "*		close			�ر��ļ�			*" << endl;
					cout << "*		read			���ļ�				*" << endl;
					cout << "*		write			д�ļ�				*" << endl;
					cout << "*		switch			�л��û�			*" << endl;
					cout << "*		adduser			����û�			*" << endl;
					cout << "*		exit			�˳�ϵͳ			*" << endl;
					cout << "************************************************" << endl;
					return p;
				}
				cout << "�������벻��ȷ�����������룺";
				memset(pw,'/0',sizeof(pw));
				cin.getline(pw, 10);
				count++;
			}
			cout << "������������3�Σ��˳���" << endl;
			return NULL;
			
		}
		p = p->next;
	}
	if (!p) {
		cout << "�û������ڣ���¼ʧ�ܣ�" << endl;
		return NULL;
	}
}				
void create_user() {
	MFD *p,*q;
	char name[10];
	char pw[10];
	cout << "������Ҫ�������û�����";
	cin.getline(name, 10);
	cout << "���������룺";
	cin.getline(pw, 10);
	q = mfd_head;
	while (q->next!= NULL) {
		if (!strcmp(q->name, name)) {
			cout << "���û��Ѵ��ڣ�" << endl;
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
		cout << "�û������ɹ���" << endl;
	}
}
void list_dir(MFD *cur_user) {
	cout << left << setw(10) << "�ļ���" << left << setw(10) <<"���̿��" << left << setw(10) << "�ļ���С" << left << setw(10) <<"�ļ�ģʽ" << left << setw(10) << "�ļ�״̬" << endl;
	UFD *p;
	p = cur_user->ufd->next;
	while (p!= NULL) {
		cout << left << setw(10) << p->fcb.file_name << left << setw(10) << p->fcb.begin_block->num << left << setw(10) << p->fcb.size;
		switch (p->fcb.pro_code) {
		case 0:
			cout << left << setw(10) << "�ɶ�";
			break;
		case 1:
			cout << left << setw(10) << "��д";
			break;
		case 2:
			cout << left << setw(10) << "�ɶ���д";
			break;
		}
		switch (p->fcb.openflag) {
		case 0:
			cout << left << setw(10) << "�ر�";
			break;
		case 1:
			cout << left << setw(10) << "��";
			break;
		}
		cout << endl;
		p = p->next;
	}

}
void create_file(MFD *cur_user,char *s) {
	UFD *p;
	p = cur_user->ufd->next;
	while (p != NULL) {								//�ж��Ƿ����ļ�����
		if (!strcmp(p->fcb.file_name, s)) {
			cout << "�ļ��Ѵ��ڣ�����ʧ�ܣ�" << endl;
			break;
		}
		p = p->next;
	}
	if (!p) {
		int choose;
		cout << "0:read  1:write  2:read&write ? ";
		cin >> choose;
		cin.ignore();				//���Ի��з����������getchar()����
		UFD *q, *p;
		p = new UFD;						//����һ���µ�Ŀ¼��
		p->next = NULL;
		strcpy(p->fcb.file_name, s);
		diskblock *n;					//�ӿ��п���ȡ��һ�����п�
		n = free_block_head->next;
		free_block_head->next = n->next;
		n->next = NULL;
		p->fcb.begin_block = n;
		p->fcb.size = 0;
		p->fcb.pro_code = choose;								//����Ϊ�ɶ��ļ���Ȼ����ʲô����
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
			d = p->fcb.begin_block;							//ָ���ļ����ݵ�һ�����̿�
			while (d != NULL) {								//�ж��ļ��Ƿ��д��̿�
				d = d->next;
				i = 0;
				while (i < p->fcb.size && i < DiskBlockSize) {								//���մ�������,ע��߽�����
					disk[p->fcb.begin_block->num * DiskBlockSize + i] = NULL;
					i++;
				}
				p->fcb.size -= i;
				p->fcb.begin_block->next = free_block_head->next;				//�����ͷŵ��̿����������ͷ��
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
		cout << "�ļ������ڣ�" << endl;
	}
}
void open_file(MFD *cur_user, char *s) {
	UFD *p;
	p = cur_user->ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			p->fcb.openflag = 1;
			cout << "�ɹ����ļ���" << endl;
			break;
		}
		p = p->next;
	}
	if (p == NULL) {
		cout << "�ļ������ڣ�" << endl;
	}
}
void close_file(MFD *cur_user, char *s) {
	UFD *p;
	p = cur_user->ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			p->fcb.openflag = 0;
			cout << "�ɹ��ر��ļ���" << endl;
			break;
		}
		p = p->next;
	}
	if (p == NULL) {
		cout << "�ļ������ڣ�" << endl;
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
					if (i % DiskBlockSize == 0) {						//��Ҫת����һ����̿�
						d = d->next;
					}
				}
				cout << endl;
			}
			else {
				cout << "�ļ����ɶ���" << endl;
			}
		}
		else {
			cout << "�ļ�δ�򿪣�" << endl;
		}

	}
	else {
		cout << "�ļ������ڣ�" << endl;
	}
}
void write_file(MFD *cur_user, char *s) {
	UFD *p;
	cout<<"�༭�ļ���"<<endl;
	diskblock *d,*d_new;
	int block_sum;						//������Ĵ��̿����ܺ�
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

				while (d->next!= NULL) {						//���ļ���ԭ��������ʱ����������ʾ����
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
				while ((ch = getchar()) != '\n') {											//���������ļ���������֯��ʽ
					disk[d->num * DiskBlockSize + p->fcb.size % DiskBlockSize] = ch;
					p->fcb.size++;						//�Ӹ��жϣ����size����1024��ʹ�ö���Ĵ��̿�
					if (p->fcb.size / DiskBlockSize == block_sum) {
						d_new = free_block_head->next;				//�ӿ��п�����ȡ���¿�
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
				cout << "�ļ�����д��" << endl;
			}
		}
		else {
			cout << "�ļ�δ�򿪣�" << endl;
		}
	}
	else {
		cout << "�ļ������ڣ�" << endl;
	} 
}

/*��ʼ�����̿�,Ϊϵͳ��ȡ���ݵĻ�����λ*/
void init_disk(diskblock *free_block_head) {
	diskblock *q, *p;
	q = free_block_head;
	for (int i = 0; i < (DiskSpace / DiskBlockSize); i++) {					//�ļ��洢�ռ�Ĺ��� �������ÿ��п����ӷ�
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
		cout << "MFD�ļ��򲻿���" << endl;
		//exit(1);
		return;
	}
	UFD_file.open("G:\\UFD.txt", ios::in | ios::out);
	if (!UFD_file) {
		cout << "UFD�ļ��򲻿���" << endl;
		//exit(1);
		return;
	}
	Disk.open("G:\\disk.txt", ios::in | ios::out);
	if (!Disk) {
		cout << "Disk�ļ��򲻿���" << endl;
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
	init_disk(free_block_head);						//��ʼ������
	
	mfd_head = new MFD;
	mfd_head->user_count = 0;
	mfd_head->next = NULL;
	
	create_user();
	MFD *cur_user;
	cur_user = login();							//��ȡ��ǰ��¼���û�
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
		bool flag = false;							//�ж��˳�ϵͳ
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
						cout << "�û������ܳ���5����" << endl;
					break;
				case 9:
					flag = true;
					save();
					cout << "�˳�ϵͳ��" << endl;
					break;
				}
			}
			else {
				cout << "������������" << endl;
			}
			if (flag)
				break;
		}
	}
	system("pause");
}