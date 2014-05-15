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

void tf_zenbu(){ ///全ての映画の概要内の単語のtf(出現頻度)を計算するための関数

	FILE *fp_in;
	
	int size_itibun;
	int i, j;

	char read[10000];
	char *token;
	char *itibun[300];

	string str;
	string str1, str2, str3;
	stringstream str_num;
	
	str = "概要_改2.txt";
	fp_in = fopen(str.c_str(), "r");
	cout << str << "をopenしました" << endl;

	///映画の概要の読み込み
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

	///各映画のtfの計算
	for(i=0; i<size_itibun; i++){
		tf(itibun[i], i);
	}
}

void tf(char *itibun, int num_eiga){///1つの映画の概要内の単語のtf(出現頻度)を計算する関数

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

	///形態素解析ソフト茶筅のオプション指定
	//char *option[] = {"project.cpp","-j", "-F", "%m\t%M\t%y\t%H\t%U(%P-)\t%F\t\n", NULL};
	char *option[] = {"project.cpp","-j", "-F", "%M\t%m\t%y\t%H\t%U(%P-)\t%F\t\n", NULL};	//%Mは動詞が基本形になる
	Ret = chasen_getopt_argv(option, NULL);
	//result_chasen = chasen_sparse_tostr(itibun[0]);
	//cout << result_chasen << endl;

	///概要の名詞と動詞の読み込み
	result_chasen = chasen_sparse_tostr(itibun);	///形態素解析
	token = strtok(result_chasen,"\n");		//タブで区切られた文字を分割する
	//printf("%s\n", token);
	j=0;
	while(token != NULL){
		if(strstr(token, "名詞")!=NULL || strstr(token,"動詞")!=NULL && strstr(token,"助動詞")==NULL){
			sscanf(token, "%s", word);
			tango_tmp[j] = strdup(word);						//改行で区切られた文字を格納する
			//cout << tango_tmp[j] << endl;					//その文字を出力
			j++;
		}
		token = strtok( NULL,"\n"); // 2回目以降 
	}
	size_tango_tmp = j;
	
	///tf（出現頻度）の計算
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
	
	///ソート
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
	
	///重複削除
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


	///出力
	str1 = "tf\\";
	str3 = ".txt";
	if(num_eiga<=9) str2 = "00";
	else if(num_eiga<=99) str2 = "0";
	else str2 = "";

	str_num.str("");
	str_num << num_eiga;

	str = str1 + str2 + str_num.str() + str3;
	ofs.open(str.c_str());
	cout << str << "をopenしました" << endl;

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

	///tfファイル群の読み込み
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
		cout << str << "をopenしました" << endl;

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

	///idfの計算
	for(i=0; i<NUM_EIGA; i++){
		//cout << i << "番目入りました" << endl;
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

	
	///tfidfの計算
	for(i=0; i<NUM_EIGA; i++){
		for(j=0; j<size_tango[i]; j++){
			tfidf_tango[i][j] = tf_tango[i][j] * idf_tango[i][j];
			//printf("%s\t%lf\n",tango[i][j], tfidf_tango[i][j]);
		}
	}
	
	///ソート
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
	
	
	///出力ファイルオープン
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
		cout << str << "をopenしました" << endl;
	}

	///ファイルへ出力
	for(i=0; i<NUM_EIGA; i++){
		for(j=0; j<size_tango[i]; j++){
			//ofs[i] << tango[i][j] << "\t" << tfidf_tango[i][j] << " = " << tf_tango[i][j] << " * " << idf_tango[i][j] << endl;
			ofs[i] << tango[i][j] << "\t" << tfidf_tango[i][j] << endl;
		}
	}

	///出力ファイルクローズ
	for(i=0; i<NUM_EIGA; i++){
		ofs[i].close();
	}
}
