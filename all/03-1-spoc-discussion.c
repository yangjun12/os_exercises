#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 4096
struct page{
	int size;
	struct page* next;
	struct page* last;
	//int use;    //0 not use 1 in use
	int start;
	int end;
};
struct page* used_=NULL;
struct page* free_=NULL;
int height=0;
void delete_free(struct page *oldpage);
void delete_page(struct page *oldpage);
void myfree(struct page *oldpage);
void mymalloc(int size);
void insert_free(struct page *newpage);
void insert_used(struct page *newpage);
void showall();
void delete_page(struct page *oldpage){
	if(oldpage->last!=NULL && oldpage->next!=NULL){
		oldpage->last->next=oldpage->next;
		oldpage->next->last=oldpage->last;
	}else if(oldpage->next!=NULL){
		used_=oldpage->next;
		used_->last=NULL;
	}else if(oldpage->last!=NULL){
		oldpage->last->next=NULL;
	}
	else used_=NULL;
}
void delete_free(struct page *oldpage){
	if(oldpage->last!=NULL && oldpage->next!=NULL){
		oldpage->last->next=oldpage->next;
		oldpage->next->last=oldpage->last;
	}else if(oldpage->next!=NULL){
		free_=oldpage->next;
		free_->last=NULL;
	}else if(oldpage->last!=NULL){
		oldpage->last->next=NULL;
	}else free_=NULL;
}
void myfree(struct page *oldpage){
	delete_page(oldpage);
	struct page* current=free_;
	int flag=0;
	struct page* answer=NULL;
	while(current!=NULL&&flag==0){
		if(current->start==oldpage->end){
			current->start=oldpage->start;
			current->size=current->size+oldpage->size;
			flag=1;
			answer=current;
			break;
		}
		if(current->end==oldpage->start){
			current->end=oldpage->end;
			current->size=current->size+oldpage->size;
			flag=1;
			answer=current;
			break;
		}
		current=current->next;
	}
	if(flag==1){
	}
	else {
		insert_free(oldpage);
	}
}

void mymalloc(int size){
	if(used_==NULL){
		used_=(struct page*)malloc(sizeof(struct page));
		used_->last=NULL;
		used_->next=NULL;	
		used_->size=size;
		used_->start=0;
		used_->end=size;

		height=size;

		free_=(struct page*)malloc(sizeof(struct page));
		free_->last=NULL;
		free_->next=NULL;	
		free_->size=SIZE-size;
		free_->start=size;
		free_->end=SIZE;
		return;
	}
	if(free_->size<size){
		printf("memory not enough error!");
		return;
	}
	struct page* currentfree=free_;
	delete_free(free_);
	if(currentfree->size>size){
		struct page* addpage=(struct page*)malloc(sizeof(struct page));
		addpage->size=currentfree->size-size;
		addpage->start=currentfree->start+size;
		addpage->end=currentfree->end;
		insert_free(addpage);
	}
	struct page* newpage=(struct page*)malloc(sizeof(struct page));
	newpage->size=size;
	newpage->start=currentfree->start;
	newpage->end=currentfree->start+size;
	insert_used(newpage);

}
void insert_free(struct page *newpage){
	if(free_==NULL){
		free_=newpage;
		free_->last=NULL;
		free_->next=NULL;
	}
	else{
		int flag=0;
		struct page* current=free_;
		struct page* current1=free_;
		while(current!=NULL){
			if(current->size<=newpage->size){
				flag=1;
				if(current==free_){
					newpage->last=NULL;
					newpage->next=current;
					current->last=newpage;
					free_=newpage;
				}
				else{
					current->last->next=newpage;
					newpage->last=current->last;
					current->last=newpage;
					newpage->next=current;
				}
				break;
			}
			current1=current;
			current=current->next;
		}
		if(flag==0){
			current1->next=newpage;
			newpage->last=current1;
			newpage->next=NULL;
		}
	}
}
void insert_used(struct page *newpage){
	if(used_==NULL){
		used_=newpage;
		used_->last=NULL;
		used_->next=NULL;
	}
	else{
		int flag=0;
		struct page* current=used_;
		struct page* current1=used_;
		while(current!=NULL){
			if(current->size<=newpage->size){
				flag=1;
				if(current==used_){
					newpage->last=NULL;
					newpage->next=current;
					current->last=newpage;
					used_=newpage;
				}
				else{
					current->last->next=newpage;
					newpage->last=current->last;
					current->last=newpage;
					newpage->next=current;
				}
				break;
			}
			current1=current;
			current=current->next;
		}
		if(flag==0){
			current1->next=newpage;
			newpage->last=current1;
		}
	}
}
void showall(){
	struct page* c1=used_;
	struct page* c2=free_;
	printf("used_*****\n");
	while(c1!=NULL){
		printf("#%d %d#$%d$",c1->start,c1->end,c1->size);
		if(c1->last==NULL){
			printf(" 0");
		}else{
			printf(" 1");
		}
		if(c1->next==NULL){
			printf(" 0\n");
		}else{
			printf(" 1\n");
		}
		c1=c1->next;
	}
	printf("free_*****\n");
	while(c2!=NULL){
		printf("#%d %d#$%d$",c2->start,c2->end,c2->size);
		if(c2->last==NULL){
			printf(" 0");
		}else{
			printf(" 1");
		}
		if(c2->next==NULL){
			printf(" 0\n");
		}else{
			printf(" 1\n");
		}
		c2=c2->next;	
	}
	printf("show over\n");
}
int main(){
	int flag=0,size=0;
	while(1){
		printf("please cin:");
		scanf("%d %d",&flag,&size);
		if(flag!=0&&flag!=1&&flag!=2){
			printf("illegal input!\n");	
			continue;			
		}
		if(flag==0){
			return 0;
		}
		else if(flag==1){
			mymalloc(size);
		}
		else if(flag==2){
			struct page* current=used_;
			while(current!=NULL){
				if(current->size==size){
					myfree(current);
					break;
				}
				current=current->next;
			}
		}
		showall();
	}
}





