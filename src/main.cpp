#include <Arduino.h>
#include <avr/interrupt.h>

char x;

#define bitanzahl 5
#define mask ((1<<bitanzahl) - 1)
#define S1M (1<<4)
#define S2M (1<<5)

#define SCHALTERMASK (S1M|S2M)

int ct;
volatile char ein;
int speeds[4] = {10,20,50, 100};
volatile unsigned char aktualSpeed;

ISR(INT4_vect) {
    ein = !ein;
}
ISR(INT5_vect) {
    aktualSpeed = (aktualSpeed+1)%4;
}

void setup() {
    //DDRK = 0b011111;
    //DDRK = 0x1F;
    DDRK = mask;
    x = 1;

    DDRE  = 0;       // ganzer PortE als Eingang
    PORTE = SCHALTERMASK;  // Pull-Up für alle Schalter

    ct  = 0;
    ein = 0;
    aktualSpeed = 1;

    EICRB = (2<<ISC40) | (2<<ISC50);
    EIMSK = (1<<INT4)  | (1<<INT5);
    sei();
}

void loop() {
    if (ein) {
        PORTK = (PORTK & ~mask) | (x & mask);
        delay(10);
        if (++ct>=speeds[aktualSpeed]) {
            x <<= 1;
            if (x > mask) x = 1;
            ct=0;
        }
    } else {
        PORTK = (PORTK & ~mask);
        ct = 0;
        x  = 1;
    }
}