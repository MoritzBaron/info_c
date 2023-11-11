#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1 PC1
#define LED2 PC2
#define LED3 PC3
#define LED4 PC4
#define LED5 PC5
#define LED6 PD4
#define LED7 PD5
#define LED8 PD6
#define LED9 PD7
#define LED10 PB0

#define BUTTON1 PD1

static unsigned char currentLED = 0;

static volatile unsigned char ledIndex = 0;
static volatile unsigned char buttonState = 0;
static volatile unsigned char currentState = 0;
static volatile unsigned char counter = 0;

void init(void){
    DDRB |= 0x01;   //output
    DDRC |= 0x3E;   //output
    DDRD |= 0xF0;   //output
    
    //Button konfig
    DDRD &= ~(1<<PORTD1);   //input
    PORTD |= (1<<PORTD1);   //pull-up
    
    LED_OFF();
}

void init_Timer(void){
    TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);   //wgm12 für ctc mode //cs12/10 für OCR
    OCR1A = 15;  //vergleichswert so gesetzt dass bei 0,001 sek interrupt stattfindet(wg prescaler)
    TIMSK1 |= (1<<OCIE1A);      //interrupt wird ausgelöst
}

void LED_ON(){
    currentLED ++;
    
    if(currentLED > 10){
        currentLED = 1;
    }
    
    switch(currentLED){
        case 1:
            PORTC |= (1 << LED1);
            break;
        case 2: 
            PORTC |= (1 << LED2); 
            break;
        case 3: 
            PORTC |= (1 << LED3); 
            break;
        case 4: 
            PORTC |= (1 << LED4);
            break;
        case 5:
            PORTC |= (1 << LED5);
            break;
        case 6:
            PORTD |= (1 << LED6); 
            break;
        case 7: 
            PORTD |= (1 << LED7); 
            break;
        case 8: 
            PORTD |= (1 << LED8); 
            break;
        case 9: 
            PORTD |= (1 << LED9); 
            break;
        case 10:
            PORTB |= (1 << LED10); 
            break;
    }
}

void LED_OFF(void){
    PORTB &= ~0x01;
    PORTC &= ~0x3E;
    PORTD &= ~0xF0;
}

ISR(TIMER1_COMPA_vect) {
    
    if (bit_is_clear(PIND, BUTTON1)) {    //Button gedrückt?
    currentState = 1;
    } else {
    currentState = 0;
    }

    if (currentState != buttonState) {      //Counter für entprellen
        counter ++;
        if (counter >= 2) { 
            LED_OFF();
            LED_ON();
            counter = 0;
        }
    }
    buttonState = currentState; // Zustand zurücksetzen
}

int main(void){
    init();
    init_Timer();
    sei();
    while(1);
}

