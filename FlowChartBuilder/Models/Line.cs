using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Models
{
    public class Line
    {
        private List<Coordinates> Points { get; set; }

        public Line()
        {
            this.Points = new List<Coordinates>();
        }

        public List<Coordinates> GetPointsOfLine()
        {
            return this.Points;
        }

        public void AddPointToLine(int x, int y)
        {
            Points.Add(new Coordinates(x, y));
        }

        public Line(Line line)
        {
            this.Points = new List<Coordinates>();
            foreach (var point in line.GetPointsOfLine())
            {
                this.Points.Add(new Coordinates(point.x, point.y));
            }
        }

        public void DisplayLine()
        {
            foreach (var point in this.Points)
            {
                Console.WriteLine("(" + point.x + " , " + point.y + ")");
            }
        }
    }
}
