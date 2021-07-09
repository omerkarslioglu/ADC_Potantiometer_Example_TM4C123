# ADC_Potantiometer_Example_TM4C123
ADC &amp; Potantiometer Example On TM4C123

# General Purpose of This Laboratory : 

The main objective of this experiment is to interface a potentiometer with EKTM4C123G GPIO (PE3) by configuring it as an Analog Input (AN0) and to observe its corresponding 12-bit digital value. In this experiment, we will also understand the Analog-to-Digital Conversion (ADC) and processing of the analog signals in a digital environment.

# Pin/Port Selection :

I have made my connections as in the image below.
I connected the middle leg of the potentiometer to PE3.
I put a resistor between +3.3 v and pot leg for safety reasons.

![image](https://user-images.githubusercontent.com/67158049/125127366-16e31900-e105-11eb-9606-4b32d2904167.png)

What is the ADC Peripheral ?
First of all, let's answer this what is the duty of the ADC. ADC is
the name of the structures that convert an analog signal to a
digital signal. I can convert and process analog signals to digital
with the ADC structure.
Tiva Launchpad has two 12-bit resolution ADCs. These are ADC0
and ADC1. ADC0 and ADC1 have the same structure. So when
choosing the ADC we will use, we can choose any of them.
These two ADC modules provide the following features:
12 shared analog input channels
12-bit precision ADC
Single-ended and differential-input configurations
On-chip internal temperature sensor
Maximum sample rate of one million samples/second (MSPS)
Optional phase shift in sample time programmable
Four programmable sample conversion sequencers from one to
eight entries long, with corresponding conversion result FIFOs
Five flexible trigger controls
-Software Trigger Controller (default)
-Timers
-Analog Comparators
-PWM
-GPIO
Hardware averaging of up to 64 samples
2 Analog Comparators

![image](https://user-images.githubusercontent.com/67158049/125127447-35491480-e105-11eb-9cb2-d19bbeab548f.png)

What does 12 bit resolution mean ?
It means that analog values will be expressed as digital values at 2 ^
12 levels. For example, if the resolution is 2 and the analog value
changes between 0 and 10V ,
For ADC in Tiva, the digital range is from 0 to 2^12 -1 (0 to 4095).
So it corresponds to 0V = 0 and 3.3V = 4095.
So, why 4095 (2 ^ 12 - 1) ?
It was said that both ADCs have 12 bit resolution. This means that
212 values can be created for 12 bits. It takes value up to 2^12 - 1,
that is, 4095, within which we start from 0.

![image](https://user-images.githubusercontent.com/67158049/125127483-4134d680-e105-11eb-9430-ef96f6ba2172.png)

As can be seen from the diagram above, the input channels and
triggers of both ADCs are the same. As we can see from the
datasheet, which of the input pins are valid for both ADCs, it is also
shown below ...

Look to the report to continue to the article ...


All of the things about project in following link :

https://www.youtube.com/watch?v=fReR-6Vf7V0
https://github.com/omerkarslioglu/ADC...

Also you can look my other project like the project :

https://www.youtube.com/watch?v=MFl8q...

https://www.youtube.com/watch?v=5BAgs...

https://www.youtube.com/watch?v=RsW6q...

Thanks for reading and watching ...

Ömer Karslıoğlu
