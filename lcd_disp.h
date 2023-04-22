#define Rs_CTRL 0x08000000 // Port 0.27
#define En_CTRL 0x10000000 // Port 0.28
#define Dt_CTRL 0x07800000 //Port 0.23 to Port 0.26
unsigned long int temp1;

void lcd_int(void);
void wr_cn(void);
void delay_cn(unsigned int);
void lcd_com(void);
void wr_cn(void);
void lcd_data(void);
void clear_ports();
void lcd_puts(unsigned char*);

void lcd_init()
{
	LPC_GPIO0->FIODIR|=Dt_CTRL;
	LPC_GPIO0->FIODIR|=Rs_CTRL;
	LPC_GPIO0->FIODIR|=En_CTRL;
	clear_ports();
	delay_lcd(3200);

	//Initial values 
	temp1=0x33;
	lcd_com();
	delay_lcd(800);
	temp1=0x32;
	lcd_com();
	delay_lcd(800);

	// Set 4 bit mode
	lcd_com();
	delay_lcd(800);

	temp1=0x0c;
	lcd_com();
	delay_lcd(800);

	return; 
}

void lcd_com() {
	temp2=temp1&0xf0;
	temp2=temp2<<19;
	wr_cn();
	delay_lcd(3000);
	temp2=temp2<<23;
	wr_cn();
	delay_lcd(32000);
	return;
}

void wr_cn()
{
	clear_ports();
	LPC_GPIO0->FIOPIN=temp2;
	LPC_GPIO0->FIOCLR=Rs_CTRL;// Assign Value to the data line
	LPC_GPIO0->FIOSET=En_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR=En_CTRL;
	return;
}

void lcd_puts(unsigned char *buf1){
	unsigned int i=0;
	while(buf1[i]!='\0')
	{
		temp1=buf1[i];
		lcd_data();
		delay_lcd(800);
		i++;
		if (i == 16)
		{
			temp1=0xc0;
			lcd_com();
			delay_lcd(800);
		}
	}
	return;
}
void lcd_data()
{
	temp2=temp1 & 0xf0;
	temp2=temp2<<19;
	wr_dn();
	delay_lcd(3000);
	temp2=temp1&0x0f;
	temp2=temp2<<23;
	wr_dn();
	return;
}
void wr_dn()
{
	clear_ports();
	LPC_GPIO0->FIOPIN=temp2;
	LPC_GPIO0->FIOSET=Rs_CTRL;
	LPC_GPIO0->FIOSET=En_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR=En_CTRL;
	return;
}