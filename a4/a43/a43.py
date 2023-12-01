#!/usr/bin/env python

from typing import IO
from typing import NamedTuple
import random as rand


class CircleShape(NamedTuple):
    """CircleShape class"""
    #Initialization of these variables comes from NamedTuple
    cx : int
    cy: int
    rad : int
    red: int
    green:int
    blue: int 
    op: float

class RectangleShape(NamedTuple):
   """RectangleShape class"""
   #Initialization of these variables comes from NamedTuple
   x: int
   y: int
   width: int
   height: int
   red: int
   green: int
   blue:int
   op: float

class EllipseShape(NamedTuple):
   """EllipseShape class"""
   #Initialization of these variables comes from NamedTuple
   cx:int 
   cy:int 
   rx: int
   ry:int
   red:int
   green:int
   blue:int
   op:float

class HtmlDocument:
    """HtmlDocument class"""
    TAB: str = "   "

    def __init__(self, file_name:str, win_title: str) -> None:
        self.win_title:str = win_title
        self.__tabs: int = 0
        self.__filename: IO = open(file_name, "w")
        
    
    def increase_indent(self) -> None:
        """increase_indent() method"""
        self.__tabs +=1

    def decrease_indent(self) -> None:
        """decrease_indent() method"""
        self.__tabs -= 1

    def append(self, content: str) -> None:
        """append() method"""
        tabs: str = HtmlDocument.TAB * self.__tabs
        self.__filename.write(f'{tabs}{content}\n')

    def write_head(self) -> None:
        """write_head() method"""
        self.append('<html>')
        self.append('<head>')
        self.increase_indent()
        self.append(f'<title>{self.win_title}</title>')
        self.decrease_indent()
        self.append('</head>')
        self.append('<body>')

    def end_body(self) -> None:
        """end_body() method"""
        self.append('</body>')

    def end_html(self) -> None:
        """end_html() method"""
        self.append('</html>')

    def write_comment(self, comment: str) -> None:
        """write_comment() method"""
        self.append(f'<!--{comment}-->')

class SvgCanvas:
    """SvgCanvas class"""
    TAB: str = "   "

    def __init__(self) -> None:
        """__init__() method"""
        self.__tabs: int = 0
        

    def svg_start(self) -> str:
        """svg_start() method"""
        return '<svg width="1000" height="1000">\n'

    def svg_end(self) -> str:
        """svg_end() method"""
        return '</svg>\n'
    
    def CircleSVG(self, circle: CircleShape) -> str:
        """CircleSVG() method"""
        line:str = "      "
        line1: str = f'<circle cx="{circle.cx}" cy="{circle.cy}" r="{circle.rad}" '
        line2: str = f'fill="rgb({circle.red}, {circle.green}, {circle.blue})" fill-opacity="{circle.op}"></circle>'
        result = line + line1 + line2 + '\n'

        return result
    def RectangleSVG(self, rect: RectangleShape) -> str:
        """RectangleSVG() method"""
        line:str = "      "
        line1:str = f'<rect x="{rect.x}" y="{rect.y}" width="{rect.width}" height="{rect.height}" '
        line2: str = f'fill="rgb({rect.red}, {rect.green}, {rect.blue})" fill-opacity="{rect.op}"></rect>\n'
        result = line + line1 + line2

        return result
    
    def EllipseSVG(self, ellipse: EllipseShape) -> str:
            """EllipseSVG() method"""
            line:str = "    "
            line1:str = f'<ellipse cx="{ellipse.cx}" cy="{ellipse.cy}" rx="{ellipse.rx}" ry="{ellipse.ry}" '
            line2:str = f'fill="rgb({ellipse.red}, {ellipse.green}, {ellipse.blue})" fill-opacity="{ellipse.op}"></ellipse>\n'
            return line + line1 + line2

    
    def gen_art(self) -> str:
        """gen_art() method"""

        num_shapes:int = rand.randint(300, 1000) #Get a random number of shapes 
        i:int = 0
        result = " "
        while(i < num_shapes):
            cur = RandomShape() #Generate a random shape
            if(cur.shape == 0):
                new_shape: CircleShape = CircleShape(cx=cur.x, cy=cur.y, rad=cur.rad, red=cur.red, green=cur.green, blue=cur.blue, op=cur.opacity)
                result += self.CircleSVG(new_shape)
            elif(cur.shape == 1):
                
                new_shape: RectangleShape = RectangleShape(x=cur.x, y=cur.y, width=cur.width, height=cur.height, red=cur.red, green=cur.green, blue=cur.blue, op=cur.opacity)
                result += self.RectangleSVG(new_shape)
            else:
               new_shape: EllipseShape = EllipseShape(cx=cur.x, cy=cur.y, rx=cur.rx, ry=cur.ry, red=cur.red, green=cur.green, blue=cur.blue, op=cur.opacity)
               result += self.EllipseSVG(new_shape)

            i+=1
        return result
        
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
        """__init__() method for the RandomShape class"""
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
        """__str__() method for the RandomShape class"""
        result:str = f'Shape: {self.shape}\n X: {self.x}\n Y: {self.y}\n Radius: {self.rad}\n RX: {self.rx}\n RY: {self.ry}\n Width: {self.width}\n Height: {self.height}\n Red: {self.red}\n Green: {self.green}\n Blue: {self.blue}\n Opacity: {self.opacity}'
        return result
    
    def  as_Part2_line(self) -> str:
        """as_Part2_line method"""
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
    """Main Method for Part 3"""
    #Create the htmldoc and SVG canvas objects
    htmldoc : HtmlDocument = HtmlDocument('a433.html', 'Part 3')
    svg: SvgCanvas = SvgCanvas()

    #Start off the HTML file
    htmldoc.write_head()

    #Open SVG Canvas
    startsvg: str = svg.svg_start()
    endsvg:str = svg.svg_end()
    htmldoc.append(startsvg)

    #Generate the random artwork
    svgart = svg.gen_art()
    htmldoc.append(svgart)


    #end SVG canvas
    htmldoc.append(endsvg)


    #Close off the html tags
    htmldoc.end_body()
    htmldoc.end_html()

   
    
    

if __name__ == "__main__":
    main()
