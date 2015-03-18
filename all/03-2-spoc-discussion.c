#include <stdio.h>
#include <stdlib.h>
unsigned int  mem[128][32];
int char2int(char a){
	return (a>58)?(a-87):(a-48);
}
int value2(char* a,int start){
	return char2int(a[start])*16+char2int(a[start+1]);
}
int value4(char* a,int start){
	int i=0,sum=0;
	for(;i<4;i++){
		sum*=16;
		sum+=char2int(a[start+i]);
	}
	return sum;
}
void catch(int value){
	//printf("%d %d\n",value>>10,mem[17][value>>10]);
	if(mem[17][value>>10]/128==0){
		printf("	--> pde index:0x%x  pde contents:(valid 0, pfn 0x%x)\n",value>>10,mem[17][value>>10]%128);
      		printf("		--> Fault (page directory entry not valid)\n");
	}else{
		printf("	--> pde index:0x%x  pde contents:(valid 1, pfn 0x%x)\n",value>>10,mem[17][value>>10]%128);
		if(mem[mem[17][value>>10]%128][(value>>5)%32]/128==0){
			printf("		--> pte index:0x%x  pte contents:(valid 0, pfn 0x%x)\n",(value>>5)%32,mem[mem[17][value>>10]%128][(value>>5)%32]%128);
			printf("			--> Fault (page table entry not valid)\n");
		}else{
			printf("		--> pte index:0x%x  pte contents:(valid 1, pfn 0x%x)\n",(value>>5)%32,mem[mem[17][value>>10]%128][(value>>5)%32]%128);
			int address=value%32+(mem[mem[17][value>>10]%128][(value>>5)%32]%128)*32;
			//printf("%d\n",address);
			printf("			 --> Translates to Physical Address 0x%x --> Value: 0x%x\n",address,mem[address/32][address%32]);
		}
	}
}
int main(){
	FILE *fp=fopen("data.txt","rt");
	int index=0;
	char a[1024];
	int offset = 9;
	unsigned int page=0x00;
	while(!feof(fp)){
		fgets(a,1000,fp);
		int i=0;
		for(;i<32;i++){
			mem[index][i]=value2(a,offset+3*i);		
		}
		index++;
	};
	FILE *fin=fopen("add.txt","rt");
	while(!feof(fin)){
		fgets(a,100,fin);
		printf("Virtual Address 0x%x:\n",value4(a,16));
		catch(value4(a,16));
	}
	return 0;
}
