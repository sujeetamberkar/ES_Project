#include <LPC17xx.h>
#include "lcd_disp.h"  // Make sure to create a corresponding header file for your LCD library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void scan(void);
unsigned char row,flag,key;
unsigned long int i, var1,temp,temp2,temp3;
unsigned char scan_code[16]={0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,0x14,0x24,0x44,0x84,0x18,0x28,0x48,0x88};
unsigned char ascci_code[16]={'1','2','3','@','4','5','6','%','7','8','9','&','*','0','#','_'};
unsigned char KEY;
int index=1;
unsigned char Msg3={"Memory full"};
unsigned char Msg4={"Memory empty"};
unsigned char output[32];
unsigned char Temp_output[128]; 
#define MAX_SIZE 128
typedef struct {
    int front;
    int rear;
    int data[MAX_SIZE];
} Queue;

int is_full(Queue *queue) {
    lcd_puts(Msg3);
    delay_cn(800);
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}


int is_empty(Queue *queue) {
    lcd_puts(Msg4);
    delay_cn(800);
    return queue->front == -1;
}

void enqueue(Queue *queue, int value) {
    if (is_full(queue)) {
        printf("Error: Queue is full!\n");
        return;
    }
    if (is_empty(queue)) {
        queue->front = 0;
    }
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    queue->data[queue->rear] = value;
}


int dequeue(Queue *queue) {
    if (is_empty(queue)) {
        printf("Error: Queue is empty!\n");
        return -1;
    }
    int value = queue->data[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1; // Reset the queue
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }
    return value;
}

void queue_to_array(Queue *q, char *arr) {
    int index = 0;
    while (!is_empty(q)) {
        arr[index++] = dequeue(q);
    }
    arr[index] = '\0'; // Null-terminate the character array
}
void print_on_LCD(char *arr){
    temp1=01;
    lcd_com();
    delay_cn(800);
    lcd_puts(&arr[0]);
}

// Mapping of numbers to their corresponding letters on a Nokia keypad
const char* nokia_keypad[] = {
    " 0", ".,?1", "ABC2", "DEF3",
    "GHI4", "JKL5", "MNO6",
    "PQRS7", "TUV8", "WXYZ9"
};

char get_char_from_keypad(int key, int presses) {
    const char* keypad_chars = nokia_keypad[key];
    int num_chars = strlen(keypad_chars);
    return keypad_chars[(presses - 1) % num_chars];
}

void decode_nokia_keypad(const char* input, char* output) {
    int key = 0;
    int presses = 0;
    int out_idx = 0;
    for (size_t i = 0; i < strlen(input); ++i) {
        if (input[i] >= '0' && input[i] <= '9') {
            key = input[i] - '0';
            presses++;
        } else if (input[i] == '_') {
            output[out_idx++] = get_char_from_keypad(key, presses);
            presses = 0;
        }
    }
    if (presses > 0) {
        output[out_idx++] = get_char_from_keypad(key, presses);
    }
    output[out_idx] = '\0';
}

int main(void)
{
    SystemInit();// Clock and Pclk configuration 
    
    //Intialize LCD
    lcd_init();
    temp1=0x80;
    lcd_com();
    delay_cn(800);


    //Intialization for keyboard
    LPC_GPIO2->FIODIR |= 0x00003c00; // Port 2.10 - 2.13 are made output ports
    LPC_GPIO1->FIODIR &= 0xF87FFFFF; // Port 1.23- 1.26 are made input ports 
    unsigned int counter = 1;

    Queue queue;
    queue.front = queue.rear = -1;
    while (1)
    {
        while (1)
        {
            for (row = 1; row < 5; row++)
            {
                if (row == 1)
                    var1 = 0x00000400;
                else if (row == 2)
                    var1 = 0x00000800;
                else if (row == 3)
                    var1 = 0x00001000;
                else if (row == 4)
                    var1 = 0x00002000;

                temp = var1;
                LPC_GPIO2->FIOCLR = 0x00003c00; // Clear the ports and send appropriate data
                LPC_GPIO2->FIOSET = var1;       // Enabling rows
                flag = 0;
                scan();
                if (flag == 1)
                    break;
            } // End for for loop
            if (flag == 1)
                break;
        } // End 2nd While Loop

        for (i = 0; i < 16; i++)
        {
            if (key == scan_code[i])
            {
                KEY = ascci_code[i];
                enqueue(&queue, KEY);
            }
        }
    }

    queue_to_array(&queue,Temp_output);
    decode_nokia_keypad(Temp_output, output);
    print_on_LCD(output);
    
}

void scan()
{
    temp3 = LPC_GPIO1->FIOPIN;
    if (temp3 != 0x00000000)
    {
        flag = 1;
        temp3 >>= 19;
        temp >>= 10;
        key = temp3 | temp; // To get scan code
    } // if temp3!= 0x00000000
}
