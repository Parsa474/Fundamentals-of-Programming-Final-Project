#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "Mr.Jack.h"
#define debug 0

int main(){
	srand(time(0));
	struct block blocks[9][13], blk;
	int i, j, k, choice, character, n, counter=0, Jack, initial=0, flag=1;
	struct coordinates coo;
	struct node *list=NULL, *new_node, *deck=NULL, *sub_deck1=NULL, *sub_deck2=NULL, *innocent_list=NULL;
	FILE *f;
	char name[30], file_name[30];
	blk.identifier=street;
	blk.blocked=0; blk.direction=0; blk.num=0; blk.on=0; blk.sus=0; blk.visible=0, blk.well=0;
	for(i=0; i<9; i++){
		for(j=0; j<13; j++){
			blocks[i][j]=blk;
		}
	}
	printf("1.New Game\n2.Load\n");
	scanf(" %d", &choice);
	checker(1, 2, &choice);
	if(choice==1){
		if(read_locations("buildings.txt", building, blocks)) return 1;
		if(read_locations("lights.txt", light, blocks)) return 1;
		if(read_locations("wells.txt", street, blocks)) return 1;
		if(read_locations("extras.txt", extra, blocks)) return 1;
		if(read_locations("escapes.txt", esc, blocks)) return 1;
		f=fopen("characters.txt", "r");
		if(f==NULL) {printf("Could not open the characters' file\n"); return 1;}
		for(i=0; i<8; i++){
			fscanf(f, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &coo.x, &coo.y, &blk.sus, &blk.visible, &blk.well, &blk.blocked, &blk.on, &blk.direction, &blk.num);
			blk.identifier=i;
			blocks[coo.y][coo.x]=blk;
		}
		close(f);
	} else{
		while(1){
			system("cls");
			printf("Enter the name of the saved file to load: ");
			scanf(" %s", name);
			sprintf(file_name, "%s.bin", name);
			f = fopen(file_name, "rb");
			if(f==NULL) printf("could not open this file");
			else{
				fread(&initial, sizeof(int), 1, f);
				fread(&Jack, sizeof(int), 1, f);				
				//fread(innocent_list, sizeof(struct node*), 1)
				for(j=0; j<9; j++){
					for(k=0; k<13; k++){
						fread(&blocks[j][k], sizeof(struct block), 1, f);
					}
				}
				close(f);
				break;
			}
		}
	}
	update_screen(blocks);
	//make the linked list of characters
	for(i=0; i<8; i++){
		new_node = (struct node *)malloc(sizeof(struct node));
		if(new_node == NULL){
			printf("memory allocation failed!\n");
            return 1;                               
        }
        new_node->character.character=i;
        if(list == NULL)
            add_front(new_node, &(list));
        else
            add_end(new_node, list);
	}
	shuffle(list);
	if(debug){
		printf("The list is: ");
		print_list(list);
	}
	if(choice==1){
		printf("Mr.Jack, draw your character card(press any key), Sir detective you must look away here\n");
		getch();
		linked_list_cpy(&innocent_list, list);
		shuffle(innocent_list);
		Jack = get(innocent_list, 0);
		remove_node(&innocent_list, 0);
		switch(Jack){
			case SH:
				printf("Mr.Jack is Sherlock Holmes(SH)\n");
			break;
			case JW:
				printf("Mr.Jack is John H. Watson(JW)\n");
				break;
			case JS:
				printf("Mr.Jack is John Smith(JS)\n");
			break;
			case IL:
				printf("Mr.Jack is Inspector Lestrade(IL)\n");
			break;
			case MS:
				printf("Mr.Jack is Miss Stealthy(MS)\n");
			break;
			case SG:
				printf("Mr.Jack is Sergeant Goodley(SG)\n");
			break;
			case WG:
				printf("Mr.Jack is William Gull(WG)\n");
			break;
			case JB:
				printf("Mr.Jack is Jeremy Bert(JB)\n");
			break;
			default:
				printf("Failed to get the identity of Mr.Jack\n");
			break;
		}
		printf("Press any key to continue\n");
		getch();
	}
	int dbug=0;		//test the gameplay of a character
	if(dbug){
		n=3;
		play(SG, blocks, &n, &counter, Jack, 0, innocent_list);
		update_visiblity_by_JW(blocks);
		getch();
		update_screen(blocks);
		if(innocent_list!=NULL) free(innocent_list);
		free(list);
		return 0;
	}
	if(1){
		print_list(innocent_list);
	}
	system("cls");
	for(i=initial; i<8 && flag; i++){
		system("cls");
		printf("Round %d starts!\n", i+1);
		print_map(blocks);
		if(i%2==0){
			linked_list_cpy(&deck, list);
			printf("Sir detective, draw the top 4 character cards from the deck after shuffling it(press any key)\n");
			getch();
			shuffle(deck);
			if(debug){
				printf("The deck is:");
				print_list(deck);	
			}
			printf("The drawn cards in order are: ");
			sub_deck2=deck->next->next->next->next;
			sub_deck1=deck;
			deck->next->next->next->next=NULL;
			for(j=0; j<4; j++){
				print_character_name(get(sub_deck1, j));
				printf("   ");
			}
			printf("\n");
			printf("Detective's turn: which character do you want to play?(enter the number of the drawn card from 1 to 4)\n");
			scanf(" %d", &choice);
			checker(1, 4, &choice);
			character = get(sub_deck1, choice-1);
			remove_node(&sub_deck1, choice-1);
			n=3;
			if(character==MS) n++;
			coo=find_character(character, blocks);
			play(character, blocks, &n, &counter, Jack, 1, innocent_list);
			for(k=0; k<2; k++){
				printf("Jack's turn: which character do you want to play?(enter the number amongst the remaining cards from 1 to %d)\n", 3-k);
				for(j=0; j<3-k; j++){
				print_character_name(get(sub_deck1, j));
				printf("   ");
				}
				scanf(" %d", &choice);
				checker(1, 3-k, &choice);
				character = get(sub_deck1, choice-1);
				remove_node(&sub_deck1, choice-1);
				n=3;
				if(character==MS) n++;
				coo=find_character(character, blocks);
				play(character, blocks, &n, &counter, Jack, 0, innocent_list);
			}
			printf("Detective's turn: play with the last card: ");
			character = get(sub_deck1, 0);
			remove_node(&sub_deck1, 0);
			print_character_name(character);
			printf("\n");
			n=3;
			if(character==MS) n++;
			coo=find_character(character, blocks);
			play(character, blocks, &n, &counter, Jack, 1, innocent_list);
		}else{
			deck=list;
			printf("Jack, reveal the 4 remaining cards(press any key)\n");
			getch();
			printf("The cards in order are: ");
			for(j=0; j<4; j++){
				print_character_name(get(sub_deck2, j));
				printf("   ");
			}
			printf("\n");
			printf("Jack's turn: which character do you want to play?(enter the number of the drawn card from 1 to 4)\n");
			scanf(" %d", &choice);
			checker(1, 4, &choice);
			character = get(sub_deck2, choice-1);
			remove_node(&sub_deck2, choice-1);
			n=3;
			if(character==MS) n++;
			coo=find_character(character, blocks);
			play(character, blocks, &n, &counter, Jack, 0, innocent_list);
			for(k=0; k<2; k++){
				printf("Detective's turn: which character do you want to play?(enter the number amongst the remaining cards from 1 to %d)\n", 3-k);
				for(j=0; j<3-k; j++){
				print_character_name(get(sub_deck2, j));
				printf("   ");
				}
				scanf(" %d", &choice);
				checker(1, 3-k, &choice);
				character = get(sub_deck2, choice-1);
				remove_node(&sub_deck2, choice-1);
				n=3;
				if(character==MS) n++;
				coo=find_character(character, blocks);
				play(character, blocks, &n, &counter, Jack, 1, innocent_list);
			}
			printf("Jack's turn: play with the last card: ");
			character = get(sub_deck2, 0);
			remove_node(&sub_deck2, 0);
			n=3;
			if(character==MS) n++;
			print_character_name(character);
			printf("\n");
			coo=find_character(character, blocks);
			play(character, blocks, &n, &counter, Jack, 0, innocent_list);
		}
		update_visiblity_by_JW(blocks);
		update_visibility(blocks);
		coo=find_character(Jack, blocks);
		if(blocks[coo.y][coo.x].visible){
			for(j=0; j<8; j++){
				coo=find_character(j, blocks);
				if(blocks[coo.y][coo.x].visible==0){
					blocks[coo.y][coo.x].sus=0;
				}
			}
		} else{
			for(j=0; j<8; j++){
				coo=find_character(j, blocks);
				if(blocks[coo.y][coo.x].visible){
					blocks[coo.y][coo.x].sus=0;
				}
			}
		}
		if(i<4){
			coo=find_numbered_light(i+1, blocks);
			blocks[coo.y][coo.x].on=0;
		}
		printf("Do you want to 1. continue playing or 2.save and come back later? ");
		scanf(" %d", &choice);
		checker(1, 2, &choice);
		if(choice==2){
			printf("Enter the name of the file you want to save: ");
			scanf(" %s", name);
			sprintf(file_name, "%s.bin", name);
			f = fopen(file_name, "wb");
			if(f==NULL){
				printf("cound not create the save\n");
				return 1;
			}
			i++;
			fwrite(&i, sizeof(int), 1, f);
			fwrite(&Jack, sizeof(int), 1, f);
			for(j=0; j<9; j++){
				for(k=0; k<13; k++){
					fwrite(&blocks[j][k], sizeof(struct block), 1, f);
				}
			}
			close(f);
			flag==0;
		}
	}
	if(i==8) printf("Time is up! Mr.Jack wins\n");
	if(innocent_list!=NULL) free(innocent_list);
	free(list);
	getch();
	return 0;
}
