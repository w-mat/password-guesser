//TO JEST TEST INO

const int pinSwitch = 2;

int
    currSwitch,
    lastSwitch;
unsigned long
    switchCount;

void setup()
{
    pinMode(pinSwitch, INPUT_PULLUP);
    lastSwitch = digitalRead(pinSwitch);
    switchCount = 0;
    Serial.begin(9600);

} //setup

void loop()
{
    //read the switch pin...
    currSwitch = digitalRead(pinSwitch);
    //wait 20mS and read it again
    delay(20);
    //if it's the same we assume no bounce...
    if (digitalRead(pinSwitch) == currSwitch)
    {
        //if the switch is low now but was high then we
        //sensed a keypress
        if (currSwitch == LOW && currSwitch != lastSwitch)
        {
            //bump the count and show it
            switchCount++;
            Serial.print("Switch count: ");
            Serial.println(switchCount);

        } //if

        //save this valid state as the last
        lastSwitch = currSwitch;

    } //if

} //loop
