#define BAUD_RATE 9600

int incoming_byte = 0;

void setup()
{
    Serial.begin(BAUD_RATE);
}

void loop()
{

    if (Serial.available() > 0)
    {
        incoming_byte = Serial.read();
        Serial.println(incoming_byte);
    }

}
