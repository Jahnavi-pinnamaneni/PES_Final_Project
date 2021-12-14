# PES_Final_Project
Explores FreeRTOS


Two Tasks are scheduled
	MMATask - 500ms
	LCDTask - 600ms

SysTick is used by FreeRTOS functions hence, the function xTaskGetTickCount() is used to get the time that it takes
between tasks. This is tested with assert to see if the tasks get scheduled correctly everytime.

MMA8541 is calibrated before the tasks get scheduled and the offset values are later used in the execution.

Video_link:
https://drive.google.com/file/d/1qDIEettvljOSUPC7I78TEFHoHU1rCXYI/view?usp=sharing