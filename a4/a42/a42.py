#!/usr/bin/env python
"""Assignment 4 Part 1"""
print(__doc__)

from typing import IO
from typing import NamedTuple
import random as rand

class PyArtConfig:
    """PyArtConfig class"""
    def __init__(self) -> None:
        """__init__() method"""
        #Min and Max Value Variables

        self.colorMAX:int = 255
        self.colorMIN:int = 1

        self.ellipseMIN:int = 10
        self.ellipseMAX:int = 30

        self.HEIGHTWIDTHRAD_MAX:int = 100

        self.shape_MAX:int = 2
        self.shape_MIN:int = 0

        self.rectMin = 10
        self.rectMax = 100

        

class RandomShape:
    """RandomShape class"""
    def __init__(self) -> None: 
        """__init__() method"""
        cur = PyArtConfig() #Get the min and max variables to generate the random values


        self.shape: int = rand.randint(cur.shape_MIN, cur.shape_MAX)
        self.x:int = rand.randint(1, 1000)
        self.y:int = rand.randint(1, 1000)
        self.rad:int = rand.randint(1, 100)
        self.rx:int = rand.randint(cur.ellipseMIN, cur.ellipseMAX)
        self.ry :int = rand.randint(cur.ellipseMIN, cur.ellipseMAX)
        self.width:int = rand.randint(cur.rectMin, cur.rectMax)
        self.height:int = rand.randint(cur.rectMin, cur.rectMax)
        self.red:int  = rand.randint(cur.colorMIN, cur.colorMAX)
        self.green:int = rand.randint(cur.colorMIN, cur.colorMAX)
        self.blue:int = rand.randint(cur.colorMIN, cur.colorMAX)
        self.opacity: float = rand.random()


    def __str__(self) -> str:
        """__str__() method"""
        result:str = f'Shape: {self.shape}\n X: {self.x}\n Y: {self.y}\n Radius: {self.rad}\n RX: {self.rx}\n RY: {self.ry}\n Width: {self.width}\n Height: {self.height}\n Red: {self.red}\n Green: {self.green}\n Blue: {self.blue}\n Opacity: {self.opacity}'
        return result
    
    def  as_Part2_line(self) -> str:
        """as_Part2_line() method"""
        return f'{self.shape}  {self.x}  {self.y}  {self.rad}  {self.rx}  {self.ry}  {self.width}  {self.height}  {self.red}  {self.green}  {self.blue}  {self.opacity:.1f}'
    
    def as_svg(self) -> str:
        """as_svg() method"""

        if(self.shape == 0):
            ts: str = "   "
            line1: str = f'<circle cx="{self.x}" cy="{self.y}" r="{self.rad}" '
            line2: str = f'fill="rgb({self.red}, {self.green}, {self.blue})" fill-opacity="{self.opacity}"></circle>\n'
            return ts + line1 + line2
        elif(self.shape == 1):
            line:str = "    "
            line1:str = f'<rect x="{self.x}" y="{self.y}" width="{self.width}" height="{self.height}" '
            line2: str = f'fill="rgb({self.red}, {self.green}, {self.blue})" fill-opacity="{self.opacity}"></rect>'
            result = line + line1 + line2
            return result
        
        else:
            line:str = "    "
            line1:str = f'<ellipse cx="{self.x}" cy="{self.y}" rx="{self.rx}" ry="{self.ry}" '
            line2:str = f'fill="rgb({self.red}, {self.green}, {self.blue})" fill-opacity="{self.opacity}></ellipse>\n'
            return line + line1 + line2
        
    
def main() -> None:
    """main method"""
    
    print('CNT  SHA  X  Y  RAD  RX  RY   W   H   R   G   B    OP') #Top row of Table
    i:int = 0

    while(i < 10):
        cur = RandomShape()
        line = cur.as_Part2_line()
        print(f'{i}  {line}')
        i +=1

    
    

if __name__ == "__main__":
    main()
