#!/usr/bin/env python
"""Assignment 4 Part 1"""
print(__doc__)

from typing import IO

class HtmlDoc:
    """HTMLDoc class which with methods that can write html code for head, body, and html"""
    def __init__(self) -> None:
        pass

    def writeHTMLstart(self) -> None:
        print("<html>\n")

    def writeHTMLend(self) -> None:
        print("</html>\n")

    def writeBodystart(self) -> None:
        print("<body>\n")

    def writeBodyend(self) -> None:
        print("</body>\n")

    def writeHeadstart(self) -> None:
        print("<head>\n")

    def writeHeadend(self) -> None:
        print("</head>\n")

    def writeHTMLcomment(self, t: int, com: str) -> None:
        """writeHTMLcomment method"""
        ts: str = "   " * t
        print(f"{ts}<!--{com}-->\n")
    def writeHTMLHeader(self, winTitle: str) -> None:
        """writeHeadHTML method"""
        
        print(f"<title>{winTitle}</title>\n")

    def writeHTMLline(self, t: int, line: str) -> None:
         """writeLineHTML method"""
         ts = "   " * t
         print(f"{ts}{line}\n")

class SvgCanvas:
    """SvgCanvas class with methods to generate html <svg> tags, and a gen_art method"""
    def __init__(self) -> None:
        pass

    def writeSVGstart(self, t:int, canvas:tuple) -> None:
        
        ts: str = "   " * t
        print(f'{ts}<svg width="{canvas[0]}" height="{canvas[1]}">\n')
        

    def writeSVGend(self, f:IO[str]) -> None:
        print("</svg>\n")

    def gen_art(self, t: int) -> None:
        """Method to generate the art"""
        
        return
    
    def writeHTMLcomment(self, t: int, com: str) -> None:
        """writeHTMLcomment method"""
        ts: str = "   " * t
        print(f"{ts}<!--{com}-->\n")
        
class Circle:
    """Circle class"""
    def __init__(self, cir: tuple, col: tuple) -> None:
        self.cx: int = cir[0]
        self.cy: int = cir[1]
        self.rad: int = cir[2]
        self.red: int = col[0]
        self.green: int = col[1]
        self.blue: int = col[2]
        self.op: float = col[3]

    def draw_Circle(self, t: int) -> None:
        ts: str = "   " * t
        line1: str = f'<circle cx="{self.cx}" cy="{self.cy}" r="{self.rad}" '
        line2: str = f'fill="rgb({self.red}, {self.green}, {self.blue})" fill-opacity="{self.op}"></circle>'
        print(f"{ts}{line1+line2}\n")
        

class Rectangle:
    """Rectangle Class"""
    def __init__(self, corner: tuple, dimensions: tuple, col: tuple) -> None:
        self.x: int = corner[0]
        self.y: int = corner[1]
        self.width: int = dimensions[0]
        self.height: int = dimensions[1]
        self.red: int = col[0]
        self.green: int = col[1]
        self.blue: float = col[2]
        self.op: float = col[3]
    def draw_Rectangle(self, t:int) -> None:
        ts: str = "   " * t
        line1: str = f'<rect x="{self.x}" y="{self.y}" width="{self.width}" height="{self.height}" '
        line2: str = f'fill="rgb({self.red}, {self.green}, {self.blue})" fill-opacity="{self.op}"></rect>'
        print(f"{ts}{line1+line2}\n")

class Ellipse:
    """Ellipse Class"""
    def __init__(self, center: tuple, rxry: tuple, col: tuple) -> None:
        self.cx: int = center[0]
        self.cy: int = center[1]
        self.rx: int = rxry[0]
        self.ry: int = rxry[1]
        self.red: int = col[0]
        self.green: int = col[1]
        self.blue: float = col[2]
        self.op: float = col[3]

    def draw_Ellipse(self, t:int) -> None:
        ts: str = "   " * t
        line1: str = f'<ellipse cx="{self.cx}" cy="{self.cy}" rx="{self.rx}" ry="{self.ry}" '
        line2: str = f'fill="rgb({self.red}, {self.green}, {self.blue})" fill-opacity="{self.op}"></ellipse>'
        print(f"{ts}{line1+line2}\n")

def main() -> None:
    """main method"""
    #Red Circle objects
    c1 = Circle((50,50,50,), (255,0,0,1.0))
    c2 = Circle((150,50,50), (255,0,0,1.0))
    c3 = Circle((250,50,50), (255,0,0,1.0))
    c4 = Circle((350,50,50), (255,0,0,1.0))
    c5 = Circle((450,50,50), (255,0,0,1.0))
    #Blue circle objects
    c6 = Circle((50,250,50), (0,0,255,1.0))
    c7 = Circle((150,250,50), (0,0,255,1.0))
    c8 = Circle((520,250,50), (0,0,255,1.0))
    c9 = Circle((350,250,50), (0,0,255,1.0))
    c10 = Circle((450,250,50), (0,0,255,1.0))

    start = HtmlDoc()
    svg = SvgCanvas()

    start.writeHTMLstart()
    start.writeHeadstart()
    start.writeHTMLHeader("Part 1 Art")
    start.writeHeadend()
    start.writeBodystart()
    svg.writeSVGstart()
    svg.gen_art()
    svg.writeSVGend()
    start.writeBodyend()
    start.writeHTMLend()

  
    return
if __name__ == "__main__":
    main()
    
        