#define BAUD_RATE 9600
#define MAX_SIZE_PAYLOAD 12

void read_payload(char *payload)
{
    if (strcmp(payload, "Hi InoDAQ") == 0)
    {
        Serial.println("Hello from InoDAQ");
    }
    else
    {
        Serial.println(payload);
    }
}

void setup()
{
    Serial.begin(BAUD_RATE);
}

void loop()
{
    if (Serial.available() > 0)
    {
        static char payload[MAX_SIZE_PAYLOAD];
        static unsigned int payload_idx = 0;

        char incoming_byte = Serial.read();

        if ((incoming_byte != '\n') and (payload_idx < MAX_SIZE_PAYLOAD -1))
        {
            payload[payload_idx] = incoming_byte;
            payload_idx++;
        }
        else
        {
            payload[payload_idx] = '\0';
            read_payload(payload);
            payload_idx = 0;
        }
    }
}