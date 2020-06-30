#include <Windows.h>
#include <string>
#include <list>
#include <iostream>
#include <functional>

using namespace std;
#define BUFFER_SIZE 128

bool getDevcieInfo(char* cmd, list<string>& resultList) {
	char buffer[BUFFER_SIZE];
	bool ret = false;
	FILE* pipe = _popen(cmd, "r"); //打开管道，并执行命令 
	if (!pipe)
		return ret;

	char name[3][20] = { "UUID","ProcessorId","SerialNumber" };
	int len0 = strlen(name[0]), len1 = strlen(name[1]), len2 = strlen(name[2]);
	bool isOk = false;
	while (!feof(pipe))
	{
		if (fgets(buffer, BUFFER_SIZE, pipe))
		{
			if (strncmp(name[0], buffer, len0) == 0
				|| strncmp(name[1], buffer, len1) == 0
				|| strncmp(name[2], buffer, len2) == 0) // 能够正确获取信息
			{
				isOk = true;
				continue;
			}
			if (isOk == false
				|| strcmp("\r\n", buffer) == 0) //去掉windows无用的空行
			{
				continue;
			}
			ret = true;
			resultList.push_back(string(buffer));
		}
	}
	_pclose(pipe); // 关闭管道 
	return ret;
}


string getDeviceFingerPrint() {
	list<string> strList;
	list<string>::iterator it;
	hash<string> str_hash;
	size_t num;
	char tmp[11] = { 0 };

	// 主板UUID存在，就使用主板UUID生成机器指纹
	if (getDevcieInfo((char*)("wmic csproduct get UUID"), strList)
		&& (*strList.begin()).compare("FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF\r\n") != 0)
	{
		//cout << *strList.begin();
		//cout << str_hash(*strList.begin()) << endl;
		num = str_hash(*strList.begin());
		sprintf_s(tmp, "%u", num);
		//cout << string(tmp) << endl;
		return string(tmp);
	}

	// 主板UUID不存在，使用CPUID、BIOS序列号、硬盘序列号生成机器指纹
	string otherStr("");
	strList.clear();
	if (getDevcieInfo((char*)("wmic cpu get processorid"), strList)) {
		otherStr.append(*strList.begin());
	}
	strList.clear();
	if (getDevcieInfo((char*)("wmic bios get serialnumber"), strList)) {
		otherStr.append(*strList.begin());
	}
	strList.clear();
	if (getDevcieInfo((char*)("wmic diskdrive get serialnumber"), strList)) {
		string allDiskNum("");
		// 硬盘可能有多块
		for (it = strList.begin(); it != strList.end(); it++)
		{
			allDiskNum.append(*it);
		}
		//cout << allDiskNum ;
		//cout << str_hash(allDiskNum) << endl;
		otherStr.append(*strList.begin());
	}
	cout << str_hash(otherStr) << endl;
	//memset(tmp,0,11);
	num = str_hash(otherStr);
	sprintf_s(tmp, "%u", num);
	//cout << string(tmp) << endl;
	return string(tmp);
}

void test()
{
	string deviceFingerPrint = getDeviceFingerPrint();
	cout << deviceFingerPrint << endl;
}

int main(int args, char* argv[])
{
	test();
	system("pause");
	return 0;
}
