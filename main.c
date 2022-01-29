#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "Mr.Jack.h"
#define debug 0

int main(){
	srand(time(0));
	struct block blocks[9][13], blk, logs[32][9][13];
	int i, j, k, choice, character, n, counter=0, Jack, initial, il[7], innocent_list_index, logs_index;
	struct coordinates coo;
	struct node *list=NULL, *new_node, *new_node2, *new_node3, *deck=NULL, *sub_deck1=NULL, *sub_deck2=NULL, *innocent_list=NULL;
	FILE *f;
	char name[30], file_name[30];
	
	//fill in the map with street blocks
	blk.identifier=street;
	blk.blocked=0; blk.direction=0; blk.num=0; blk.on=0; blk.sus=0; blk.visible=0, blk.well=0;
	for(i=0; i<9; i++){
		for(j=0; j<13; j++){
			blocks[i][j]=blk;
		}
	}
	
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
	
	printf("1.New Game\n2.Load\n");
	scanf(" %d", &choice);
	checker(1, 2, &choice);
	switch(choice){
		case 1:
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
			printf("Mr.Jack, draw your character card(press any key), Sir detective you must look away here\n");
			getch();
			linked_list_cpy(&innocent_list, list);
			shuffle(innocent_list);
			Jack = get(innocent_list, 0);
			remove_node(&innocent_list, 0);
			innocent_list_index=7;
			printf("Mr.Jack is : ");
			print_character_name(Jack);
			printf("\nPress any key to continue\n");
			getch();
			initial=0;
			logs_index=0;
		break;
		case 2:
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
					fread(&innocent_list_index, sizeof(int), 1, f);
					for(j=0; j<innocent_list_index; j++){
						fread(&il[j], sizeof(int), 1, f);
					}
					for(j=0; j<9; j++){
						for(k=0; k<13; k++){
							fread(&blocks[j][k], sizeof(struct block), 1, f);
						}
					}
					if(initial%2){
						for(j=0; j<4; j++){
							new_node3 = (struct node *)malloc(sizeof(struct node));
							if(new_node3 == NULL){
								printf("memory allocation failed!\n");
            					return 1;                               
        					}
        					fread(&(new_node3->character.character), sizeof(int), 1, f);
        					if(sub_deck2 == NULL)
            					add_front(new_node3, &(sub_deck2));
        					else
            					add_end(new_node3, sub_deck2);
						}
					}
					fread(&logs_index, sizeof(int), 1, f);
					for(i=0; i<logs_index; i++){
						for(j=0; j<9; j++){
							for(k=0; k<13; k++){
								fread(&logs[i][j][k], sizeof(struct block), 1, f);
							}
						}
					}
					close(f);
					break;
				}
			}
			for(j=0; j<innocent_list_index; j++){
				new_node2 = (struct node *)malloc(sizeof(struct node));
				if(new_node == NULL){
					printf("memory allocation failed!\n");
            		return 1;                               
        		}
       			new_node2->character.character=il[j];
        		if(innocent_list == NULL)
            		add_front(new_node2, &(innocent_list));
        		else
            		add_end(new_node2, innocent_list);
			}
		break;
	}
	update_screen(blocks);
		
#if debug
	printf("The innocent list: ");
	for(i=0; i<innocent_list_index; i++){
		print_character_name(get(innocent_list, i));
		printf("   ");
	}
	if(initial%2){
		printf("\ndeck 2:\n");
		for(j=0; j<4; j++){
			print_character_name(get(sub_deck2, j));
			printf("   ");
		}
	}
	getch();
#endif

	for(i=initial; i<8; i++){
		system("cls");
		printf("Round %d starts!\n", i+1);
		print_map(blocks);
		//phase 1: choosing and using the characters
		if(i%2==0){
			linked_list_cpy(&deck, list);
			printf("Sir detective, draw the top 4 character cards from the deck after shuffling it(press any key)\n");
			getch();
			shuffle(deck);
#if debug
	printf("The deck is:");
	print_list(deck);	
#endif
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
			play(character, blocks, &n, &counter, Jack, 1, &innocent_list, logs, &logs_index);
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
				play(character, blocks, &n, &counter, Jack, 0, &innocent_list, logs, &logs_index);
			}
			printf("Detective's turn: play with the last card: ");
			character = get(sub_deck1, 0);
			remove_node(&sub_deck1, 0);
			print_character_name(character);
			printf("\n");
			n=3;
			if(character==MS) n++;
			coo=find_character(character, blocks);
			play(character, blocks, &n, &counter, Jack, 1, &innocent_list, logs, &logs_index);
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
			play(character, blocks, &n, &counter, Jack, 0, &innocent_list, logs, &logs_index);
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
				play(character, blocks, &n, &counter, Jack, 1, &innocent_list, logs, &logs_index);
			}
			printf("Jack's turn: play with the last card: ");
			character = get(sub_deck2, 0);
			remove_node(&sub_deck2, 0);
			n=3;
			if(character==MS) n++;
			print_character_name(character);
			printf("\n");
			coo=find_character(character, blocks);
			play(character, blocks, &n, &counter, Jack, 0, &innocent_list, logs, &logs_index);
		}
		update_visiblity_by_JW(blocks);
		update_visibility(blocks);
		coo=find_character(Jack, blocks);
		
		//phase 2: calling for witnesses
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
		
		//phase 3: shutting off the lights
		if(i<4){
			coo=find_numbered_light(i+1, blocks);
			blocks[coo.y][coo.x].on=0;
			blocks[coo.y][coo.x].num=0;
		}
		update_screen(blocks);
		
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
			}
			i++;
			fwrite(&i, sizeof(int), 1, f);
			fwrite(&Jack, sizeof(int), 1, f);
			innocent_list_index=0;
			while(innocent_list!=NULL){
				il[innocent_list_index]=get(innocent_list, 0);
				innocent_list=innocent_list->next;
				innocent_list_index++;
			}
			fwrite(&innocent_list_index, sizeof(int), 1, f);
			for(j=0; j<innocent_list_index; j++){
				fwrite(&il[j], sizeof(int), 1, f);
			}
			for(j=0; j<9; j++){
				for(k=0; k<13; k++){
					fwrite(&blocks[j][k], sizeof(struct block), 1, f);
				}
			}
			close(f);
			free(list);
			if(innocent_list!=NULL) free(innocent_list);
			if(i%2){
				for(j=0; j<4; j++){
					character=get(sub_deck2, j);
					fwrite(&character, sizeof(int), 1, f);
				}
				free(sub_deck2);
			}
			fwrite(&logs_index, sizeof(int), 1, f);
			for(i=0; i<logs_index; i++){
				for(j=0; j<9; j++){
					for(k=0; k<13; k++){
						fwrite(&logs[i][j][k], sizeof(struct block), 1, f);
					}
				}
			}
			return 0;
		}
	}
	printf("Time is up! Mr.Jack wins\n");
	
	//to replay the game from the end:
	printf("Do you want to see the replay of the game? 0.no 1.yes");
	scanf(" %d", &choice);
	checker(0, 1, &choice);
	if(choice){
		system("cls");
		for(i=logs_index-1; i>=0; i--){
			printf("Round %d play %d:\n", i/4+1, i+1);
			print_map(logs[i]);
			if(i!=0) printf("Press any key to see the previous play\n");
			else printf("Press any key\n");
			getch();
		}
	}
	
	free(list);
	getch();
	return 0;
}
