#include <Arduino.h>
#include <avr/interrupt.h>

char x;

#define bitanzahl 5
#define mask ((1<<bitanzahl) - 1)

ISR(INT4_vect) {
    //TODO Muster wechseln

}
ISR(PCINT1_vect) {
    //TODO Auswertung des Inkrementalgebers
    unsigned char ig;
    ig = PINJ & 0x03;
    static unsigned char ig_alt=0;
    // Auswerten von Flanke auf A
    if ( !(ig_alt&(1<<0)) && (ig&(1<<0)) ) {
        if (ig&(1<<1)) {
            // links
            x <<= 1;
            if (x > mask) x = 1;
        } else {
            //rechts
            x >>= 1;
            if (x == 0) x = 1<<(bitanzahl-1);
        }
    }
    ig_alt=ig;
}

void setup() {
    DDRK = mask;
    x = 1;

    DDRE  = 0;       // ganzer PortE als Eingang
    DDRJ  = 0;       // ganzer PortJ als Eingang

    // EXT-INT
    EICRB = (2<<ISC40);
    EIMSK = (1<<INT4);

    // PCINT
    PCICR  = 1<<PCIE1;
    PCMSK1 = (1<<PCINT9) | (1<<PCINT10);

    sei();
}

void loop() {
    PORTK = (PORTK & ~mask) | (x & mask);
}