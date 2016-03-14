# Change digital image into a watercolor style

## Referenced from 
```
¡¶Towards Photo Watercolorization with Artistic Verisimilitude¡·
```

## Environment require
	To run this program,you need to install:
	* VS 2013+  (Do not use VS 2015 if possible for some strange things may happen)
	* OpenCV 2.4.11+

## Program Structure
WaterColor
	$ WaterColor water
	$ water.deal(inputImg,outputImg)

There are 5 steps processed by WaterColor:
1.ColorAdjustment
2.SaliencyDistance
3.Abstraction
4.WetInWet
5.HandTremorEffect( still some bugs,maybe repair in the future)
6.EdgeDarkening(same as 6)
7.Granulation(same as 6)
8.TurblenceFlow(same as 6)
9.After deal(same as 6)


## Examples
program examples locate in /watercolor/process/pic/*src|dst.jpg|png 
referenced paper examples locate in /example from paper
@Tianlong Yang (yangtianlong@gmail.com)    In 2016.3.14