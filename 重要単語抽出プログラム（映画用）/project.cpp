#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <math.h>
#include <strstream>
#include "chasen.h"

#define NUM_EIGA 135

using namespace std;

void tf(char *itibun, int num_eiga);
void tf_zenbu();
void tfidf();

int main(void)
{

	tf_zenbu();
	tfidf();

	return 0;
}

void tf_zenbu(){ ///�S�Ẳf��̊T�v���̒P���tf(�o���p�x)���v�Z���邽�߂̊֐�

	FILE *fp_in;
	
	int size_itibun;
	int i, j;

	char read[10000];
	char *token;
	char *itibun[300];

	string str;
	string str1, str2, str3;
	stringstream str_num;
	
	str = "�T�v_��2.txt";
	fp_in = fopen(str.c_str(), "r");
	cout << str << "��open���܂���" << endl;

	///�f��̊T�v�̓ǂݍ���
	i=0;
	while(fgets(read, 10000, fp_in)!=NULL){
		token = strtok(read,"\n");
		itibun[i] = strdup(token);
		//printf("%s", itibun[i]);
		i++;
	}
	fclose(fp_in);
	//printf("%s", itibun[0]);
	size_itibun = i;

	///�e�f���tf�̌v�Z
	for(i=0; i<size_itibun; i++){
		tf(itibun[i], i);
	}
}

void tf(char *itibun, int num_eiga){///1�̉f��̊T�v���̒P���tf(�o���p�x)���v�Z����֐�

	ofstream ofs;

	int size_tango_seisiki;
	int size_tango_tmp;
	int Ret;
	int flag;
	int i, j, k, l;
	int s, t;

	double tf_tango_seisiki[600];
	double tf_tango_tmp[1000];
	double temp;

	char *result_chasen;
	char *tango_seisiki[600];
	char *tango_tmp[1000];

	char *token;
	char word[50];

	string str;
	string str1, str2, str3;
	stringstream str_num;

	///�`�ԑf��̓\�t�g��⤂̃I�v�V�����w��
	//char *option[] = {"project.cpp","-j", "-F", "%m\t%M\t%y\t%H\t%U(%P-)\t%F\t\n", NULL};
	char *option[] = {"project.cpp","-j", "-F", "%M\t%m\t%y\t%H\t%U(%P-)\t%F\t\n", NULL};	//%M�͓�������{�`�ɂȂ�
	Ret = chasen_getopt_argv(option, NULL);
	//result_chasen = chasen_sparse_tostr(itibun[0]);
	//cout << result_chasen << endl;

	///�T�v�̖����Ɠ����̓ǂݍ���
	result_chasen = chasen_sparse_tostr(itibun);	///�`�ԑf���
	token = strtok(result_chasen,"\n");		//�^�u�ŋ�؂�ꂽ�����𕪊�����
	//printf("%s\n", token);
	j=0;
	while(token != NULL){
		if(strstr(token, "����")!=NULL || strstr(token,"����")!=NULL && strstr(token,"������")==NULL){
			sscanf(token, "%s", word);
			tango_tmp[j] = strdup(word);						//���s�ŋ�؂�ꂽ�������i�[����
			//cout << tango_tmp[j] << endl;					//���̕������o��
			j++;
		}
		token = strtok( NULL,"\n"); // 2��ڈȍ~ 
	}
	size_tango_tmp = j;
	
	///tf�i�o���p�x�j�̌v�Z
	for(j=0; j<size_tango_tmp; j++){
		l=0;
		for(k=0; k<size_tango_tmp; k++){
			if(strcmp(tango_tmp[k], tango_tmp[j])==0){
				l++;
			}
		}
		tf_tango_tmp[j] = (double)l/size_tango_tmp;
		//printf("%s\t%lf\n", tango_tmp[j], tf_tango_tmp[j]);
	}
	
	///�\�[�g
	for(j=0; j<size_tango_tmp-1; j++){
		for(k=j+1; k<size_tango_tmp; k++){
			if(tf_tango_tmp[j] < tf_tango_tmp[k]){
				temp = tf_tango_tmp[j];
				tf_tango_tmp[j] = tf_tango_tmp[k];
				tf_tango_tmp[k] = temp;

				token = tango_tmp[j];
				tango_tmp[j] = tango_tmp[k];
				tango_tmp[k] = token;
			}
		}
		//printf("%s\t%lf\n", tango_tmp[j], tf_tango_tmp[j]);
	}
	
	///�d���폜
	size_tango_seisiki = 0;
	for(j=0; j<size_tango_tmp; j++){
		flag = 0;
		for(k=0; k<size_tango_seisiki; k++){
			if(strcmp(tango_seisiki[k], tango_tmp[j]) == 0){
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			tango_seisiki[size_tango_seisiki] = strdup(tango_tmp[j]);
			tf_tango_seisiki[size_tango_seisiki] = tf_tango_tmp[j];
			size_tango_seisiki++;
		}
	}


	///�o��
	str1 = "tf\\";
	str3 = ".txt";
	if(num_eiga<=9) str2 = "00";
	else if(num_eiga<=99) str2 = "0";
	else str2 = "";

	str_num.str("");
	str_num << num_eiga;

	str = str1 + str2 + str_num.str() + str3;
	ofs.open(str.c_str());
	cout << str << "��open���܂���" << endl;

	for(i=0; i<size_tango_seisiki; i++){
		ofs << tango_seisiki[i] << "\t" << tf_tango_seisiki[i] << endl;
	}
	ofs.close();
	//printf("%s\t%lf\n", tango_seisiki[0], tf_tango_seisiki[0]);
}

void tfidf(){
	
	FILE *fp_in;
	ofstream ofs[NUM_EIGA];

	int size_tango[NUM_EIGA];
	int i, j, k, l;
	int s, t;

	double tf_tango[NUM_EIGA][600];
	double idf_tango[NUM_EIGA][600];
	double tfidf_tango[NUM_EIGA][600];
	double temp;

	char read[1000];
	char word[50];
	char *token;
	char *tango[NUM_EIGA][600];

	string str;
	string str1, str2, str3;
	stringstream str_num;

	///tf�t�@�C���Q�̓ǂݍ���
	for(j=0; j<NUM_EIGA; j++){
		
		str1 = "tf\\";
		str3 = ".txt";
		if(j<=9) str2 = "00";
		else if(j<=99) str2 = "0";
		else str2 = "";

		str_num.str("");
		str_num << j;

		str = str1 + str2 + str_num.str() + str3;
		fp_in = fopen(str.c_str(), "r");
		cout << str << "��open���܂���" << endl;

		k=0;
		while(fgets(read, 1000, fp_in)!=NULL){
			token = strtok(read,"\t");
			sscanf(token, "%s", word);
			tango[j][k] = strdup(word);
			token = strtok( NULL,"\t");
			sscanf(token, "%lf", &tf_tango[j][k]);
			//cout << tango[j][k] << "\t" << tf_tango[j][k] << endl;
			k++;			
		}
		fclose(fp_in);

		size_tango[j] = k;
	}

	///idf�̌v�Z
	for(i=0; i<NUM_EIGA; i++){
		//cout << i << "�Ԗړ���܂���" << endl;
		for(j=0; j<size_tango[i]; j++){
			s = 0;
			for(k=0; k<NUM_EIGA; k++){
				for(l=0; l<size_tango[k]; l++){
					if(strcmp(tango[k][l], tango[i][j]) == 0){
						s++;
						break;
					}
				}
			}

			idf_tango[i][j] = log((double)NUM_EIGA/(double)s);
			//printf("%s\t%d %lf\n", tango[i][j], s, idf_tango[i][j]);
		}
	}

	
	///tfidf�̌v�Z
	for(i=0; i<NUM_EIGA; i++){
		for(j=0; j<size_tango[i]; j++){
			tfidf_tango[i][j] = tf_tango[i][j] * idf_tango[i][j];
			//printf("%s\t%lf\n",tango[i][j], tfidf_tango[i][j]);
		}
	}
	
	///�\�[�g
	for(i=0; i<NUM_EIGA; i++){
		for(j=0; j<size_tango[i]-1; j++){
			for(k=j+1; k<size_tango[i]; k++){
				if(tfidf_tango[i][j] < tfidf_tango[i][k]){
					temp = tfidf_tango[i][j];
					tfidf_tango[i][j] = tfidf_tango[i][k];
					tfidf_tango[i][k] = temp;

					temp = tf_tango[i][j];
					tf_tango[i][j] = tf_tango[i][k];
					tf_tango[i][k] = temp;

					temp = idf_tango[i][j];
					idf_tango[i][j] = idf_tango[i][k];
					idf_tango[i][k] = temp;

					token = tango[i][j];
					tango[i][j] = tango[i][k];
					tango[i][k] = token;
				}
			}
			printf("%s\t%lf\n", tango[i][j], tfidf_tango[i][j]);
		}
	}
	
	
	///�o�̓t�@�C���I�[�v��
	str1 = "tfidf\\";
	str3 = ".txt";
	for(i=0; i<NUM_EIGA; i++){
		if(i<=9) str2 = "00";
		else if(i<=99) str2 = "0";
		else str2 = "";

		str_num.str("");
		str_num << i;

		str = str1 + str2 + str_num.str() + str3;
		ofs[i].open(str.c_str());
		cout << str << "��open���܂���" << endl;
	}

	///�t�@�C���֏o��
	for(i=0; i<NUM_EIGA; i++){
		for(j=0; j<size_tango[i]; j++){
			//ofs[i] << tango[i][j] << "\t" << tfidf_tango[i][j] << " = " << tf_tango[i][j] << " * " << idf_tango[i][j] << endl;
			ofs[i] << tango[i][j] << "\t" << tfidf_tango[i][j] << endl;
		}
	}

	///�o�̓t�@�C���N���[�Y
	for(i=0; i<NUM_EIGA; i++){
		ofs[i].close();
	}
}
