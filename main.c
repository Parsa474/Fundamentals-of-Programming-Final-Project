#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "Mr.Jack.h"
#define debug 0

int main(){
	srand(time(0));
	struct block blocks[9][13], blk;
	int i, j, k, x, y, choice, character, n;
	bool cont=1;
	char MrJack_identity[2];
	struct node *list=NULL, *new_node, *deck=NULL, *sub_deck1=NULL, *sub_deck2=NULL, *innocent_list=NULL;
	FILE *f;
	blk.identifier=street;
	blk.blocked=0; blk.direction=0; blk.num=0; blk.on=0; blk.sus=0; blk.visible=0, blk.well=0;
	for(y=0; y<9; y++){
		for(x=0; x<13; x++){
			blocks[y][x]=blk;
		}
	}
	if(read_locations("buildings.txt", building, blocks)) return 1;
	if(read_locations("lights.txt", light, blocks)) return 1;
	if(read_locations("wells.txt", street, blocks)) return 1;
	if(read_locations("extras.txt", extra, blocks)) return 1;
	if(read_locations("escapes.txt", esc, blocks)) return 1;
	f=fopen("characters.txt", "r");
	if(f==NULL) {printf("Could not open the characters' file\n"); return 1;}
	for(i=0; i<8; i++){
		fscanf(f, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &x, &y, &blk.sus, &blk.visible, &blk.well, &blk.blocked, &blk.on, &blk.direction, &blk.num);
		blk.identifier=i;
		blocks[y][x]=blk;
	}
	close(f);
	do{
		system("cls");
		printf("Who wants to be Mr.Jack?\n1.Player 1\n2.Player 2\n");
		printf("Enter 1 or 2: ");
		scanf(" %d", &choice);
	} while(choice!=1 && choice!=2);
	system("cls");
	switch(choice){
		case 1:
			printf("Player 1 is Mr.Jack and Player 2 is the Detective, let the game begin (;\n");
		break;
		case 2:
			printf("Player 2 is Mr.Jack and Player 1 is the Detective, let the game begin (;\n");
		break;
	}
	print_map(blocks);				
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
	printf("Mr.Jack, draw your character card(press any key), Sir detective you must look away here\n");
	getch();
	linked_list_cpy(&innocent_list, list);
	choice = get(innocent_list, 0);
	remove_node(&innocent_list, 0);
	switch(choice){
		case SH:
			strcpy(MrJack_identity, "SH");
			printf("Mr.Jack is Sherlock Holmes(SH)\n");
		break;
		case JW:
			strcpy(MrJack_identity, "JW");
			printf("Mr.Jack is John H. Watson(JW)\n");
		break;
		case JS:
			strcpy(MrJack_identity, "JS");
			printf("Mr.Jack is John Smith(JS)\n");
		break;
		case IL:
			strcpy(MrJack_identity, "IL");
			printf("Mr.Jack is Inspector Lestrade(IL)\n");
		break;
		case MS:
			strcpy(MrJack_identity, "MS");
			printf("Mr.Jack is Miss Stealthy(MS)\n");
		break;
		case SG:
			strcpy(MrJack_identity, "SG");
			printf("Mr.Jack is Sergeant Goodley(SG)\n");
		break;
		case WG:
			strcpy(MrJack_identity, "WG");
			printf("Mr.Jack is William Gull(WG)\n");
		break;
		case JB:
			strcpy(MrJack_identity, "JB");
			printf("Mr.Jack is Jeremy Bert(JB)\n");
		break;
		default:
			printf("Failed to get the identity of Mr.Jack\n");
		break;
	}
	close(f);
	printf("Press any key to continue\n");
	getch();
	system("cls");
	for(i=0; i<1; i++){
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
			n=2;
			if(character==MS) n++;
			printf("You chose: ");
			print_character_name(character);
			remove_node(&sub_deck1, choice-1);
			printf("\n");
			move(character, blocks, 1);
			cont=1;
			for(j=0; cont && j<n; j++){
				cont = move(character, blocks, 0);
			}
			update_screen(blocks);
			for(k=0; k<2; k++){
				printf("Jack's turn: which character do you want to play?(enter the number amongst the remaining cards from 1 to %d)\n", 3-k);
				for(j=0; j<3-k; j++){
				print_character_name(get(sub_deck1, j));
				printf("   ");
				}
				scanf(" %d", &choice);
				checker(1, 3-k, &choice);
				character = get(sub_deck1, choice-1);
				n=2;
				if(character==MS) n++;
				printf("You chose: ");
				print_character_name(character);
				remove_node(&sub_deck1, choice-1);
				printf("\n");
				move(character, blocks, 1);
				cont=1;
				for(j=0; cont && j<n ; j++){
					cont = move(character, blocks, 0);
				}
				update_screen(blocks);
			}
			printf("Detective's turn: play with the last card: ");
			character = get(sub_deck1, 0);
			n=2;
			if(character==MS) n++;
			print_character_name(character);
			remove_node(&sub_deck1, choice-1);
			printf("\n");
			move(character, blocks, 1);
			cont=1;
			for(j=0; cont && j<n; j++){
				cont = move(character, blocks, 0);
			}
			update_screen(blocks);
		}else{
			deck=list;
			printf("Jack, reveal the 4 remaining cards(press any key)\n");
			getch();
			if(debug){
				printf("The deck is:");
				print_list(deck);	
			}
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
			n=2;
			if(character==MS) n++;
			printf("You chose: ");
			print_character_name(character);
			remove_node(&sub_deck2, choice-1);
			printf("\n");
			move(character, blocks, 1);
			cont=1;
			for(j=0; cont && j<n; j++){
				cont = move(character, blocks, 0);
			}
			update_screen(blocks);
			for(k=0; k<2; k++){
				printf("Detective's turn: which character do you want to play?(enter the number amongst the remaining cards from 1 to %d)\n", 3-k);
				for(j=0; j<3-k; j++){
				print_character_name(get(sub_deck2, j));
				printf("   ");
				}
				scanf(" %d", &choice);
				checker(1, 3-k, &choice);
				character = get(sub_deck2, choice-1);
				n=2;
				if(character==MS) n++;
				printf("You chose: ");
				print_character_name(character);
				remove_node(&sub_deck2, choice-1);
				printf("\n");
				move(character, blocks, 1);
				cont=1;
				for(j=0; cont && j<n; j++){
					cont = move(character, blocks, 0);
				}
				update_screen(blocks);
			}
			printf("Jack's turn: play with the last card: ");
			character = get(sub_deck2, 0);
			n=2;
			if(character==MS) n++;
			print_character_name(character);
			remove_node(&sub_deck2, choice-1);
			printf("\n");
			move(character, blocks, 1);
			cont=1;
			for(j=0; cont && j<n; j++){
				cont = move(character, blocks, 0);
			}
			update_screen(blocks);
		}
	}
	if(innocent_list!=NULL) free(innocent_list);
	return 0;
}
